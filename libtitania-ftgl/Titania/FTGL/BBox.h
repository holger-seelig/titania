/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 * Copyright (c) 2008 Sean Morrison <learner@brlcad.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __TITANIA_FTGL_BBOX_H__
#define __TITANIA_FTGL_BBOX_H__

#include "Point.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

namespace titania {
namespace FTGL {

/**
 * BBox is a convenience class for handling bounding boxes.
 */
class BBox
{
public:

	/**
	 * Default constructor. Bounding box is set to zero.
	 */
	BBox () :
		lower (0, 0, 0),
		upper (0, 0, 0)
	{ }

	/**
	 * Constructor.
	 */
	BBox (double lx, double ly, double lz, double ux, double uy, double uz) :
		lower (lx, ly, lz),
		upper (ux, uy, uz)
	{ }

	/**
	 * Constructor.
	 */
	BBox (Point l, Point u) :
		lower (l),
		upper (u)
	{ }

	/**
	 * Constructor. Extracts a bounding box from a freetype glyph. Uses
	 * the control box for the glyph. <code>FT_Glyph_Get_CBox()</code>
	 *
	 * @param glyph A freetype glyph
	 */
	BBox (FT_GlyphSlot glyph) :
		lower (0, 0, 0),
		upper (0, 0, 0)
	{
		FT_BBox bbox;

		FT_Outline_Get_CBox (&(glyph -> outline), &bbox);

		lower .X (static_cast <double> (bbox.xMin) / 64.0);
		lower .Y (static_cast <double> (bbox.yMin) / 64.0);
		lower .Z (0);
		upper .X (static_cast <double> (bbox.xMax) / 64.0);
		upper .Y (static_cast <double> (bbox.yMax) / 64.0);
		upper .Z (0);
	}

	/**
	 * Destructor
	 */
	~BBox ()
	{ }

	/**
	 * Mark the bounds invalid by setting all lower dimensions greater
	 * than the upper dimensions.
	 */
	void
	Invalidate ()
	{
		lower = Point (1, 1, 1);
		upper = Point (-1, -1, -1);
	}

	/**
	 * Determines if this bounding box is valid.
	 *
	 * @return True if all lower values are <= the corresponding
	 *         upper values.
	 */
	bool
	IsValid ()
	{
		return lower .X () <= upper .X ()
		       && lower .Y () <= upper .Y ()
		       && lower .Z () <= upper .Z ();
	}

	/**
	 * Move the Bounding Box by a vector.
	 *
	 * @param vector  The vector to move the bbox in 3D space.
	 */
	BBox &
	operator += (const Point vector)
	{
		lower += vector;
		upper += vector;

		return *this;
	}

	/**
	 * Combine two bounding boxes. The result is the smallest bounding
	 * box containing the two original boxes.
	 *
	 * @param bbox  The bounding box to merge with the second one.
	 */
	BBox &
	operator |= (const BBox & bbox)
	{
		if (bbox.lower.X () < lower .X ()) lower .X (bbox.lower.X ());

		if (bbox.lower.Y () < lower .Y ()) lower .Y (bbox.lower.Y ());

		if (bbox.lower.Z () < lower .Z ()) lower .Z (bbox.lower.Z ());

		if (bbox.upper.X () > upper .X ()) upper .X (bbox.upper.X ());

		if (bbox.upper.Y () > upper .Y ()) upper .Y (bbox.upper.Y ());

		if (bbox.upper.Z () > upper .Z ()) upper .Z (bbox.upper.Z ());

		return *this;
	}

	void
	SetDepth (double depth)
	{
		if (depth > 0)
			upper .Z (lower.Z () + depth);
		else
			lower .Z (upper.Z () + depth);
	}

	inline Point const
	Upper () const
	{
		return upper;
	}

	inline Point const
	Lower () const
	{
		return lower;
	}

private:

	/**
	 * The bounds of the box
	 */
	Point lower, upper;

};

} // FTGL
} // titania

#endif  //  __FTBBox__
