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

#include "Appearance.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/X3DProgrammableShaderObject.h"
#include "../Shaders/X3DShaderNode.h"
#include "../Shape/FillProperties.h"
#include "../Shape/LineProperties.h"
#include "../Shape/X3DMaterialNode.h"
#include "../Texturing/TextureTransform.h"
#include "../Texturing/X3DTextureNode.h"

namespace titania {
namespace X3D {

const ComponentType Appearance::component      = ComponentType::SHAPE;
const std::string   Appearance::typeName       = "Appearance";
const std::string   Appearance::containerField = "appearance";

Appearance::Fields::Fields () :
	  fillProperties (new SFNode ()),
	  lineProperties (new SFNode ()),
	        material (new SFNode ()),
	         texture (new SFNode ()),
	textureTransform (new SFNode ()),
	         shaders (new MFNode ())
{ }

Appearance::Appearance (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext),
	   X3DAppearanceNode (),
	              fields (),
	  fillPropertiesNode (),
	  linePropertiesNode (),
	        materialNode (),
	         textureNode (),
	textureTransformNode (),
	         shaderNodes (),
	          shaderNode ()
{
	addType (X3DConstants::Appearance);

	addField (inputOutput, "metadata",         metadata ());
	addField (inputOutput, "fillProperties",   fillProperties ());
	addField (inputOutput, "lineProperties",   lineProperties ());
	addField (inputOutput, "material",         material ());
	addField (inputOutput, "texture",          texture ());
	addField (inputOutput, "textureTransform", textureTransform ());
	addField (inputOutput, "shaders",          shaders ());

	addChildren (fillPropertiesNode,
	             linePropertiesNode,
	             materialNode,
	             textureNode,
	             textureTransformNode,
	             shaderNodes,
	             shaderNode);
}

X3DBaseNode*
Appearance::create (X3DExecutionContext* const executionContext) const
{
	return new Appearance (executionContext);
}

void
Appearance::initialize ()
{
	X3DAppearanceNode::initialize ();

	fillProperties ()   .addInterest (this, &Appearance::set_fillProperties);
	lineProperties ()   .addInterest (this, &Appearance::set_lineProperties);
	material ()         .addInterest (this, &Appearance::set_material);
	texture ()          .addInterest (this, &Appearance::set_texture);
	textureTransform () .addInterest (this, &Appearance::set_textureTransform);
	shaders ()          .addInterest (this, &Appearance::set_shaders);

	shaderNodes .addInterest (this, &Appearance::set_shader);

	set_lineProperties ();
	set_fillProperties ();
	set_material ();
	set_texture ();
	set_textureTransform ();
	set_shaders ();
}

void
Appearance::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DAppearanceNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		set_lineProperties ();
		set_fillProperties ();
		set_textureTransform ();
	}
}

bool
Appearance::isTransparent () const
{
	if (materialNode and materialNode -> isTransparent ())
		return true;

	if (textureNode and textureNode -> isTransparent ())
		return true;

	return false;
}

void
Appearance::set_fillProperties ()
{
	fillPropertiesNode .set (x3d_cast <FillProperties*> (fillProperties ()));

	if (fillPropertiesNode)
		return;

	fillPropertiesNode .set (getBrowser () -> getDefaultFillProperties ());
}

void
Appearance::set_lineProperties ()
{
	linePropertiesNode .set (x3d_cast <LineProperties*> (lineProperties ()));

	if (linePropertiesNode)
		return;

	linePropertiesNode .set (getBrowser () -> getDefaultLineProperties ());
}

void
Appearance::set_material ()
{
	materialNode .set (x3d_cast <X3DMaterialNode*> (material ()));
}

void
Appearance::set_texture ()
{
	if (textureNode)
		textureNode -> isCameraObject () .removeInterest (const_cast <SFBool &> (isCameraObject ()));

	textureNode .set (x3d_cast <X3DTextureNode*> (texture ()));

	if (textureNode)
	{
		textureNode -> isCameraObject () .addInterest (const_cast <SFBool &> (isCameraObject ()));

		setCameraObject (textureNode -> isCameraObject ());
	}
	else
		setCameraObject (false);
}

void
Appearance::set_textureTransform ()
{
	textureTransformNode .set (x3d_cast <X3DTextureTransformNode*> (textureTransform ()));

	if (textureTransformNode)
		return;

	textureTransformNode .set (getBrowser () -> getDefaultTextureTransform ());
}

void
Appearance::set_shaders ()
{
	using addEvent = void (X3DPtrArray <X3DShaderNode>::*) ();

	for (const auto & shaderNode : shaderNodes)
		shaderNode -> isValid () .removeInterest (shaderNodes, (addEvent) & X3DPtrArray <X3DShaderNode>::addEvent);

	shaderNodes .clear ();

	for (const auto & node : shaders ())
	{
		const auto shaderNode = x3d_cast <X3DShaderNode*> (node);

		if (shaderNode)
		{
			shaderNodes .emplace_back (shaderNode);
			shaderNode -> isValid () .addInterest (shaderNodes, (addEvent) & X3DPtrArray <X3DShaderNode>::addEvent);
		}
	}
}

void
Appearance::set_shader ()
{
	if (shaderNode)
		shaderNode -> deselect ();

	// Select shader

	shaderNode .set (nullptr);

	for (const auto & shader : shaderNodes)
	{
		if (shader -> isValid ())
		{
			shaderNode .set (shader);

			shaderNode -> select ();
			break;
		}
	}
}

void
Appearance::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (textureNode)
		textureNode -> traverse (type, renderObject);
}

void
Appearance::draw ()
{
	#ifdef FIXED_PIPELINE
	if (getBrowser () -> getFixedPipelineRequired ())
	{
		// Material

		if (materialNode)
			materialNode -> draw ();
	
		else
		{
			glDisable (GL_LIGHTING);
			glColor4f (1, 1, 1, 1);
		}
	
		// Texture
	
		if (textureNode)
		{
			textureNode -> draw ();
			getBrowser () -> setTexture (textureNode);
		}
	
		// TextureTransform
	
		textureTransformNode -> draw ();
	
		// Shader
	
		if (shaderNode)
			shaderNode -> draw ();
	}
	#endif

	getBrowser () -> setLineProperties (linePropertiesNode);
	getBrowser () -> setMaterial (materialNode);
	getBrowser () -> setTexture (textureNode);
	getBrowser () -> setTextureTransform (textureTransformNode);
	getBrowser () -> setShader (shaderNode ? shaderNode : getBrowser () -> getDefaultShader ());
}

} // X3D
} // titania
