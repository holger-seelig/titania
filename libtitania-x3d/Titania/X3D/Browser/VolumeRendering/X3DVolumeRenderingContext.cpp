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

#include "X3DVolumeRenderingContext.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Shaders/ComposedShader.h"
#include "../../Components/Texturing/PixelTexture.h"
#include "../../Components/Texturing/TextureProperties.h"
#include "../../Components/VolumeRendering/OpacityMapVolumeStyle.h"

namespace titania {
namespace X3D {

X3DVolumeRenderingContext::X3DVolumeRenderingContext () :
	                   X3DBaseNode (),
	        defaultVolumeStyleNode (),
	 defaultBlendedVolumeStyleNode (),
	       defaultTransferFunction ()
{
	addChildObjects (defaultVolumeStyleNode,
	                 defaultBlendedVolumeStyleNode,
	                 defaultTransferFunction);
}

void
X3DVolumeRenderingContext::initialize ()
{ }

const X3DPtr <OpacityMapVolumeStyle> &
X3DVolumeRenderingContext::getDefaultVolumeStyle () const
{
	if (defaultVolumeStyleNode)
		return defaultVolumeStyleNode;

	defaultVolumeStyleNode = MakePtr <OpacityMapVolumeStyle> (getExecutionContext ());

	defaultVolumeStyleNode -> setup ();

	return defaultVolumeStyleNode;
}

const X3DPtr <OpacityMapVolumeStyle> &
X3DVolumeRenderingContext::getDefaultBlendedVolumeStyle () const
{
	if (defaultBlendedVolumeStyleNode)
		return defaultBlendedVolumeStyleNode;

	defaultBlendedVolumeStyleNode = MakePtr <OpacityMapVolumeStyle> (getExecutionContext ());

	defaultBlendedVolumeStyleNode -> setup ();

	return defaultBlendedVolumeStyleNode;
}

const X3DPtr <PixelTexture> &
X3DVolumeRenderingContext::getDefaultTransferFunction () const
{
	if (defaultTransferFunction)
		return defaultTransferFunction;

	defaultTransferFunction = MakePtr <PixelTexture> (getExecutionContext ());

	const auto textureProperties = MakePtr <TextureProperties> (getExecutionContext ());

	textureProperties -> generateMipMaps () = true;
	textureProperties -> boundaryModeS ()   = "CLAMP_TO_EDGE";
	textureProperties -> boundaryModeT ()   = "REPEAT";

	defaultTransferFunction -> textureProperties () = textureProperties;

	defaultTransferFunction -> image () .setWidth (256);
	defaultTransferFunction -> image () .setHeight (1);
	defaultTransferFunction -> image () .setComponents (2);

	for (size_t i = 0; i < 256; ++ i)
		defaultTransferFunction -> image () .getArray () [i] = (i << 8) | i;

	textureProperties       -> setup ();
	defaultTransferFunction -> setup ();

	return defaultTransferFunction;
}

X3DVolumeRenderingContext::~X3DVolumeRenderingContext ()
{ }

} // X3D
} // titania
