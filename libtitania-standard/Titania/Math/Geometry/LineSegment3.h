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

#ifndef __TITANIA_MATH_GEOMETRY_LINE_SEGMENT3_H__
#define __TITANIA_MATH_GEOMETRY_LINE_SEGMENT3_H__

#include "../Geometry/Line3.h"
#include "../Numbers/Vector3.h"

namespace titania {
namespace math {

/**
 *  Template to represent a line segment.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of the two point values.
 */
template <class Type>
class line_segment3
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Construction

	///  Default constructor. Constructs a line segement with length 0 on point (0, 0, 0).
	constexpr
	line_segment3 () :
		m_point1 (),
		m_point2 (),
		  m_line ()
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	line_segment3 (const line_segment3 <Up> & other) :
		m_point1 (other .point1 ()),
		m_point2 (other .point2 ()),
		  m_line (other .line ())
	{ }

	///  Constructs a line segment from @a point1 and @a point2.
	line_segment3 (const vector3 <Type> & point1, const vector3 <Type> & point2) :
		m_point1 (point1),
		m_point2 (point2),
		  m_line (point1, point2, points_type ())
	{ }

	///  @name Member access

	const vector3 <Type> &
	point1 () const
	{ return m_point1; }

	const vector3 <Type> &
	point2 () const
	{ return m_point2; }

	const line3 <Type> &
	line () const
	{ return m_line; }

	///  @name Operations

	bool
	is_between (const vector3 <Type> & point) const;

	Type
	distance (const vector3 <Type> & point) const;


private:

	vector3 <Type> m_point1;
	vector3 <Type> m_point2;
	line3 <Type>   m_line;

};

///  Returns true if @a point lies between point2 and point2.
template <class Type>
bool
line_segment3 <Type>::is_between (const vector3 <Type> & point) const
{
	const auto closest = line () .closest_point (point);
	return abs ((closest - point1 ()) + (closest - point2 ())) <= abs (point1 () - point2 ());
}

///  Returns the distance to @a point.
template <class Type>
Type
line_segment3 <Type>::distance (const vector3 <Type> & point) const
{
	const auto closest = line () .closest_point (point);
	const auto between = abs ((closest - point1 ()) + (closest - point2 ())) <= abs (point1 () - point2 ());

	// Closest point lies between point1 and point2.
	if (between)
		return abs (point - closest);

	const auto distance0 = abs (point - point1 ());
	const auto distance1 = abs (point - point2 ());

	return std::min (distance0, distance1);
}

///  @relates line_segment3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, line_segment3 <Type> & segment)
{
	vector3 <Type> point1, point2;

	istream >> point1 >> point2;

	if (istream)
		segment = line_segment3 <Type> (point1, point2);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const line_segment3 <Type> & segment)
{
	return ostream << segment .point1 () << "  " << segment .point2 ();
}

extern template class line_segment3 <float>;
extern template class line_segment3 <double>;
extern template class line_segment3 <long double>;

//
extern template std::istream & operator >> (std::istream &, line_segment3 <float> &);
extern template std::istream & operator >> (std::istream &, line_segment3 <double> &);
extern template std::istream & operator >> (std::istream &, line_segment3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const line_segment3 <float> &);
extern template std::ostream & operator << (std::ostream &, const line_segment3 <double> &);
extern template std::ostream & operator << (std::ostream &, const line_segment3 <long double> &);

} // math
} // titania

#endif
