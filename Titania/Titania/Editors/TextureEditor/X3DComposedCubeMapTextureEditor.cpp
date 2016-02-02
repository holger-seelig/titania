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

#include "X3DComposedCubeMapTextureEditor.h"

#include "../../ComposedWidgets/TexturePreview.h"

namespace titania {
namespace puck {

X3DComposedCubeMapTextureEditor::X3DComposedCubeMapTextureEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	             frontPreview (new TexturePreview (this, getComposedCubeMapFrontTexturePreviewBox (),  getComposedCubeMapFrontTextureFormatLabel ())),
	              backPreview (new TexturePreview (this, getComposedCubeMapBackTexturePreviewBox (),   getComposedCubeMapBackTextureFormatLabel ())),
	              leftPreview (new TexturePreview (this, getComposedCubeMapLeftTexturePreviewBox (),   getComposedCubeMapLeftTextureFormatLabel ())),
	             rightPreview (new TexturePreview (this, getComposedCubeMapRightTexturePreviewBox (),  getComposedCubeMapRightTextureFormatLabel ())),
	               topPreview (new TexturePreview (this, getComposedCubeMapTopTexturePreviewBox (),    getComposedCubeMapTopTextureFormatLabel ())),
	            bottomPreview (new TexturePreview (this, getComposedCubeMapBottomTexturePreviewBox (), getComposedCubeMapBottomTextureFormatLabel ())),
	           cubeMapTexture ()
{ }

void
X3DComposedCubeMapTextureEditor::setComposedCubeMapTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (cubeMapTexture)
	{
		cubeMapTexture -> front ()  .removeInterest (this, &X3DComposedCubeMapTextureEditor::set_texture);
		cubeMapTexture -> back ()   .removeInterest (this, &X3DComposedCubeMapTextureEditor::set_texture);
		cubeMapTexture -> left ()   .removeInterest (this, &X3DComposedCubeMapTextureEditor::set_texture);
		cubeMapTexture -> right ()  .removeInterest (this, &X3DComposedCubeMapTextureEditor::set_texture);
		cubeMapTexture -> top ()    .removeInterest (this, &X3DComposedCubeMapTextureEditor::set_texture);
		cubeMapTexture -> bottom () .removeInterest (this, &X3DComposedCubeMapTextureEditor::set_texture);
	}

	cubeMapTexture = value;

	getComposedCubeMapTextureBox () .set_visible (cubeMapTexture);

	if (cubeMapTexture)
		getEnvironmentTextureNotebook () .set_current_page (0);

	else
	{
		cubeMapTexture = new X3D::ComposedCubeMapTexture (getCurrentContext ());
		getCurrentContext () -> addUninitializedNode (cubeMapTexture);
		getCurrentContext () -> realize ();
	}

	cubeMapTexture -> front ()  .addInterest (this, &X3DComposedCubeMapTextureEditor::set_texture, frontPreview,  std::cref (cubeMapTexture -> front ()));
	cubeMapTexture -> back ()   .addInterest (this, &X3DComposedCubeMapTextureEditor::set_texture, backPreview,   std::cref (cubeMapTexture -> back ()));
	cubeMapTexture -> left ()   .addInterest (this, &X3DComposedCubeMapTextureEditor::set_texture, leftPreview,   std::cref (cubeMapTexture -> left ()));
	cubeMapTexture -> right ()  .addInterest (this, &X3DComposedCubeMapTextureEditor::set_texture, rightPreview,  std::cref (cubeMapTexture -> right ()));
	cubeMapTexture -> top ()    .addInterest (this, &X3DComposedCubeMapTextureEditor::set_texture, topPreview,    std::cref (cubeMapTexture -> top ()));
	cubeMapTexture -> bottom () .addInterest (this, &X3DComposedCubeMapTextureEditor::set_texture, bottomPreview, std::cref (cubeMapTexture -> bottom ()));

	set_texture (frontPreview,  cubeMapTexture -> front ());
	set_texture (backPreview,   cubeMapTexture -> back ());
	set_texture (leftPreview,   cubeMapTexture -> left ());
	set_texture (rightPreview , cubeMapTexture -> right ());
	set_texture (topPreview,    cubeMapTexture -> top ());
	set_texture (bottomPreview, cubeMapTexture -> bottom ());
}

const X3D::X3DPtr <X3D::ComposedCubeMapTexture> &
X3DComposedCubeMapTextureEditor::getComposedCubeMapTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getComposedCubeMapTextureBox () .set_visible (true);

	if (value)
	{
		switch (value -> getType () .back ())
		{
			default:
				break;
		}
	}

	return cubeMapTexture;
}

void
X3DComposedCubeMapTextureEditor::set_texture (const std::shared_ptr <TexturePreview> & preview, const X3D::SFNode & texture)
{
	preview -> setTexture (X3D::X3DPtr <X3D::X3DTextureNode> (X3D::X3DPtr <X3D::X3DTexture2DNode> (texture)));
}

X3DComposedCubeMapTextureEditor::~X3DComposedCubeMapTextureEditor ()
{ }

} // puck
} // titania
