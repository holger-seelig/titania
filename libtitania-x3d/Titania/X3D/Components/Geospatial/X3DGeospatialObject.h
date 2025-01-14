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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_X3D_COMPONENTS_GEOSPATIAL_X3DGEOSPATIAL_OBJECT_H__
#define __TITANIA_X3D_COMPONENTS_GEOSPATIAL_X3DGEOSPATIAL_OBJECT_H__

#include "../../Browser/Geospatial/Geospatial.h"
#include "../Geospatial/GeoOrigin.h"

namespace titania {
namespace X3D {

class X3DScene;

class X3DGeospatialObject :
	virtual public X3DBaseNode
{
public:

	///  @name Fields

	virtual
	MFString &
	geoSystem ()
	{ return *fields .geoSystem; }

	virtual
	const MFString &
	geoSystem () const
	{ return *fields .geoSystem; }

	virtual
	SFNode &
	geoOrigin ()
	{ return *fields .geoOrigin; }

	virtual
	const SFNode &
	geoOrigin () const
	{ return *fields .geoOrigin; }


protected:

	///  @name Construction

	X3DGeospatialObject ();

	virtual
	void
	initialize () override;

	///  @name Operations

	Vector3d
	getCoord (const Vector3d &) const;

	Vector3d
	getGeoCoord (const Vector3d &) const;

	double
	getElevation (const Vector3d &) const;

	Vector3d
	getUpVector (const Vector3d &) const;

	Matrix4d
	getLocationMatrix (const Vector3d &) const;

	bool
	isStandardOrder () const
	{ return standardOrder; }

//	Vector3d
//	lerp (const Vector3d &, const Vector3d &, const double);


private:

	///  @name Event handlers

	void
	set_geoSystem ();

	void
	set_geoOrigin ();

	void
	set_origin ();

	void
	set_originMatrix ();

	void
	set_rotateYUp ();

	Matrix4d
	getStandardLocationMatrix (const Vector3d &) const;

	///  @name Member access

	Geospatial::CoordinateSystemType
	getCoordinateSystem () const
	{ return coordinateSystem; }

	///  @name Members

	struct Fields
	{
		Fields ();

		MFString* const geoSystem;
		SFNode* const geoOrigin;
	};

	Fields fields;

	Geospatial::CoordinateSystemType coordinateSystem;
	Geospatial::ReferenceFramePtr    referenceFrame;
	Geospatial::ElevationFramePtr    elevationFrame;
	bool                             standardOrder;
	X3DPtr <GeoOrigin>               geoOriginNode;
	Vector3d                         origin;
	Matrix4d                         originMatrix;
	Matrix4d                         invOriginMatrix;
	bool                             rotateYUp;
	bool                             radians;

};

} // X3D
} // titania

#endif
