/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DVolumeRenderStyleNode.h"

#include "../Shaders/ComposedShader.h"

namespace titania {
namespace X3D {

X3DVolumeRenderStyleNode::Fields::Fields () :
	enabled (new SFBool (true))
{ }

X3DVolumeRenderStyleNode::X3DVolumeRenderStyleNode () :
	         X3DNode (),
	          fields (),
	 volumeDataNodes ()
{
	addType (X3DConstants::X3DVolumeRenderStyleNode);
}

X3DVolumeRenderStyleNode::~X3DVolumeRenderStyleNode ()
{ }

std::string
X3DVolumeRenderStyleNode::getStyleId () const
{
	return basic::to_string (getId (), std::locale::classic ());
}

std::string
X3DVolumeRenderStyleNode::getNormalText (const bool surfaceNormalsNode) const
{
	std::string string;

	if (surfaceNormalsNode)
	{
		string += "uniform sampler3D surfaceNormals_" + getStyleId () + ";\n";

		string += "\n";
		string += "vec4\n";
		string += "getNormal_" + getStyleId () + " (in vec3 texCoord)\n";
		string += "{\n";
		string += "	vec3 n = texture (surfaceNormals_" + getStyleId () + ", texCoord) .xyz * 2.0 - 1.0;\n";
		string += "\n";
		string += "	return vec4 (normalize (x3d_TextureNormalMatrix * n), length (n));\n";
		string += "}\n";
	}
	else
	{
		string += "\n";
		string += "vec4\n";
		string += "getNormal_" + getStyleId () + " (in vec3 texCoord)\n";
		string += "{\n";
		string += "	vec4  offset = vec4 (1.0 / x3d_TextureSize, 0.0);\n";
		string += "	float i0     = texture (x3d_Texture3D [0], texCoord + offset .xww) .r;\n";
		string += "	float i1     = texture (x3d_Texture3D [0], texCoord - offset .xww) .r;\n";
		string += "	float i2     = texture (x3d_Texture3D [0], texCoord + offset .wyw) .r;\n";
		string += "	float i3     = texture (x3d_Texture3D [0], texCoord - offset .wyw) .r;\n";
		string += "	float i4     = texture (x3d_Texture3D [0], texCoord + offset .wwz) .r;\n";
		string += "	float i5     = texture (x3d_Texture3D [0], texCoord - offset .wwz) .r;\n";
		string += "	vec3  n      = vec3 (i1 - i0, i3 - i2, i5 - i4);\n";
		string += "\n";
		string += "	return vec4 (normalize (x3d_TextureNormalMatrix * n), length (n));\n";
		string += "}\n";
	}

	return string;
}

void
X3DVolumeRenderStyleNode::addVolumeData (X3DVolumeDataNode* const volumeDataNode)
{
	volumeDataNodes .emplace (volumeDataNode);
}

void
X3DVolumeRenderStyleNode::removeVolumeData (X3DVolumeDataNode* const volumeDataNode)
{
	volumeDataNodes .erase (volumeDataNode);
}

} // X3D
} // titania
