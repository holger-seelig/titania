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

#include "CoordinateDamper.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string CoordinateDamper::componentName  = "Followers";
const std::string CoordinateDamper::typeName       = "CoordinateDamper";
const std::string CoordinateDamper::containerField = "children";

CoordinateDamper::Fields::Fields () :
	         set_value (new MFVec3f ()),
	   set_destination (new MFVec3f ()),
	      initialValue (new MFVec3f ({ SFVec3f () })),
	initialDestination (new MFVec3f ({ SFVec3f () })),
	     value_changed (new MFVec3f ())
{ }

CoordinateDamper::CoordinateDamper (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DDamperNode (),
	       fields (),
	        value ()
{
	addField (inputOutput,    "metadata",           metadata ());
	addField (inputOnly,      "set_value",          set_value ());
	addField (inputOnly,      "set_destination",    set_destination ());
	addField (initializeOnly, "initialValue",       initialValue ());
	addField (initializeOnly, "initialDestination", initialDestination ());
	addField (initializeOnly, "order",              order ());
	addField (inputOutput,    "tau",                tau ());
	addField (inputOutput,    "tolerance",          tolerance ());
	addField (outputOnly,     "isActive",           isActive ());
	addField (outputOnly,     "value_changed",      value_changed ());
}

X3DBaseNode*
CoordinateDamper::create (X3DExecutionContext* const executionContext) const
{
	return new CoordinateDamper (executionContext);
}

void
CoordinateDamper::initialize ()
{
	X3DDamperNode::initialize ();

	set_value ()       .addInterest (this, &CoordinateDamper::_set_value);
	set_destination () .addInterest (this, &CoordinateDamper::_set_destination);
	order ()           .addInterest (this, &CoordinateDamper::set_order);

	value .resize (getOrder () + 1);

	for (auto & v : basic::adapter (value .begin () + 1, value .end ()))
	{
		v .assign (initialValue () .begin (), initialValue () .end ());
		v .resize (initialDestination () .size ());
	}

	value [0] .assign (initialDestination () .begin (), initialDestination () .end ());

	set_active (not equals (initialDestination (), initialValue (), getTolerance ()));
}

void
CoordinateDamper::_set_value ()
{
	for (auto & v : basic::adapter (value .begin () + 1, value .end ()))
		v .assign (set_value () .begin (), set_value () .end ());

	value [0] .resize (set_value () .size ());

	value_changed () = set_value ();

	set_active (true);
}
 
void
CoordinateDamper::_set_destination ()
{
	if (not equals (value [0], set_destination (), getTolerance ()))
	{
		for (auto & v : basic::adapter (value .begin () + 1, value .end ()))
			v .resize (set_destination () .size ());

		value [0] .assign (set_destination () .begin (), set_destination () .end ());

		set_active (true);
	}
}

void
CoordinateDamper::set_order ()
{
	value .resize (getOrder () + 1, value .back ());
}

void
CoordinateDamper::prepareEvents ()
{
	size_t order = value .size () - 1;

	if (tau ())
	{
		time_type delta = 1 / getBrowser () -> getCurrentFrameRate ();

		float alpha = std::exp (-delta / tau ());
		
		for (size_t i = 0; i < order; ++ i)
		{
			for (size_t j = 0, s = value [i] .size (); j < s; ++ j)
			{
				value [i + 1] [j] = lerp (value [i] [j], value [i + 1] [j], alpha);
			}
		}

		value_changed () .assign (value [order] .begin (), value [order] .end ());

		if (not equals (value [order], value [0], getTolerance ()))
			return;
	}
	else
	{
		value_changed () .assign (value [0] .begin (), value [0] .end ());
		
		order = 0;
	}

	for (auto & v : basic::adapter (value .begin () + 1, value .end ()))
		v = value [order];

	set_active (false);
}

} // X3D
} // titania
