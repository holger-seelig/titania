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

#ifndef __TITANIA_MATH_ALGORITHMS_CATMULL_ROM_SPLINE_H__
#define __TITANIA_MATH_ALGORITHMS_CATMULL_ROM_SPLINE_H__

#include "../Functional.h"
#include "../Numbers/Matrix4.h"
#include "../Numbers/Vector2.h"
#include "../Numbers/Vector3.h"
#include "../Numbers/Vector4.h"

#include <cmath>
#include <vector>

namespace titania {
namespace math {

template <class Type, class Scalar>
class catmull_rom_spline
{
public:

	catmull_rom_spline ()
	{ }

	template <class Key, class KeyValue, class KeyVelocity>
	void
	generate (bool, const Key &, const KeyValue, const KeyVelocity &, bool);

	template <class KeyValue>
	Type
	evaluate (size_t, size_t, const Scalar &, const KeyValue &);


private:

	std::vector <Type> T0;
	std::vector <Type> T1;

};

template <class Type, class Scalar>
template <class Key, class KeyValue, class KeyVelocity>
void
catmull_rom_spline <Type, Scalar>::generate (bool closed, const Key & key, const KeyValue keyValue, const KeyVelocity & keyVelocity, bool normalizeVelocity)
{
	std::vector <Type>   T;
	std::vector <Scalar> Fp, Fm;

	T  .reserve (key .size ());
	T0 .reserve (key .size ());
	T1 .reserve (key .size ());
	Fp .reserve (key .size ());
	Fm .reserve (key .size ());

	// T

	if (keyVelocity .empty ())
	{
		if (closed)
			T .emplace_back ((keyValue [1] - keyValue [keyValue .size () - 2]) / 2.0f);

		else
			T .emplace_back ();

		for (size_t i = 1, size = keyValue .size () - 1; i < size; ++ i)
			T .emplace_back ((keyValue [i + 1] - keyValue [i - 1]) / 2.0f);

		T .emplace_back (T . front ());
	}
	else
	{
		T .assign (keyVelocity .begin (), keyVelocity .end ());

		if (normalizeVelocity)
		{
			Scalar Dtot = 0;

			for (size_t i = 0, size = keyValue .size () - 1; i < size; ++ i)
				Dtot += math::abs (keyValue [i] - keyValue [i + 1]);

			for (auto & Ti : T)
				Ti *= Dtot / math::abs (Ti);
		}
	}

	// Fm, Fp

	if (closed)
	{
		size_t i_1 = key .size () - 1;
		size_t i_2 = key .size () - 2;

		Scalar d = key [1] - key [0] + key [i_1] - key [i_2];

		Fm .emplace_back (2 * (key [1] - key [0]) / d);
		Fp .emplace_back (2 * (key [i_1] - key [i_2]) / d);
	}
	else
	{
		Fm .emplace_back (1);
		Fp .emplace_back (1);
	}

	for (size_t i = 1, size = key .size () - 1; i < size; ++ i)
	{
		Scalar d = key [i + 1] - key [i - 1];

		Fm .emplace_back (2 * (key [i + 1] - key [i]) / d);
		Fp .emplace_back (2 * (key [i] - key [i - 1]) / d);
	}

	Fm .emplace_back (Fm .front ());
	Fp .emplace_back (Fp .front ());

	// T0, T1

	for (size_t i = 0, size = T .size (); i < size; ++ i)
	{
		T0 .emplace_back (Fp [i] * T [i]);
		T1 .emplace_back (Fm [i] * T [i]);
	}

	T0 .shrink_to_fit ();
	T1 .shrink_to_fit ();
}

template <class Type, class Scalar>
template <class KeyValue>
Type
catmull_rom_spline <Type, Scalar>::evaluate (size_t index0, size_t index1, const Scalar & weight, const KeyValue & keyValue)
{
	vector4 <Scalar> S (std::pow (weight, 3), math::sqr (weight), weight, 1);

	static matrix4 <Scalar> H (2, -2,  1,  1,
	                           -3,  3, -2, -1,
	                           0,  0,  1,  0,
	                           1,  0,  0,  0);

	const Type & C0 = keyValue [index0];
	const Type & C1 = keyValue [index1];
	const Type & C2 = T0 [index0];
	const Type & C3 = T1 [index1];
	
	// vs = S^T H C

	vector4 <Scalar> SH = S * H;
	
	// Taking dot product from SH and C;

	return SH [0] * C0 + SH [1] * C1 + SH [2] * C2 + SH [3] * C3;
}

extern template class catmull_rom_spline <float, float>;
extern template class catmull_rom_spline <double, double>;
extern template class catmull_rom_spline <vector2 <float>, float>;
extern template class catmull_rom_spline <vector2 <double>, double>;
extern template class catmull_rom_spline <vector3 <float>, float>;
extern template class catmull_rom_spline <vector3 <double>, double>;
extern template class catmull_rom_spline <vector4 <float>, float>;
extern template class catmull_rom_spline <vector4 <double>, double>;

} // math
} // titania

#endif
