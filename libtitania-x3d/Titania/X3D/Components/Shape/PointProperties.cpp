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

#include "PointProperties.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

namespace titania {
namespace X3D {

const Component   PointProperties::component      = Component ("Shape", 5);
const std::string PointProperties::typeName       = "PointProperties";
const std::string PointProperties::containerField = "pointProperties";

PointProperties::Fields::Fields () :
	pointSizeScaleFactor (new SFFloat (1)),
	   pointSizeMinValue (new SFFloat (1)),
	   pointSizeMaxValue (new SFFloat (1)),
	pointSizeAttenuation (new MFFloat ({ 1, 0, 0 })),
	           colorMode (new SFString ("TEXTURE_AND_POINT_COLOR"))
{ }

PointProperties::PointProperties (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	   X3DAppearanceChildNode (),
	                   fields (),
	pointSizeAttenuationValue (),
	            colorModeType ()
{
	addType (X3DConstants::PointProperties);

	addField (inputOutput, "metadata",             metadata ());
	addField (inputOutput, "pointSizeScaleFactor", pointSizeScaleFactor ());
	addField (inputOutput, "pointSizeMinValue",    pointSizeMinValue ());
	addField (inputOutput, "pointSizeMaxValue",    pointSizeMaxValue ());
	addField (inputOutput, "pointSizeAttenuation", pointSizeAttenuation ());
	addField (inputOutput, "colorMode",            colorMode ());
}

X3DBaseNode*
PointProperties::create (X3DExecutionContext* const executionContext) const
{
	return new PointProperties (executionContext);
}

void
PointProperties::initialize ()
{
	X3DAppearanceChildNode::initialize ();

	pointSizeAttenuation () .addInterest (&PointProperties::set_pointSizeAttenuation, this);
	colorMode ()            .addInterest (&PointProperties::set_colorMode,            this);

	set_pointSizeAttenuation ();
	set_colorMode ();
}

void
PointProperties::set_pointSizeAttenuation ()
{
	pointSizeAttenuationValue [0] = pointSizeAttenuation () .size () > 0 ? std::max <float> (0, pointSizeAttenuation () [0]) : 1.0f;
	pointSizeAttenuationValue [1] = pointSizeAttenuation () .size () > 1 ? std::max <float> (0, pointSizeAttenuation () [1]) : 0.0f;
	pointSizeAttenuationValue [2] = pointSizeAttenuation () .size () > 2 ? std::max <float> (0, pointSizeAttenuation () [2]) : 0.0f;
}

void
PointProperties::set_colorMode ()
{
	try
	{
		static const std::map <std::string, ColorModeType> colorModes ({
			std::pair ("POINT_COLOR",             ColorModeType::POINT_COLOR),
			std::pair ("TEXTURE_COLOR",           ColorModeType::TEXTURE_COLOR),
			std::pair ("TEXTURE_AND_POINT_COLOR", ColorModeType::TEXTURE_AND_POINT_COLOR),
		});

		colorModeType = colorModes .at (colorMode ());
	}
	catch(const std::out_of_range & error)
	{
		colorModeType = ColorModeType::TEXTURE_AND_POINT_COLOR;
	}
}

void
PointProperties::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject) const
{
	glUniform1f  (shaderObject -> getPointPropertiesPointSizeScaleFactorUniformLocation (), std::max <float> (1, pointSizeScaleFactor ()));
	glUniform1f  (shaderObject -> getPointPropertiesPointSizeMinValueUniformLocation (),    std::max <float> (0, pointSizeMinValue ()));
	glUniform1f  (shaderObject -> getPointPropertiesPointSizeMaxValueUniformLocation (),    std::max <float> (0, pointSizeMaxValue ()));
	glUniform3fv (shaderObject -> getPointPropertiesPointSizeAttenuationUniformLocation (), 1, pointSizeAttenuationValue .data ());
	glUniform1i  (shaderObject -> getPointPropertiesColorModeUniformLocation (),            int32_t (colorModeType));
}

PointProperties::~PointProperties ()
{ }

} // X3D
} // titania
