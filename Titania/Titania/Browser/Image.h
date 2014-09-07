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

#include <Magick++.h>

namespace titania {

class Image
{
public:

	///  @name Member types

	typedef unsigned char value_type;
	typedef size_t        size_type;

	///  @name Construction

	Image (const std::string &);

	///  @name Member access

	size_type
	getWidth () const
	{ return image .size () .width (); }

	size_type
	getHeight () const
	{ return image .size () .height (); }

	size_type
	getComponents () const
	{ return components; }

	bool
	getTransparency () const
	{ return transparency; }

	const Image::value_type*
	getData () const
	{ return static_cast <const value_type*> (blob .data ()); }

	///  @name Destructions

	~Image ();


private:

	///  @name Operations

	void
	getBlob ();
	
	///  @name Members

	Magick::Image image;
	Magick::Blob  blob;

	size_type components;
	bool      transparency;

};

inline
Image::Image (const std::string & data) :
	       image (),
	        blob (),
	  components (0),
	transparency (false)
{
	image .backgroundColor (Magick::Color (0, 0, 0, uint16_t (-1)));
	image .read (Magick::Blob (data .c_str (), data .length ()));

	getBlob ();

	//	std::list <Magick::Image> images;
	//	Magick::readImages (&images, Magick::Blob (data .c_str (), data .length ()));
	//
	//	switch (images .size ())
	//	{
	//		case 0:
	//			break;
	//
	//		case 1:  // Image with one layer image.
	//			image = images .back ();
	//			break;
	//
	//		default: // Flatten image with more than one layer.
	//			Magick::flattenImages (&image, images .begin (), images .end ());
	//			break;
	//	}
	//
	//	getBlob ();
}

inline
void
Image::getBlob ()
{
	std::string magick;

	switch (image .type ())
	{
		case Magick::GrayscaleType:
		{
			if (image .matte ())
			{
				components   = 4;
				transparency = true,
				magick       = "RGBA";
			}
			else
			{
				components   = 3;
				transparency = false,
				magick       = "RGB";
			}

			break;
		}
		case Magick::GrayscaleMatteType:
		{
			components   = 4;
			transparency = true,
			magick       = "RGBA";
			break;
		}
		case Magick::TrueColorType:
		{
			if (image .matte ())
			{
				components   = 4;
				transparency = true,
				magick       = "RGBA";
			}
			else
			{
				components   = 3;
				transparency = false,
				magick       = "RGB";
			}

			break;
		}
		case Magick::TrueColorMatteType:
		{
			components   = 4;
			transparency = true,
			magick       = "RGBA";
			break;
		}
		default:
		{
			if (image .matte ())
			{
				components   = 4;
				transparency = true,
				magick       = "RGBA";
			}
			else
			{
				components   = 3;
				transparency = false,
				magick       = "RGB";
			}

			break;
		}
	}

	image .depth (8);
	image .magick (magick);
	image .write (&blob);
}

inline
Image::~Image ()
{ }

} // titania
