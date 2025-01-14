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

#ifndef __TITANIA_X3D_RENDERING_PROJECTIVE_TEXTURE_CONTAINER_H__
#define __TITANIA_X3D_RENDERING_PROJECTIVE_TEXTURE_CONTAINER_H__

#include "../Rendering/OpenGL.h"
#include "../Rendering/X3DCollectableObject.h"

#include <memory>

namespace titania {
namespace X3D {

class X3DBrowser;
class X3DTextureProjectorNode;
class X3DProgrammableShaderObject;
class X3DRenderObject;

class ProjectiveTextureContainer :
	public X3DCollectableObject
{
public:

	///  @name Construction

	ProjectiveTextureContainer (X3DBrowser* const browser, X3DTextureProjectorNode* const node, const Matrix4d & modelViewMatrix);

	///  @name Member access

	Matrix4d &
	getModelViewMatrix ()
	{ return modelViewMatrix; }

	const Matrix4d &
	getModelViewMatrix () const
	{ return modelViewMatrix; }

	void
	setProjectiveTextureMatrix (const Matrix4d & value)
	{ projectiveTextureMatrix = value; }

	const Matrix4d &
	getProjectiveTextureMatrix () const
	{ return projectiveTextureMatrix; }

	void
	setProjectiveTextureLocation (const Vector3f & value)
	{ projectiveTextureLocation = value; }

	const Vector3f &
	getProjectiveTextureLocation () const
	{ return projectiveTextureLocation; }

	///  @name Operations

	void
	setGlobalVariables (X3DRenderObject* const renderObject);

	virtual
	void
	setShaderUniforms (X3DRenderObject* const renderObject, X3DProgrammableShaderObject* const shaderObject);

	///  @name Destruction

	~ProjectiveTextureContainer ();


private:

	///  @name Members

	X3DBrowser* const              browser;
	X3DTextureProjectorNode* const node;
	Matrix4d                       modelViewMatrix;
	Matrix4d                       projectiveTextureMatrix;
	Vector3f                       projectiveTextureLocation;

};

using ProjectiveTextureContainerArray = std::vector <std::shared_ptr <ProjectiveTextureContainer>>;

} // X3D
} // titania

#endif
