/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
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

#include "Face.h"

#include "Library.h"

#include FT_TRUETYPE_TABLES_H

namespace titania {
namespace FTGL {

Face::Face (const char* fontFilePath, bool precomputeKerning) :
	       numGlyphs (0),
	fontEncodingList (0),
	    kerningCache (0),
	             err (0)
{
	const FT_Long DEFAULT_FACE_INDEX = 0;

	ftFace = new FT_Face;

	err = FT_New_Face (*Library::getInstance () .getLibrary (), fontFilePath,
	                   DEFAULT_FACE_INDEX, ftFace);

	if (err)
	{
		delete ftFace;
		ftFace = 0;
		return;
	}

	numGlyphs       = (*ftFace) -> num_glyphs;
	hasKerningTable = (FT_HAS_KERNING ((*ftFace)) not_eq 0);

	if (hasKerningTable && precomputeKerning)
	{
		buildKerningCache ();
	}
}

Face::Face (const unsigned char* pBufferBytes, size_t bufferSizeInBytes,
            bool precomputeKerning) :
	       numGlyphs (0),
	fontEncodingList (0),
	    kerningCache (0),
	             err (0)
{
	const FT_Long DEFAULT_FACE_INDEX = 0;

	ftFace = new FT_Face;

	err = FT_New_Memory_Face (*Library::getInstance () .getLibrary (),
	                          (FT_Byte const*) pBufferBytes, (FT_Long) bufferSizeInBytes,
	                          DEFAULT_FACE_INDEX, ftFace);

	if (err)
	{
		delete ftFace;
		ftFace = 0;
		return;
	}

	numGlyphs       = (*ftFace) -> num_glyphs;
	hasKerningTable = (FT_HAS_KERNING ((*ftFace)) not_eq 0);

	if (hasKerningTable && precomputeKerning)
	{
		buildKerningCache ();
	}
}

Face::~Face ()
{
	if (kerningCache)
	{
		delete [ ] kerningCache;
	}

	if (ftFace)
	{
		FT_Done_Face (*ftFace);
		delete ftFace;
		ftFace = 0;
	}
}

bool
Face::attach (const char* fontFilePath)
{
	err = FT_Attach_File (*ftFace, fontFilePath);
	return ! err;
}

bool
Face::attach (const unsigned char* pBufferBytes, size_t bufferSizeInBytes)
{
	FT_Open_Args open;

	open .flags       = FT_OPEN_MEMORY;
	open .memory_base = (FT_Byte const*) pBufferBytes;
	open .memory_size = (FT_Long) bufferSizeInBytes;

	err = FT_Attach_Stream (*ftFace, &open);
	return ! err;
}

const Size &
Face::getSize (const unsigned int size, const unsigned int res)
{
	charSize .setCharSize (ftFace, size, res, res);
	err = charSize .getError ();

	return charSize;
}

unsigned int
Face::getCharMapCount () const
{
	return (*ftFace) -> num_charmaps;
}

FT_Encoding*
Face::getCharMapList ()
{
	if (0 == fontEncodingList)
	{
		fontEncodingList = new FT_Encoding [getCharMapCount ()];

		for (size_t i = 0; i < getCharMapCount (); ++ i)
		{
			fontEncodingList [i] = (*ftFace) -> charmaps [i] -> encoding;
		}
	}

	return fontEncodingList;
}

Point
Face::getKernAdvance (unsigned int index1, unsigned int index2)
{
	double x, y;

	if (! hasKerningTable or ! index1 or ! index2)
	{
		return Point (0, 0);
	}

	if (kerningCache && index1 < Face::MAX_PRECOMPUTED
	    && index2 < Face::MAX_PRECOMPUTED)
	{
		x = kerningCache [2 * (index2 * Face::MAX_PRECOMPUTED + index1)];
		y = kerningCache [2 * (index2 * Face::MAX_PRECOMPUTED + index1) + 1];
		return Point (x, y);
	}

	FT_Vector kernAdvance;
	kernAdvance .x = kernAdvance .y = 0;

	err = FT_Get_Kerning (*ftFace, index1, index2, ft_kerning_unfitted,
	                      &kernAdvance);

	if (err)
	{
		return Point (0, 0);
	}

	x = static_cast <double> (kernAdvance.x) / 64.0;
	y = static_cast <double> (kernAdvance.y) / 64.0;

	return Point (x, y);
}

FT_GlyphSlot
Face::getGlyph (unsigned int index, FT_Int load_flags)
{
	err = FT_Load_Glyph (*ftFace, index, load_flags);

	if (err)
	{
		return NULL;
	}

	return (*ftFace) -> glyph;
}

void
Face::buildKerningCache ()
{
	FT_Vector kernAdvance;

	kernAdvance .x = 0;
	kernAdvance .y = 0;
	kerningCache   = new double [Face::MAX_PRECOMPUTED
	                             * Face::MAX_PRECOMPUTED * 2];

	for (unsigned int j = 0; j < Face::MAX_PRECOMPUTED; j ++)
	{
		for (unsigned int i = 0; i < Face::MAX_PRECOMPUTED; i ++)
		{
			err = FT_Get_Kerning (*ftFace, i, j, ft_kerning_unfitted,
			                      &kernAdvance);

			if (err)
			{
				delete [ ] kerningCache;
				kerningCache = NULL;
				return;
			}

			kerningCache [2 * (j * Face::MAX_PRECOMPUTED + i)]
			   = static_cast <double> (kernAdvance.x) / 64.0;
			kerningCache [2 * (j * Face::MAX_PRECOMPUTED + i) + 1]
			   = static_cast <double> (kernAdvance.y) / 64.0;
		}
	}
}

} // FTGL
} // titania
