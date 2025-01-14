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

#include "X3DActiveLayerTool.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Layout/LayoutGroup.h"
#include "../../Components/Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

X3DActiveLayerTool::Fields::Fields () :
	enabled (new SFBool (true))
{ }

X3DActiveLayerTool::X3DActiveLayerTool () :
	   X3DChildNode (),
	  X3DToolObject (),
	         fields (),
	activeLayerNode ()
{
	addType (X3DConstants::X3DActiveLayerTool);

	addChildObjects (activeLayerNode);
}

void
X3DActiveLayerTool::initialize ()
{
	X3DChildNode::initialize ();
	X3DToolObject::initialize ();

	getBrowser () -> getActiveLayer () .addInterest (&X3DActiveLayerTool::set_activeLayer, this);
	enabled () .addInterest (&X3DActiveLayerTool::set_activeLayer, this);

	set_activeLayer ();
}

void
X3DActiveLayerTool::setExecutionContext (X3DExecutionContext* const value)
{
	getBrowser () -> getActiveLayer () .removeInterest (&X3DActiveLayerTool::set_activeLayer, this);

	X3DToolObject::setExecutionContext (value);
	X3DChildNode::setExecutionContext (value);

	if (isInitialized ())
	{
		getBrowser () -> getActiveLayer () .addInterest (&X3DActiveLayerTool::set_activeLayer, this);

		set_activeLayer ();
	}
}

Matrix4d
X3DActiveLayerTool::getPickingMatrix () const
{
	if (activeLayerNode)
		return getModelMatrix () * activeLayerNode -> getViewpoint () -> getInverseCameraSpaceMatrix ();

	return Matrix4d ();
}

Matrix4d
X3DActiveLayerTool::getModelMatrix () const
{
	LayoutGroup* const layoutGroupNode = dynamic_cast <LayoutGroup*> (activeLayerNode -> getFriends () .getValue ());

	if (layoutGroupNode)
		return layoutGroupNode -> getMatrix ();

	return Matrix4d ();
}

void
X3DActiveLayerTool::set_activeLayer ()
{
	if (activeLayerNode)
		activeLayerNode -> getFriends () -> children () .remove (SFNode (this));

	activeLayerNode = getBrowser () -> getActiveLayer ();

	if (enabled () and activeLayerNode)
		activeLayerNode -> getFriends () -> children () .emplace_back (this);
}

void
X3DActiveLayerTool::dispose ()
{
	X3DToolObject::dispose ();
	X3DChildNode::dispose ();
}

X3DActiveLayerTool::~X3DActiveLayerTool ()
{ }

} // X3D
} // titania
