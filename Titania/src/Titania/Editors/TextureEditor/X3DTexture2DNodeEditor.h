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

#ifndef __TITANIA_TEXTURE_EDITOR_X3DTEXTURE2DNODE_EDITOR_H__
#define __TITANIA_TEXTURE_EDITOR_X3DTEXTURE2DNODE_EDITOR_H__

#include "../../ComposedWidgets.h"
#include "../../UserInterfaces/X3DTextureEditorInterface.h"
#include "X3DImageTextureEditor.h"
#include "X3DMovieTextureEditor.h"
#include "X3DPixelTextureEditor.h"

namespace titania {
namespace puck {

class X3DTexture2DNodeEditor :
	virtual public X3DTextureEditorInterface,
	public X3DImageTextureEditor,
	public X3DPixelTextureEditor,
	public X3DMovieTextureEditor
{
public:

	virtual
	~X3DTexture2DNodeEditor () override;


protected:

	///  @name Construction

	X3DTexture2DNodeEditor ();

	///  @name Member access

	void
	setTexture2DNode (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureNode> &);

	virtual
	const X3D::X3DPtr <X3D::ImageTexture> &
	getImageTexture (const X3D::X3DPtr <X3D::X3DTextureNode> &) final override;

	virtual
	const X3D::X3DPtr <X3D::PixelTexture> &
	getPixelTexture (const X3D::X3DPtr <X3D::X3DTextureNode> &) final override;

	virtual
	const X3D::X3DPtr <X3D::MovieTexture> &
	getMovieTexture (const X3D::X3DPtr <X3D::X3DTextureNode> &) final override;


private:

	///  @name Event handlers

	bool
	on_configure_event (GdkEventConfigure* const);

	///  @name Construction

	void
	setTexture2DNode (const X3D::X3DPtr <X3D::X3DTexture2DNode> &, const X3D::X3DPtr <X3D::X3DTextureNode> &);

	///  @name Members

	X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode;
	X3DFieldToggleButton <X3D::SFBool>  repeatS;
	X3DFieldToggleButton <X3D::SFBool>  repeatT;

};

} // puck
} // titania

#endif
