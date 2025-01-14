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

#include "X3DShapeContext.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Shape/Appearance.h"
#include "../../Components/Shape/PointProperties.h"
#include "../../Components/Shape/FillProperties.h"
#include "../../Components/Shape/LineProperties.h"
#include "../../Components/Texturing/ImageTexture.h"
#include "../../Components/Texturing/TextureProperties.h"

namespace titania {
namespace X3D {

X3DShapeContext::X3DShapeContext () :
	                X3DBaseNode (),
	     defaultPointProperties (new PointProperties (getExecutionContext ())),
	      defaultLineProperties (new LineProperties (getExecutionContext ())),
	      defaultFillProperties (new FillProperties (getExecutionContext ())),
	          defaultAppearance (new Appearance (getExecutionContext ())),
	  lineFillTextureProperties (new TextureProperties (getExecutionContext ())),
	           linetypeTextures (16),
	         hatchStyleTextures (20),
	               linetypeUnit (0),
	             hatchStyleUnit (0)
{
	addChildObjects (defaultPointProperties,
	                 defaultLineProperties,
	                 defaultFillProperties,
	                 defaultAppearance,
	                 lineFillTextureProperties,
	                 linetypeTextures,
	                 hatchStyleTextures);
}

void
X3DShapeContext::initialize ()
{
	defaultLineProperties -> applied () = false;
	defaultFillProperties -> hatched () = false;

	defaultPointProperties      -> setup ();
	defaultLineProperties       -> setup ();
	defaultFillProperties       -> setup ();
	defaultAppearance           -> setup ();
	lineFillTextureProperties   -> setup ();

	if (getBrowser () -> getCombinedTextureUnits () .size ())
	{
		linetypeUnit = getBrowser () -> getCombinedTextureUnits () .top ();
		getBrowser () -> getCombinedTextureUnits () .pop ();
	}

	if (getBrowser () -> getCombinedTextureUnits () .size ())
	{
		hatchStyleUnit = getBrowser () -> getCombinedTextureUnits () .top ();
		getBrowser () -> getCombinedTextureUnits () .pop ();
	}
}

const X3DPtr <X3DTexture2DNode> &
X3DShapeContext::getLinetype (int32_t index) const
{
	if (index < 1 or index > 15)
		index = 1;

	const auto & texture = linetypeTextures [index];

	if (texture)
		return texture;

	const auto linetypeTexture = MakePtr <ImageTexture> (getExecutionContext ());

	linetypeTexture -> url ()               = { get_linetype (index) .str () };
	linetypeTexture -> textureProperties () = lineFillTextureProperties;

	linetypeTexture -> setup ();

	return linetypeTextures [index] = linetypeTexture;
}

const X3DPtr <X3DTexture2DNode> &
X3DShapeContext::getHatchStyle (int32_t index) const
{
	if (index < 1 or index > 19)
		index = 1;

	const auto & texture = hatchStyleTextures [index];

	if (texture)
		return texture;

	const auto hatchStyleTexture = MakePtr <ImageTexture> (getExecutionContext ());

	hatchStyleTexture -> url ()               = { get_hatching (index) .str () };
	hatchStyleTexture -> textureProperties () = lineFillTextureProperties;

	hatchStyleTexture -> setup ();

	return hatchStyleTextures [index] = hatchStyleTexture;
}

X3DShapeContext::~X3DShapeContext ()
{ }

} // X3D
} // titania
