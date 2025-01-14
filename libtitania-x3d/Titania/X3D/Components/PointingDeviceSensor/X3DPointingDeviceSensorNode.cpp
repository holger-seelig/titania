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

#include "X3DPointingDeviceSensorNode.h"

#include "../../Browser/PointingDeviceSensor/PointingDeviceSensorContainer.h"
#include "../../Browser/Notification.h"
#include "../../Browser/Selection.h"
#include "../../Browser/X3DBrowser.h"

#include "../Layering/X3DLayerNode.h"
#include "../Layering/X3DViewportNode.h"

namespace titania {
namespace X3D {

X3DPointingDeviceSensorNode::Fields::Fields () :
	description (new SFString ()),
	     isOver (new SFBool ())
{ }

X3DPointingDeviceSensorNode::X3DPointingDeviceSensorNode () :
	   X3DSensorNode (),
	          fields (),
	        viewport (),
	projectionMatrix (),
	 modelViewMatrix ()
{
	addType (X3DConstants::X3DPointingDeviceSensorNode);
}

void
X3DPointingDeviceSensorNode::initialize ()
{
	X3DSensorNode::initialize ();

	enabled () .addInterest (&X3DPointingDeviceSensorNode::set_enabled, this);
}

void
X3DPointingDeviceSensorNode::set_enabled ()
{
	if (not enabled ())
	{
		if (isActive ())
			isActive () = false;

		if (isOver ())
			isOver () = false;
	}
}

void
X3DPointingDeviceSensorNode::set_over (const bool over, const HitPtr &, const Matrix4d & modelViewMatrix, const Matrix4d & projectionMatrix, const Vector4i & viewport)
{
	setViewport (viewport);
	setProjectionMatrix (projectionMatrix);
	setModelViewMatrix (modelViewMatrix);

	if (over not_eq isOver ())
	{
		isOver () = over;

		if (isOver () and not description () .empty ())
			getBrowser () -> getNotification () -> string () = description ();
	}
}

void
X3DPointingDeviceSensorNode::set_active (const bool active, const HitPtr & hit, const Matrix4d & modelViewMatrix, const Matrix4d & projectionMatrix, const Vector4i & viewport)
{
	setViewport (viewport);
	setProjectionMatrix (projectionMatrix);
	setModelViewMatrix (modelViewMatrix);

	if (active not_eq isActive ())
		isActive () = active;
}

void
X3DPointingDeviceSensorNode::push (X3DRenderObject* const renderObject)
{
	if (not enabled ())
		return;

	if (not getExecutionContext () -> isLive () or not isLive ())
		return;

	if (not getScene () -> getPrivate () and renderObject -> getBrowser () -> getSelectable ())
		return;

	auto & sensors   = renderObject -> getBrowser () -> getSensors () .back ();
	auto   container = std::make_shared <PointingDeviceSensorContainer> (this,
	                                                                     renderObject -> getModelViewMatrix  () .get (),
	                                                                     renderObject -> getProjectionMatrix () .get (),
	                                                                     renderObject -> getViewVolumes () .back () .getViewport ());

	sensors .emplace (std::move (container));
}

} // X3D
} // titania
