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

#include "X3DTexture3DNodeEditor.h"

namespace titania {
namespace puck {

X3DTexture3DNodeEditor::X3DTexture3DNodeEditor () :
	          X3DBaseInterface (),
	 X3DTextureEditorInterface (),
	X3DComposedTexture3DEditor (),
	   X3DImageTexture3DEditor (),
	   X3DPixelTexture3DEditor (),
	             texture3DNode (),
	                   repeatS (this, getTexture3DNodeRepeatSCheckButton (), "repeatS"),
	                   repeatT (this, getTexture3DNodeRepeatTCheckButton (), "repeatT"),
	                   repeatR (this, getTexture3DNodeRepeatRCheckButton (), "repeatR")
{
	addChildren (texture3DNode);
}

void
X3DTexture3DNodeEditor::setTexture3DNode (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	setComposedTexture3D (value);
	setImageTexture3D (value);
	setPixelTexture3D (value);

	setTexture3DNode (X3D::X3DPtr <X3D::X3DTexture3DNode> (value), value);
}

const X3D::X3DPtr <X3D::ComposedTexture3D> &
X3DTexture3DNodeEditor::getComposedTexture3D (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	const auto & texture = X3DComposedTexture3DEditor::getComposedTexture3D (value);

	setTexture3DNode (X3D::X3DPtr <X3D::X3DTexture3DNode> (texture), value);

	return texture;
}

const X3D::X3DPtr <X3D::ImageTexture3D> &
X3DTexture3DNodeEditor::getImageTexture3D (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	const auto & texture = X3DImageTexture3DEditor::getImageTexture3D (value);

	setTexture3DNode (X3D::X3DPtr <X3D::X3DTexture3DNode> (texture), value);

	return texture;
}

const X3D::X3DPtr <X3D::PixelTexture3D> &
X3DTexture3DNodeEditor::getPixelTexture3D (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	const auto & texture = X3DPixelTexture3DEditor::getPixelTexture3D (value);

	setTexture3DNode (X3D::X3DPtr <X3D::X3DTexture3DNode> (texture), value);

	return texture;
}

void
X3DTexture3DNodeEditor::setTexture3DNode (const X3D::X3DPtr <X3D::X3DTexture3DNode> & texture, const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getTexture3DBox () .set_visible (texture);

	texture3DNode = texture;

	if (not texture3DNode or not value)
	{
		repeatS .setNodes ({ });
		repeatT .setNodes ({ });
		repeatR .setNodes ({ });
		return;
	}

	repeatS .setNodes ({ texture3DNode });
	repeatT .setNodes ({ texture3DNode });
	repeatR .setNodes ({ texture3DNode });

	if (texture3DNode == value)
		return;

	for (const auto & type : value -> getType ())
	{
		switch (type)
		{
			case X3D::X3DConstants::X3DTexture3DNode :
				{
					const X3D::X3DPtr <X3D::X3DTexture3DNode> last (value);

					texture3DNode -> repeatS ()           = last -> repeatS ();
					texture3DNode -> repeatT ()           = last -> repeatT ();
					texture3DNode -> repeatR ()           = last -> repeatR ();
					texture3DNode -> textureProperties () = last -> textureProperties ();
					break;
				}
			default:
				break;
		}
	}
}

X3DTexture3DNodeEditor::~X3DTexture3DNodeEditor ()
{ }

} // puck
} // titania
