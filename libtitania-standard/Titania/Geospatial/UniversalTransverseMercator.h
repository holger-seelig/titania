/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_GEOSPATIAL_UNIVERSAL_TRANSVERSE_MERCATOR_H__
#define __TITANIA_GEOSPATIAL_UNIVERSAL_TRANSVERSE_MERCATOR_H__

#include "../Math/Functional.h"
#include "Geodetic.h"

namespace titania {
namespace geospatial {

///  @name Convert universal transverse mercator (utm) coordinates to geocentric coordinates
template <class Type>
class universal_transverse_mercator :
	public basic_converter <Type>
{
public:

	constexpr
	universal_transverse_mercator (const spheroid3 <Type> & spheroid, const int zone, const bool southern_hemisphere = false, const bool easting_first = false) :
		southern_hemisphere (southern_hemisphere),
		      easting_first (easting_first),
		                  a (spheroid .a ()),
		                 ee (1 - sqr (spheroid .c () / a)),
		                 EE (ee / (1 - ee)),
		                 E8 (8 * EE),
		                 E9 (9 * EE),
		               E252 (252 * EE),
		                 e1 ((1 - std::sqrt (1 - ee)) / (1 + std::sqrt (1 - ee))),
		                  A (k0 * (a * (1 - ee / 4 - 3 * ee * ee / 64 - 5 * ee * ee * ee / 256))),
		                  B (3 * e1 / 2 - 7 * e1 * e1 * e1 / 32),
		                  C (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32),
		                  D (151 * e1 * e1 * e1 / 96),
		                  E (a * (1 - ee)),
		         longitude0 (radians (Type (zone) * 6 - 183)),
		 geodetic_converter (spheroid)
	{ }

	virtual
	vector3 <Type>
	convert (const vector3 <Type> & utm) const final override;

	virtual
	vector3 <Type>
	apply (const vector3 <Type> & geocentric) const final override;

private:

	static constexpr Type N0 = 1.0e7;
	static constexpr Type E0 = 5.0e5;
	static constexpr Type k0 = 0.9996;

	const bool southern_hemisphere;
	const bool easting_first;

	const Type a;

	const Type ee;
	const Type EE;
	const Type E8;
	const Type E9;
	const Type E252;
	const Type e1;

	const Type A;
	const Type B;
	const Type C;
	const Type D;
	const Type E;
	const Type longitude0;

	geodetic <Type> geodetic_converter;

};

template <class Type>
vector3 <Type>
universal_transverse_mercator	<Type> ::convert (const vector3 <Type> & utm) const
{
	Type northing = easting_first ? utm .y () : utm .x ();
	Type easting  = easting_first ? utm .x () : utm .y ();

	// Check for southern hemisphere and remove offset from easting.

	bool S = southern_hemisphere;

	if (northing < 0)
	{
		S        = true;
		northing = -northing;
	}

	if (S)
		northing = northing - N0;

	easting -= E0;

	// Begin calculation.

	const Type mu = northing / A;

	const Type phi1 = mu + B* std::sin (2 * mu) + C* std::sin (4 * mu) + D* std::sin (6 * mu);

	const Type sinphi1 = sqr (std::sin (phi1));
	const Type cosphi1 = std::cos (phi1);
	const Type tanphi1 = std::tan (phi1);

	const Type N1 = a / std::sqrt (1 - ee * sinphi1);
	const Type T2 = sqr (tanphi1);
	const Type T8 = std::pow (tanphi1, 8);
	const Type C1 = EE * T2;
	const Type C2 = sqr (C1);
	const Type R1 = E / std::pow (1 - ee * sinphi1, 1.5);
	const Type I  = easting / (N1 * k0);

	const Type J = (5 + 3 * T2 + 10 * C1 - 4 * C2 - E9) * std::pow (I, 4) / 24;
	const Type K = (61 + 90 * T2 + 298 * C1 + 45 * T8 - E252 - 3 * C2) * std::pow (I, 6) / 720;
	const Type L = (5 - 2 * C1 + 28 * T2 - 3 * C2 + E8 + 24 * T8) * std::pow (I, 5) / 120;

	const Type latitude = phi1 - (N1 * tanphi1 / R1) * (I * I / 2 - J + K);

	const Type longitude = longitude0 + (I - (1 + 2 * T2 + C1) * std::pow (I, 3) / 6 + L) / cosphi1;

	return geodetic_converter (vector3 <Type> (latitude, longitude, utm .z ()));
}

template <class Type>
vector3 <Type>
universal_transverse_mercator	<Type> ::apply (const vector3 <Type> & geocentric) const
{
	return vector3 <Type> ();
}

extern template class universal_transverse_mercator <float>;
extern template class universal_transverse_mercator <double>;
extern template class universal_transverse_mercator <long double>;

} // geospatial
} // titania

#endif
