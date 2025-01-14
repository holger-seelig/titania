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

#include "ComposedCubeMapTexture.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

const Component   ComposedCubeMapTexture::component      = Component ("CubeMapTexturing", 1);
const std::string ComposedCubeMapTexture::typeName       = "ComposedCubeMapTexture";
const std::string ComposedCubeMapTexture::containerField = "texture";

ComposedCubeMapTexture::Fields::Fields () :
	 frontTexture (new SFNode ()),
	  backTexture (new SFNode ()),
	  leftTexture (new SFNode ()),
	 rightTexture (new SFNode ()),
	   topTexture (new SFNode ()),
	bottomTexture (new SFNode ())
{ }

ComposedCubeMapTexture::ComposedCubeMapTexture (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentTextureNode (),
	                   fields (),
	                    nodes (6),
	                    width (0),
                      height (0),
	               components (0),
	                loadState (NOT_STARTED_STATE)
{
	addType (X3DConstants::ComposedCubeMapTexture);

	addField (inputOutput, "metadata",      metadata ());
	addField (inputOutput, "frontTexture",  frontTexture ());
	addField (inputOutput, "backTexture",   backTexture ());
	addField (inputOutput, "leftTexture",   leftTexture ());
	addField (inputOutput, "rightTexture",  rightTexture ());
	addField (inputOutput, "topTexture",    topTexture ());
	addField (inputOutput, "bottomTexture", bottomTexture ());

	addField (X3D_V3_3, "front",  "frontTexture");
	addField (X3D_V3_3, "back",   "backTexture");
	addField (X3D_V3_3, "left",   "leftTexture");
	addField (X3D_V3_3, "right",  "rightTexture");
	addField (X3D_V3_3, "top",    "topTexture");
	addField (X3D_V3_3, "bottom", "bottomTexture");

	addField (X3D_V3_2, "front",  "frontTexture");
	addField (X3D_V3_2, "back",   "backTexture");
	addField (X3D_V3_2, "left",   "leftTexture");
	addField (X3D_V3_2, "right",  "rightTexture");
	addField (X3D_V3_2, "top",    "topTexture");
	addField (X3D_V3_2, "bottom", "bottomTexture");

	addField (X3D_V3_1, "front",  "frontTexture");
	addField (X3D_V3_1, "back",   "backTexture");
	addField (X3D_V3_1, "left",   "leftTexture");
	addField (X3D_V3_1, "right",  "rightTexture");
	addField (X3D_V3_1, "top",    "topTexture");
	addField (X3D_V3_1, "bottom", "bottomTexture");

	addField (X3D_V3_0, "front",  "frontTexture");
	addField (X3D_V3_0, "back",   "backTexture");
	addField (X3D_V3_0, "left",   "leftTexture");
	addField (X3D_V3_0, "right",  "rightTexture");
	addField (X3D_V3_0, "top",    "topTexture");
	addField (X3D_V3_0, "bottom", "bottomTexture");

	addChildObjects (textureProperties (), nodes, loadState);
}

X3DBaseNode*
ComposedCubeMapTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedCubeMapTexture (executionContext);
}

void
ComposedCubeMapTexture::initialize ()
{
	X3DEnvironmentTextureNode::initialize ();

	if (getBrowser () -> getLoadUrlObjects ())
	{
		frontTexture ()  .addInterest (&ComposedCubeMapTexture::set_texture, this, std::cref (frontTexture ()),  0);
		backTexture ()   .addInterest (&ComposedCubeMapTexture::set_texture, this, std::cref (backTexture ()),   1);
		leftTexture ()   .addInterest (&ComposedCubeMapTexture::set_texture, this, std::cref (leftTexture ()),   2);
		rightTexture ()  .addInterest (&ComposedCubeMapTexture::set_texture, this, std::cref (rightTexture ()),  3);
		topTexture ()    .addInterest (&ComposedCubeMapTexture::set_texture, this, std::cref (topTexture ()),    4);
		bottomTexture () .addInterest (&ComposedCubeMapTexture::set_texture, this, std::cref (bottomTexture ()), 5);

		set_texture (frontTexture (),  0);
		set_texture (backTexture (),   1);
		set_texture (leftTexture (),   2);
		set_texture (rightTexture (),  3);
		set_texture (topTexture (),    4);
		set_texture (bottomTexture (), 5);
	}
}

void
ComposedCubeMapTexture::set_texture (const SFNode & node, const size_t index)
{
	if (nodes [index])
		nodes [index] -> removeInterest (&ComposedCubeMapTexture::set_texture, this);

	nodes [index] = node;

	if (nodes [index])
		nodes [index] -> addInterest (&ComposedCubeMapTexture::set_texture, this, std::cref (node), index);

	setTexture (getTargets () [index], node);
}

void
ComposedCubeMapTexture::setTexture (const GLenum target, const SFNode & node)
{
	const auto texture = x3d_cast <X3DTexture2DNode*> (node);

	if (texture)
	{
		// Get transparent and components

		bool transparent = false;
		components = 0;

		for (const auto & node : nodes)
		{
			const auto texture = x3d_cast <X3DTexture2DNode*> (node);

			if (texture)
			{
				transparent = transparent or texture -> isTransparent ();
				components  = std::max <int32_t> (components, texture -> components ());
			}
		}

		setTransparent (transparent);

		// Get texture 2d data as four component RGBA

		width  = texture -> getWidth ();
		height = texture -> getHeight ();

		std::vector <uint8_t> image (width * height * 4);

		glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());
		glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
		glBindTexture (GL_TEXTURE_2D, 0);

		// Flip image vertically

		const size_t width4   = width * 4;
		const size_t height_1 = height - 1;

		for (size_t r = 0, height1_2 = height / 2; r < height1_2; ++ r)
		{
			for (size_t c = 0; c < width4; ++ c)
			{
				std::swap (image [r * width4 + c], image [(height_1 - r) * width4 + c]);
			}
		}

		// Transfer image

		if (width == height)
		{
			setImage (target, GL_RGBA, GL_RGBA, image .data ());
			setLoadState (COMPLETE_STATE);
			return;
		}

		getBrowser () -> getConsole () -> error ("Error: ComposedCubeMapTexture: width and height must be equal, and all images must be of the same size!\n");
	}

	width      = 0;
	height     = 0;
	components = 0;

	setImage (target, GL_RGBA, GL_RGBA, nullptr);
	setLoadState (FAILED_STATE);
}

void
ComposedCubeMapTexture::dispose ()
{
	removeChildObjects (textureProperties ());

	X3DEnvironmentTextureNode::dispose ();
}

} // X3D
} // titania
