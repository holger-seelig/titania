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

#include "SquadOrientationInterpolator.h"

#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Math/Algorithms/SquadInterpolator.h>

namespace titania {
namespace X3D {

const Component   SquadOrientationInterpolator::component      = Component ("Interpolation", 5);
const std::string SquadOrientationInterpolator::typeName       = "SquadOrientationInterpolator";
const std::string SquadOrientationInterpolator::containerField = "children";

SquadOrientationInterpolator::Fields::Fields () :
	              closed (new SFBool ()), // XXX: non standard
	            keyValue (new MFRotation ()),
	//	normalizeVelocity (new SFBool ()),
	       value_changed (new SFRotation ())
{ }

SquadOrientationInterpolator::SquadOrientationInterpolator (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DInterpolatorNode (),
	             fields (),
	              squad (new math::squad_interpolator <Rotation4d, double> ())
{
	addType (X3DConstants::SquadOrientationInterpolator);

	addField (inputOutput, "metadata",          metadata ());
	addField (inputOnly,   "set_fraction",      set_fraction ());
	addField (inputOutput, "closed",            closed ());           // XXX: non standard
	addField (inputOutput, "key",               key ());
	addField (inputOutput, "keyValue",          keyValue ());
	//	addField (inputOutput, "normalizeVelocity", normalizeVelocity ());
	addField (outputOnly,  "value_changed",     value_changed ());
}

X3DBaseNode*
SquadOrientationInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new SquadOrientationInterpolator (executionContext);
}

void
SquadOrientationInterpolator::initialize ()
{
	X3DInterpolatorNode::initialize ();

	keyValue () .addInterest (&SquadOrientationInterpolator::set_keyValue, this);
}

void
SquadOrientationInterpolator::set_keyValue ()
{
	if (keyValue () .size () < key () .size ())
		keyValue () .resize (key () .size (), keyValue () .size () ? keyValue () .back () : Rotation4d ());

	squad -> generate (closed (), key (), keyValue ());
}

void
SquadOrientationInterpolator::interpolate (size_t index0, size_t index1, const float weight)
{
	try
	{
		value_changed () = squad -> interpolate (index0, index1, weight, keyValue ());
	}
	catch (const std::domain_error &)
	{ }
}

} // X3D
} // titania
