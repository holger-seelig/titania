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

#include "OrientationInterpolator.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string OrientationInterpolator::componentName  = "Interpolation";
const std::string OrientationInterpolator::typeName       = "OrientationInterpolator";
const std::string OrientationInterpolator::containerField = "children";

OrientationInterpolator::Fields::Fields () :
	     keyValue (new MFRotation ()),
	value_changed (new SFRotation ())
{ }

OrientationInterpolator::OrientationInterpolator (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DInterpolatorNode (),
	             fields ()
{
	addField (inputOutput, "metadata",      metadata ());
	addField (inputOnly,   "set_fraction",  set_fraction ());
	addField (inputOutput, "key",           key ());
	addField (inputOutput, "keyValue",      keyValue ());
	addField (outputOnly,  "value_changed", value_changed ());
}

X3DBaseNode*
OrientationInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new OrientationInterpolator (executionContext);
}

void
OrientationInterpolator::initialize ()
{
	X3DInterpolatorNode::initialize ();

	keyValue () .addInterest (this, &OrientationInterpolator::set_keyValue);
}

void
OrientationInterpolator::set_keyValue ()
{
	if (keyValue () .size () < key () .size ())
		keyValue () .resize (key () .size (), keyValue () .size () ? keyValue () .back () : SFRotation ());
}

void
OrientationInterpolator::interpolate (size_t index0, size_t index1, const float weight)
{
	try
	{
		value_changed () = math::slerp <float> (keyValue () [index0], keyValue () [index1], weight);
	}
	catch (const std::domain_error & error)
	{
		std::clog << "OrientationInterpolator: " << error .what () << std::endl;
	}
}

} // X3D
} // titania
