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

#ifndef __TITANIA_X3D_TOOLS_RIGID_BODY_PHYSICS_COLLIDABLE_SHAPE_TOOL_H__
#define __TITANIA_X3D_TOOLS_RIGID_BODY_PHYSICS_COLLIDABLE_SHAPE_TOOL_H__

#include "../RigidBodyPhysics/X3DNBodyCollidableNodeTool.h"
#include "../ToolColors.h"

#include "../../Components/RigidBodyPhysics/CollidableShape.h"
#include "../../Components/Shape/Shape.h"

namespace titania {
namespace X3D {

class CollidableShapeTool :
	virtual public CollidableShape,
	public X3DNBodyCollidableNodeTool
{
public:

	///  @name Construction

	CollidableShapeTool (X3DBaseNode* const node) :
		               X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		           CollidableShape (node -> getExecutionContext ()),
		               X3DBaseTool (node),
		X3DNBodyCollidableNodeTool (ToolColors::RED)
	{
		//addType (X3DConstants::CollidableShapeTool);
	}

	///  @name Fields

	virtual
	SFNode &
	shape () final override
	{ return getNode <CollidableShape> () -> shape (); }

	virtual
	const SFNode &
	shape () const final override
	{ return getNode <CollidableShape> () -> shape (); }

	///  @name Member access

	virtual
	Box3d
	getBBox () const final override
	{ return X3DNBodyCollidableNodeTool::getBBox (); }

	virtual
	void
	setConvex (const bool value) final override
	{ getNode <CollidableShape> () -> setConvex (value); }

	virtual
	const SFBool &
	getConvex () const final override
	{ return getNode <CollidableShape> () -> getConvex (); }

	virtual
	const X3DPtr <Shape> &
	getShape () const final override
	{ return getNode <CollidableShape> () -> getShape (); }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{ X3DNBodyCollidableNodeTool::traverse (type, renderObject); }

	///  @name Destruction

	virtual
	void
	dispose ()
	{ X3DNBodyCollidableNodeTool::dispose (); }

	virtual
	~CollidableShapeTool () final override
	{ }


protected:
	
	///  @name Construction

	virtual
	void
	initialize () final override
	{ X3DNBodyCollidableNodeTool::initialize (); }

	///  @name Member access

	virtual
	Box3d
	getChildBBox () const final override
	{
		if (getShape ())
			return getShape () -> getBBox ();

		return Box3d ();
	}

};

} // X3D
} // titania

#endif
