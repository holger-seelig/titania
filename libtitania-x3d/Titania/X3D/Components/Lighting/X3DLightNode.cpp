/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "X3DLightNode.h"

#include "../../Browser/Browser.h"

namespace titania {
namespace X3D {

struct X3DLightNode::Type
{
	Type ()
	{
		GLint max_lights = 0;

		glGetIntegerv (GL_MAX_LIGHTS, &max_lights);

		lightIds .reserve (max_lights);

		for (GLint i = 0; i < max_lights; ++ i)
			lightIds .push_back (GL_LIGHT0 + i);
	}

	void
	pushLight (const GLenum lightId)
	{
		return;

		if (lightId not_eq lightIds .front ())
			lightIds .push_back (lightId);
	}

	GLenum
	popLight ()
	{
		return GL_LIGHT0;

		if (lightIds .size () > 0)
		{
			GLenum lightId = lightIds .back ();
			lightIds .pop_back ();
			return lightId;
		}

		return lightIds .front ();
	}

	std::vector <GLenum> lightIds;

};

X3DLightNode::X3DLightNode () :
	    X3DChildNode (),          
	ambientIntensity (),          // SFFloat [in,out] ambientIntensity  0            [0,1]
	           color (1, 1, 1),   // SFColor [in,out] color             1 1 1        [0,1]
	          global (true),      // SFBool  [in,out] global            FALSE
	       intensity (1),         // SFFloat [in,out] intensity         1            [0,1]
	              on (true),      // SFBool  [in,out] on                TRUE
	         lightId (GL_LIGHT0)
{
	addNodeType (X3DLightNodeType);
}

void
X3DLightNode::initialize ()
{
	X3DChildNode::initialize ();

	static X3DLightNode::Type lightNodeType;

	nodeType = &lightNodeType;
}

void
X3DLightNode::enable ()
{
	lightId = nodeType -> popLight ();
	glEnable (lightId);
}

void
X3DLightNode::disable ()
{
	glDisable (lightId);
	nodeType -> pushLight (lightId);
}

GLenum
X3DLightNode::light ()
{
	return lightId;
}

void
X3DLightNode::draw ()
{
	if (on)
	{
		if (global)
			getCurrentLayer () -> addGlobalLight (this);
		else
			getCurrentLayer () -> pushLocalLight (this);
	}
}

void
X3DLightNode::postDisplay ()
{
	if (not global and on)
		getCurrentLayer () -> popLocalLight ();
}

} // X3D
} // titania
