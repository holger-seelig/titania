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

#include "X3DTextGeometry.h"

#include "../../Components/Text/Text.h"
#include "../../Components/Text/X3DFontStyleNode.h"

namespace titania {
namespace X3D {

const Matrix4d X3DTextGeometry::matrix;;

X3DTextGeometry::X3DTextGeometry (Text* const text, const X3DFontStyleNode* const fontStyle) :
	          text (text),
	     fontStyle (fontStyle),
	          bbox (),
	  charSpacings (),
	       bearing (),
	minorAlignment (),
	  translations ()
{ }

void
X3DTextGeometry::initialize ()
{
	const size_t numLines = text -> string () .size ();

	text -> lineBounds () .resize (numLines);
	charSpacings .resize (numLines);
	translations .resize (numLines);

	if (numLines == 0)
	{
		text -> origin ()     = Vector3d ();
		text -> textBounds () = Vector2d ();

		setBBox (Box3d ());
		return;
	}

	if (fontStyle -> horizontal ())
		horizontal ();
	else
		vertical ();
}

X3DBrowser*
X3DTextGeometry::getBrowser () const
{
	return fontStyle -> getBrowser ();
}

void
X3DTextGeometry::horizontal ()
{
	Box2d bbox;

	const auto & fontFace    = fontStyle -> getFontFace ();
	const size_t numLines    = text -> string () .size ();
	const double maxExtent   = std::max <double> (0, text -> maxExtent ());
	const bool   topToBottom = fontStyle -> topToBottom ();
	const double lineHeight  = fontStyle -> getLineHeight ();
	const double scale       = fontStyle -> getScale ();
	const double factor      = lineHeight / fontStyle -> spacing (); // Scale factor of polygon font
	const double descender   = fontFace .getDescender () / double (fontFace .getUnitsPerEm ()) * factor;

	// Calculate bboxes.

	const int32_t first = topToBottom ? 0 : numLines - 1;
	const int32_t last  = topToBottom ? numLines : -1;
	const int32_t step  = topToBottom ? 1 : -1;

	for (int32_t l = first; l not_eq last; l += step)
	{
		const auto & line = text -> string () .get1Value (l);

		// Get line extents.

		Vector2d min, max;
		getHorizontalLineExtents (line, min, max);

		Vector2d size = max - min;

		// Calculate charSpacing and lineBounds.

		const size_t lineNumber = topToBottom ? l : numLines - l - 1;

		double   charSpacing = 0;
		Vector2d lineBound   = Vector2d (size .x (), lineNumber == 0 ? max .y () - descender : lineHeight) * scale;
		double   length      = text -> getLength (l);

		if (maxExtent)
		{
			if (length)
				length = std::min <double> (maxExtent, length);

			else
				length = std::min <double> (maxExtent, size .x () * scale);
		}

		if (length)
		{
			charSpacing = (length - lineBound .x ()) / (line .length () - 1) / scale;
			lineBound .x (length);
			size .x (length / scale);
		}

		charSpacings [l]          = charSpacing;
		text -> lineBounds () [l] = lineBound;

		// Calculate line translation.

		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				translations [l] = Vector2d (0, -(lineNumber * lineHeight));
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				translations [l] = Vector2d (-min .x () - size .x () / 2, -(lineNumber * lineHeight));
				break;
			case X3DFontStyleNode::Alignment::END:
				translations [l] = Vector2d (-min .x () - size .x (), -(lineNumber * lineHeight));
				break;
		}

		// Calculate center.

		const Vector2d center = min + size / 2.0;

		// Add bbox.

		bbox += Box2d (size * scale, (center + translations [l]) * scale);
	}

	// Get text extents.

	auto       extents = bbox .extents ();
	auto &     min     = extents .first;
	auto &     max     = extents .second;
	const auto size    = max - min;

	// Calculate text position

	text -> textBounds () = size;
	bearing               = Vector2d (0, -max .y ());

	switch (fontStyle -> getMinorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
			minorAlignment = bearing;
			break;
		case X3DFontStyleNode::Alignment::FIRST:
			minorAlignment = Vector2d ();
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			minorAlignment = Vector2d (0, size .y () / 2 - max .y ());
			break;
		case X3DFontStyleNode::Alignment::END:
			minorAlignment = Vector2d (0, (numLines - 1) * lineHeight * scale);
			break;
	}

	// Translate bbox by minorAlignment.

	min += minorAlignment;
	max += minorAlignment;

	// The value of the origin field represents the upper left corner of the textBounds.

	text -> origin () = Vector3d (min .x (), max .y (), 0);

	setBBox (Box3d (Vector3d (min .x (), min .y (), 0),
	                Vector3d (max .x (), max .y (), 0),
	                extents_type ()));
}

void
X3DTextGeometry::vertical ()
{
	size_t numChars = 0;

	for (const auto & line : std::pair (text -> string () .cbegin (), text -> string () .cend ()))
		numChars += line .length ();

	charSpacings .resize (numChars);
	translations .resize (numChars);

	std::vector <double> yPad (numChars);

	//

	Box2d bbox;

	const size_t numLines    = text -> string () .size ();
	const double maxExtent   = std::max <double> (0, text -> maxExtent ());
	const bool   leftToRight = fontStyle -> leftToRight ();
	const bool   topToBottom = fontStyle -> topToBottom ();
	const double spacing     = fontStyle -> getLineHeight ();
	const double scale       = fontStyle -> getScale ();
	const double height      = spacing / fontStyle -> spacing ();

	// Calculate bboxes.

	const int32_t first = leftToRight ? 0 : numLines - 1;
	const int32_t last  = leftToRight ? numLines : -1;
	const int32_t step  = leftToRight ? 1 : -1;
	size_t        t     = 0; // Translation index

	for (int32_t l = first; l not_eq last; l += step)
	{
		const auto & line = text -> string () .get1Value (l);
		
		const int32_t t0       = t;
		const size_t  numChars = line .length ();

		// Calculate line bbox

		Box2d lineBBox;

		const int32_t first = topToBottom ? 0 : numChars - 1;
		const int32_t last  = topToBottom ? numChars : -1;
		const int32_t step  = topToBottom ? 1 : -1;

		for (int32_t g = first; g not_eq last; g += step)
		{
			const auto & glyph = line [g];

			// Get glyph extents.

			Vector2d min, max;
			getGlyphExtents (glyph, min, max);

			const auto size = max - min;
			
			// Calculate glyph translation
			
			const int32_t glyphNumber = topToBottom ? g : numChars - g - 1;

			translations [t] = Vector2d ((spacing - size .x () - min .x ()) / 2, -glyphNumber * height);
			
			// Calculate center.

			const auto center = min + size / 2.0 + translations [t];

			// Add bbox.

			lineBBox += Box2d (size, center);

			// Incement translation index

			++ t;
		}
	
		// Get line extents.

		auto   extents = lineBBox .extents ();
		auto & min     = extents .first;
		auto & max     = extents .second;
		auto   size    = max - min;

		// Calculate charSpacing and lineBounds.

		const size_t lineNumber = leftToRight ? l : numLines - l - 1;
		const double padding    = (spacing - size .x ()) / 2;

		double charSpacing = 0;
		auto   lineBound   = Vector2d (l == 0 ? spacing - padding : spacing, line .empty () ? 0 : size .y ()) * scale;
		double length      = text -> getLength (l);

		if (maxExtent)
		{
			if (length)
				length = std::min <double> (maxExtent, length);

			else
				length = std::min <double> (maxExtent, size .y () * scale);
		}

		if (length)
		{
			charSpacing = (length - lineBound .y ()) / (line .length () - 1) / scale;
			lineBound .y (length);
			size .y (length / scale);
			min .y (max .y () - size .y ());
		}

		text -> lineBounds () [l] = lineBound;

		// Calculate line translation.
		
		Vector2d translation;

		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				translation = Vector2d (lineNumber * spacing, -height);
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				translation = Vector2d (lineNumber * spacing, size .y () / 2 - max .y ());
				break;
			case X3DFontStyleNode::Alignment::END:
			{
				// This is needed to make maxExtend and charSpacing work.
				Vector2d glyphMin, v;

				if (not line .empty ())
					getGlyphExtents (line [topToBottom ? numChars - 1 : 0], glyphMin, v);

				translation = Vector2d (lineNumber * spacing, size .y () - max .y () + glyphMin .y ());
				break;
			}
		}

		// Calculate glyph translation		

		double space = 0;

		for (auto & glyph : std::pair (translations .begin () + t0, translations .begin () + t))
		{
			glyph += translation;

			glyph .y (glyph .y () - space);

			space += charSpacing;
		}

		// Calculate center.

		const Vector2d center = min + size / 2.0;

		// Add bbox.
			
		switch (fontStyle -> getMajorAlignment ())
		{
			case X3DFontStyleNode::Alignment::BEGIN:
			case X3DFontStyleNode::Alignment::FIRST:
				yPad [l] = max .y () + translation .y ();
				break;
			case X3DFontStyleNode::Alignment::MIDDLE:
				break;
			case X3DFontStyleNode::Alignment::END:
				yPad [l] = min .y () + translation .y ();
				break;
		}

		bbox += Box2d (size * scale, (center + translation) * scale);
	}

	// Get text extents.

	auto       extents = bbox .extents ();
	auto &     min     = extents .first;
	auto &     max     = extents .second;
	const auto size    = max - min;

	// Extend lineBounds.
	
	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
		{
			size_t l = 0;
			
			for (MFVec2f::reference lineBound : text -> lineBounds ())
			{
				auto lb = lineBound .get ();

				lb .y (lb .y () + max .y () - yPad [l ++] * scale);

				lineBound .set (lb);
			}

			break;
		}
		case X3DFontStyleNode::Alignment::MIDDLE:
			break;
		case X3DFontStyleNode::Alignment::END:
		{
			size_t l = 0;
			
			for (MFVec2f::reference lineBound : text -> lineBounds ())
			{
				auto lb = lineBound .get ();

				lb .y (lb .y () + yPad [l ++] * scale - min .y ());

				lineBound .set (lb);
			}

			break;
		}
	}

	// Calculate text position

	text -> textBounds () = size;

	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			bearing = Vector2d (-min .x (), max .y ());
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			bearing = Vector2d (-min .x (), 0);
			break;
		case X3DFontStyleNode::Alignment::END:
			bearing = Vector2d (-min .x (), min .y ());
			break;
	}

	switch (fontStyle -> getMinorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			minorAlignment = Vector2d (-min .x (), 0);
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
			minorAlignment = Vector2d (-min .x () - size .x () / 2, 0);
			break;
		case X3DFontStyleNode::Alignment::END:
			minorAlignment = Vector2d (-min .x () - size .x (), 0);
			break;
	}

	// Translate bbox by minorAlignment.

	min += minorAlignment;
	max += minorAlignment;
	
	// The value of the origin field represents the upper left corner of the textBounds.

	text -> origin () = Vector3d (min .x (), max .y (), 0);

	setBBox (Box3d (Vector3d (min .x (), min .y (), 0),
	                Vector3d (max .x (), max .y (), 0),
	                extents_type ()));
}

void
X3DTextGeometry::getHorizontalLineExtents (const String & line, Vector2d & min, Vector2d & max) const
{
	const bool normal = fontStyle -> horizontal () ? fontStyle -> leftToRight () : fontStyle -> topToBottom ();

	if (not line .empty ())
		getLineExtents (normal ? line : String (line .rbegin (), line .rend ()), min, max);

	switch (fontStyle -> getMajorAlignment ())
	{
		case X3DFontStyleNode::Alignment::BEGIN:
		case X3DFontStyleNode::Alignment::FIRST:
			min .x (0);
			break;
		case X3DFontStyleNode::Alignment::MIDDLE:
		case X3DFontStyleNode::Alignment::END:
			break;
	}
}

void
X3DTextGeometry::getGlyphExtents (const String::value_type & glyph, Vector2d & min, Vector2d & max) const
{
	getLineExtents (String (1, glyph), min, max);
}

void
X3DTextGeometry::draw (ShapeContainer* const context)
{ }

X3DTextGeometry::~X3DTextGeometry ()
{ }

} // X3D
} // titania
