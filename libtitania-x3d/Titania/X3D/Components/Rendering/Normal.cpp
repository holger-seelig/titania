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

#include "Normal.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   Normal::component      = Component ("Rendering", 2);
const std::string Normal::typeName       = "Normal";
const std::string Normal::containerField = "normal";

Normal::Fields::Fields () :
	vector (new MFVec3f ())
{ }

Normal::Normal (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DNormalNode (),
	       fields ()
{
	addType (X3DConstants::Normal);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "vector",   vector ());
}

X3DBaseNode*
Normal::create (X3DExecutionContext* const executionContext) const
{
	return new Normal (executionContext);
}

void
Normal::set1Vector (const size_t index, const Vector3f & value)
{
	vector () .set1Value (index, value);
}

Vector3f
Normal::get1Vector (const size_t index) const
{
	if (index < vector () .size ())
		return vector () [index];

	else if (vector () .size ())
		return vector () [index % vector () .size ()];

	return Vector3f (0, 0, 0);
}

void
Normal::addVector (std::vector <Vector3f> & normals, const size_t index) const
{
	if (index < vector () .size ())
		normals .emplace_back (vector () [index]);

	else if (vector () .size ())
		normals .emplace_back (vector () [index % vector () .size ()]);

	else
		normals .emplace_back (0, 0, 0);
}

void
Normal::assignVectors (const std::vector <Vector3f> & vectors)
{
	vector () .assign (vectors .begin (), vectors .end ());
}

void
Normal::eraseVector (const size_t index)
{
	if (index < vector () .size ())
		vector () .erase (vector () .begin () + index);
}

} // X3D
} // titania
