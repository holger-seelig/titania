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

#include "X3DEnvironmentalSensorNodeTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Grouping/Transform.h"
#include "../../Rendering/X3DRenderObject.h"

#include "../Grouping/X3DTransformNodeTool.h"

namespace titania {
namespace X3D {

X3DEnvironmentalSensorNodeTool::X3DEnvironmentalSensorNodeTool (const Color3f & color) :
	X3DEnvironmentalSensorNode (),
	         X3DSensorNodeTool (),
	          X3DBoundedObject (),
	                     color (color),
	                 startSize (),
	               startCenter ()
{
	addType (X3DConstants::X3DEnvironmentalSensorNodeTool);

	addChildObjects (bboxSize (), bboxCenter ());
}

void
X3DEnvironmentalSensorNodeTool::initialize ()
{
	X3DSensorNodeTool::initialize ();
	X3DBoundedObject::initialize ();

	requestAsyncLoad ({ get_tool ("EnvironmentalSensor.x3dv") .str () });
}

void
X3DEnvironmentalSensorNodeTool::realize ()
{
	try
	{
		setTransformTool (0, getInlineNode () -> getExportedNode <Transform> ("TransformTool"));

		const auto & transformTool = getTransformTools () [0];
		const auto & toolNode      = getToolNode ();
	
		transformTool -> tools ()         = { "TRANSLATE", "SCALE" };
		transformTool -> displayCenter () = false;

		toolNode -> setField <SFColor> ("color", color);
		toolNode -> setField <SFNode>  ("node",  getNode <X3DEnvironmentalSensorNode> ());

		auto & set_enabled = getToolNode () -> getField <SFBool> ("enabled");
		enabled () .addInterest (set_enabled);
		set_enabled = getNode <X3DEnvironmentalSensorNode> () -> enabled ();

		addTool ();
	}
	catch (const X3DError & error)
	{ }
}

Box3d
X3DEnvironmentalSensorNodeTool::getBBox () const
{
	if (getBrowser () -> getDisplayTools () .top ())
		return Box3d (size () .getValue (), center () .getValue ());

	return Box3d ();
}

bool
X3DEnvironmentalSensorNodeTool::getPickable (X3DRenderObject* const renderObject) const
{
	try
	{
		if (getTransformTools () .empty ())
			return renderObject -> getBrowser () -> getToolsPickable () .top ();

		const auto & transformTool = getTransformTools () [0];

		if (renderObject -> getBrowser () -> getToolsPickable () .top ())
			transformTool -> isPickable () = true;
		else
			transformTool -> isPickable () = false;

		return true;
	}
	catch (const X3DError & error)
	{
		// __LOG__ << error .what () << std::endl;
		return renderObject -> getBrowser () -> getToolsPickable () .top ();
	}
}

void
X3DEnvironmentalSensorNodeTool::beginUndo ()
{
	startSize   = size ();
	startCenter = center ();
}

void
X3DEnvironmentalSensorNodeTool::endUndo (const UndoStepPtr & undoStep)
{
	if (size ()   not_eq startSize or
	    center () not_eq startCenter)
	{
		undoStep -> addUndoFunction (&SFVec3f::setValue, std::ref (center ()), startCenter);
		undoStep -> addUndoFunction (&SFVec3f::setValue, std::ref (size ()), startSize);
		undoStep -> addUndoFunction (&X3DEnvironmentalSensorNodeTool::setChanging, X3DPtr <X3DEnvironmentalSensorNode> (this), 0, true);

		undoStep -> addRedoFunction (&X3DEnvironmentalSensorNodeTool::setChanging, X3DPtr <X3DEnvironmentalSensorNode> (this), 0, true);
		undoStep -> addRedoFunction (&SFVec3f::setValue, std::ref (size ()), size ());
		undoStep -> addRedoFunction (&SFVec3f::setValue, std::ref (center ()), center ());
	}
}

void
X3DEnvironmentalSensorNodeTool::addTool ()
{
	try
	{
		if (getTransformTools () .empty ())
			return;

		const auto & transformTool = getTransformTools () [0];
		const auto & toolNode      = getToolNode ();
		const auto   selected      = getBrowser () -> getSelection () -> isSelected (SFNode (this));

		transformTool -> setField <SFBool> ("grouping", selected);
		toolNode      -> setField <SFBool> ("selected", selected);
	}
	catch (const X3DError &)
	{ }
}

void
X3DEnvironmentalSensorNodeTool::removeTool (const bool really)
{
	if (really)
	{
		X3DSensorNodeTool::removeTool ();
	}
	else
	{
		try
		{
			if (getTransformTools () .empty ())
				return;

			const auto & transformTool = getTransformTools () [0];
			const auto & toolNode      = getToolNode ();

			transformTool -> setField <SFBool> ("grouping", false);
			toolNode      -> setField <SFBool> ("selected", false);
		}
		catch (const X3DError &)
		{ }
	}
}

void
X3DEnvironmentalSensorNodeTool::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	getNode <X3DEnvironmentalSensorNode> () -> traverse (type, renderObject);

	// Tool

	X3DToolObject::traverse (type, renderObject);
}

void
X3DEnvironmentalSensorNodeTool::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DSensorNodeTool::dispose ();

	removeChildObjects (bboxSize (), bboxCenter ());
}

X3DEnvironmentalSensorNodeTool::~X3DEnvironmentalSensorNodeTool ()
{ }

} // X3D
} // titania
