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

#include "X3DShadingContext.h"

#include "../../Components/Networking/LoadSensor.h"
#include "../../Components/Shaders/ComposedShader.h"
#include "../../Components/Shaders/ShaderPart.h"
#include "../../Rendering/OpenGL.h"
#include "../Networking/config.h"
#include "../BrowserOptions.h"
#include "../X3DBrowser.h"

namespace titania {
namespace X3D {

X3DShadingContext::X3DShadingContext () :
	           X3DBaseNode (),
	shadingLanguageVersion (),
	           phongShader (new ComposedShader (getExecutionContext ())),
	         defaultShader (),
	            shaderNode (nullptr)
{
	addChildren (phongShader, defaultShader);
}

void
X3DShadingContext::initialize ()
{
	if (glXGetCurrentContext ())
	{
		// shadingLanguageVersionStream

		std::istringstream shadingLanguageVersionStream ((const char*) glGetString (GL_SHADING_LANGUAGE_VERSION));

		shadingLanguageVersionStream .imbue (std::locale::classic ());

		shadingLanguageVersionStream >> shadingLanguageVersion;

		// Phong Shader

		const auto phongVertex   = new ShaderPart (getExecutionContext ());
		const auto phongFragment = new ShaderPart (getExecutionContext ());

		phongFragment -> type () = "FRAGMENT";
		phongVertex   -> url ()  = { get_shader ("Shaders/Phong.vs") .str () };
		phongFragment -> url ()  = { get_shader ("Shaders/Phong.fs") .str () };

		phongShader -> parts () .emplace_back (phongVertex);
		phongShader -> parts () .emplace_back (phongFragment);

		phongVertex   -> setup ();
		phongFragment -> setup ();
		phongShader   -> setup ();

		getBrowser () -> getLoadSensor () -> watchList () .emplace_back (phongVertex);
		getBrowser () -> getLoadSensor () -> watchList () .emplace_back (phongFragment);

		// Shading

		getBrowser () -> getBrowserOptions () -> Shading () .addInterest (this, &X3DShadingContext::set_Shading);

		set_Shading (getBrowser () -> getBrowserOptions () -> Shading ());
	}
}

void
X3DShadingContext::set_Shading (const String & Shading)
{
	if (Shading == "POINTSET")
	{
		defaultShader = nullptr;
	}
	else if (Shading == "WIREFRAME")
	{
		defaultShader = nullptr;
	}
	else if (Shading == "FLAT")
	{
		defaultShader = nullptr;
	}
	else if (Shading == "PHONG")
	{
		defaultShader = getPhongShader ();
	}
	else  // GOURAUD
	{
		defaultShader = nullptr;
	}
}

X3DShadingContext::~X3DShadingContext ()
{ }

} // X3D
} // titania
