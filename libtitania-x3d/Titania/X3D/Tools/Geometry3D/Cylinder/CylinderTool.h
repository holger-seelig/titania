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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_CYLINDER_CYLINDER_TOOL_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_CYLINDER_CYLINDER_TOOL_H__

#include "../../../Components/Geometry3D/Cylinder.h"
#include "../../Rendering/X3DGeometryNodeTool.h"

namespace titania {
namespace X3D {

class CylinderTool :
	virtual public Cylinder,
	public X3DGeometryNodeTool
{
public:

	///  @name Construction

	CylinderTool (X3DBaseNode* const node);

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override
	{ X3DGeometryNodeTool::setExecutionContext (executionContext); }

	///  @name Fields

	virtual
	SFBool &
	top () final override
	{ return getNode <Cylinder> () -> top (); }

	virtual
	const SFBool &
	top () const final override
	{ return getNode <Cylinder> () -> top (); }

	virtual
	SFBool &
	side () final override
	{ return getNode <Cylinder> () -> side (); }

	virtual
	const SFBool &
	side () const final override
	{ return getNode <Cylinder> () -> side (); }

	virtual
	SFBool &
	bottom () final override
	{ return getNode <Cylinder> () -> bottom (); }

	virtual
	const SFBool &
	bottom () const final override
	{ return getNode <Cylinder> () -> bottom (); }

	virtual
	SFFloat &
	height () final override
	{ return getNode <Cylinder> () -> height (); }

	virtual
	const SFFloat &
	height () const final override
	{ return getNode <Cylinder> () -> height (); }

	virtual
	SFFloat &
	radius () final override
	{ return getNode <Cylinder> () -> radius (); }

	virtual
	const SFFloat &
	radius () const final override
	{ return getNode <Cylinder> () -> radius (); }

	virtual
	SFBool &
	solid () final override
	{ return getNode <Cylinder> () -> solid (); }

	virtual
	const SFBool &
	solid () const final override
	{ return getNode <Cylinder> () -> solid (); }

	///  @name Hidden fields

	virtual
	SFNode &
	options () final override
	{ return getNode <Cylinder> () -> options (); }

	virtual
	const SFNode &
	options () const final override
	{ return getNode <Cylinder> () -> options (); }

	///  @name Operations

	virtual
	SFNode
	toPrimitive () const final override
	{ return getNode <Cylinder> () -> toPrimitive (); }

	virtual
	void
	beginUndo () final override;

	virtual
	void
	endUndo (const UndoStepPtr & undoStep) final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~CylinderTool () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Event handlers

	void
	set_transform_tools ();
	
	void
	set_height ();
	
	void
	set_radius ();
	
	void
	set_scale ();
	
	void
	connectHeight (const SFFloat & field);
	
	void
	connectRadius (const SFFloat & field);
	
	void
	connectScale (const SFVec3f & field);

	///  @name Fields

	struct Fields
	{
		Fields ();

	};

	Fields fields;

	///  @name Members

	float startHeight;
	float startRadius;

};

} // X3D
} // titania

#endif
