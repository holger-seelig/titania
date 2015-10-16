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

#include "X3DArc2DEditor.h"

#include <Titania/X3D/Components/Shape/X3DShapeNode.h>

namespace titania {
namespace puck {

X3DArc2DEditor::X3DArc2DEditor () :
	X3DGeometryPropertiesEditorInterface (),
	                          shapeNodes (),
	                          startAngle (this, getArc2DStartAngleAdjustment (), getArc2DStartAngleSpinButton (), "startAngle"),
	                            endAngle (this, getArc2DEndAngleAdjustment (), getArc2DEndAngleSpinButton (), "endAngle"),
	                              radius (this, getArc2DRadiusAdjustment (), getArc2DRadiusSpinButton (), "radius")
{ }

void
X3DArc2DEditor::initialize ()
{
	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DArc2DEditor::set_selection);

	set_selection ();
}

void
X3DArc2DEditor::set_selection ()
{
	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .removeInterest (this, &X3DArc2DEditor::set_geometry);

	shapeNodes = getSelection <X3D::X3DShapeNode> ({ X3D::X3DConstants::X3DShapeNode });

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .addInterest (this, &X3DArc2DEditor::set_geometry);

	set_geometry ();
}

void
X3DArc2DEditor::set_geometry ()
{
	// Check if there is a direct master selecection of our node type.

	const auto & selection = getBrowserWindow () -> getSelection () -> getChildren ();

	if (not selection .empty ())
	{
		const X3D::X3DPtr <X3D::Arc2D> node (selection .back ());

		if (node)
		{
			set_arc2D (node);
			return;
		}
	}

	// Check if all shape node whithin the selection have a node of our type.

	const auto    pair   = getNode <X3D::Arc2D> (shapeNodes, "geometry");
	const int32_t active = pair .second;

	if (active == SAME_NODE) // All shapes share the same geometry
		set_arc2D (pair .first);
	else
		set_arc2D (nullptr);
}

void
X3DArc2DEditor::set_arc2D (const X3D::X3DPtr <X3D::Arc2D> & node)
{
	const X3D::MFNode nodes (node ? X3D::MFNode ({ node }) : X3D::MFNode ());

	getArc2DExpander () .set_visible (node);

	startAngle .setNodes (nodes);
	endAngle   .setNodes (nodes);
	radius     .setNodes (nodes);
}

X3DArc2DEditor::~X3DArc2DEditor ()
{ }

} // puck
} // titania
