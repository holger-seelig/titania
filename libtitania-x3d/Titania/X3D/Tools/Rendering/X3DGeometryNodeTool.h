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

#ifndef __TITANIA_X3D_TOOLS_RENDERING_X3DGEOMETRY_NODE_TOOL_H__
#define __TITANIA_X3D_TOOLS_RENDERING_X3DGEOMETRY_NODE_TOOL_H__

#include "../Core/X3DNodeTool.h"

#include "../../Components/Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

class NormalTool;
class CoordinateTool;

class X3DGeometryNodeTool :
	virtual public X3DGeometryNode,
	virtual public X3DNodeTool
{
public:

	///  @name Private fields

	SFBool &
	pickable ()
	{ return *fields .pickable; }

	const SFBool &
	pickable () const
	{ return *fields .pickable; }

	SFNode &
	normalTool ()
	{ return *fields .normalTool; }

	const SFNode &
	normalTool () const
	{ return *fields .normalTool; }

	SFNode &
	coordTool ()
	{ return *fields .coordTool; }

	const SFNode &
	coordTool () const
	{ return *fields .coordTool; }

	///  @name Member access

	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	virtual
	bool
	isTransparent () const override
	{ return getNode <X3DGeometryNode> () -> isTransparent (); }

	virtual
	bool
	isLineGeometry () const override
	{ return getNode <X3DGeometryNode> () -> isLineGeometry (); }

	///  @name Member access

	virtual
	const Box3d &
	getBBox () const final override
	{ return getNode <X3DGeometryNode> () -> getBBox (); }

	virtual
	bool
	getSolid () const final override
	{ return getNode <X3DGeometryNode> () -> getSolid (); }

	virtual
	bool
	getCCW () const final override
	{ return getNode <X3DGeometryNode> () -> getCCW (); }

	virtual
	const std::vector <Color4f> &
	getPolygonColors () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonColors (); }

	virtual
	const TexCoordArray &
	getPolygonTexCoords () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonTexCoords (); }

	virtual
	const std::vector <Vector3f> &
	getPolygonNormals () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonNormals (); }

	virtual
	const std::vector <Vector3d> &
	getPolygonVertices () const final override
	{ return getNode <X3DGeometryNode> () -> getPolygonVertices (); }

	virtual
	const std::vector <X3DGeometryNode::Element> &
	getElements () const final override
	{ return getNode <X3DGeometryNode> () -> getElements (); }

	const X3DPtr <NormalTool> &
	getNormalTool () const
	{ return normalToolNode; }

	const X3DPtr <CoordinateTool> &
	getCoordinateTool () const
	{ return coordToolNode; }
	
	///  @name Operations

	virtual
	bool
	intersects (Line3d, std::vector <IntersectionPtr> &) const final override
	{ return false; }

	virtual
	void
	intersects (const std::shared_ptr <FrameBuffer> &, const std::shared_ptr <FrameBuffer> &) final override;

	virtual
	bool
	intersects (CollisionSphere3d sphere, const CollectableObjectArray & collectables) const final override
	{ return getNode <X3DGeometryNode> () -> intersects (sphere, collectables); }

	virtual
	void
	triangulate (std::vector <Color4f> & colors, TexCoordArray & texCoords, std::vector <Vector3f> & normals, std::vector <Vector3d> & vertices) const final override
	{ getNode <X3DGeometryNode> () -> triangulate (colors, texCoords, normals, vertices); }

	virtual
	void
	collision (const CollisionContainer* const container) final override
	{ getNode <X3DGeometryNode> () -> collision (container); }

	virtual
	void
	traverse (const TraverseType) final override;

	virtual
	void
	draw (const ShapeContainer* const container) final override;

	virtual
	SFNode
	toPrimitive () const
	throw (Error <NOT_SUPPORTED>,
	       Error <DISPOSED>) override
	{ return getNode <X3DGeometryNode> () -> toPrimitive (); }

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DGeometryNodeTool ();


protected:

	///  @name Construction

	X3DGeometryNodeTool ();

	virtual
	void
	initialize () override;

	const Vector4i &
	getViewport () const
	{ return viewport; }

	const Matrix4d &
	getProjectionMatrix () const
	{ return projectionMatrix; }

	const Matrix4d &
	getModelViewMatrix () const
	{ return modelViewMatrix; }

	virtual
	void
	set_selection (const std::vector <Vector3d> &)
	{ }


private:

	///  @name Operations

	void
	set_loadState ();

	void
	set_pickable ();

	void
	eventProcessed ();

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const pickable;
		SFNode* const normalTool;
		SFNode* const coordTool;
	};

	Fields fields;

	X3DPtr <NormalTool>     normalToolNode;
	X3DPtr <CoordinateTool> coordToolNode;

	Vector4i viewport;
	Matrix4d projectionMatrix;
	Matrix4d modelViewMatrix;

};

} // X3D
} // titania

#endif
