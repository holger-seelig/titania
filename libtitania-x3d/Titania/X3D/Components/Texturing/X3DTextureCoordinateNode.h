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

#ifndef __TITANIA_X3D_COMPONENTS_TEXTURING_X3DTEXTURE_COORDINATE_NODE_H__
#define __TITANIA_X3D_COMPONENTS_TEXTURING_X3DTEXTURE_COORDINATE_NODE_H__

#include "../../Rendering/TexCoordArray.h"
#include "../Rendering/X3DGeometricPropertyNode.h"

namespace titania {
namespace X3D {

class ShapeContainer;

class X3DTextureCoordinateNode :
	public X3DGeometricPropertyNode
{
public:

	using X3DGeometricPropertyNode::assign;

	///  @name Member access

	virtual
	void
	init (MultiTexCoordArray &) const = 0;

	virtual
	void
	set1Point (const size_t, const Vector4f &) = 0;

	virtual
	Vector4f
	get1Point (const size_t) const = 0;

	void
	addTexCoord (MultiTexCoordArray & texCoord, const size_t index) const
	{ addTexCoord (0, texCoord, index); }

	virtual
	void
	addTexCoord (const size_t, MultiTexCoordArray &, const size_t) const = 0;

	virtual
	void
	getTexCoord (std::vector <Vector4f> &) const = 0;

	virtual
	bool
	isEmpty () const = 0;

	virtual
	size_t
	getSize () const = 0;

	///  @name Operations

	virtual
	void
	assign (const X3DPtr <X3DTextureCoordinateNode> & other);

	virtual
	void
	erasePoint (const size_t index) = 0;

	virtual
	void
	resize (const size_t value) = 0;

	///  @name Shader

	virtual
	void
	setShaderUniforms (X3DProgrammableShaderObject* const shaderObject) const;

	virtual
	void
	setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i) const;


protected:

	X3DTextureCoordinateNode ();

};

} // X3D
} // titania

#endif
