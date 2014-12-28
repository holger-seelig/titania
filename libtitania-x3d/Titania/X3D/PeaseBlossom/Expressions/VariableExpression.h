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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_VARIABLE_EXPRESSION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_VARIABLE_EXPRESSION_H__

#include "../Execution/pbExecutionContext.h"
#include "../Expressions/pbExpression.h"
#include "../Objects/pbFunction.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript identifier expression.
 */
class VariableExpression :
	public pbExpression
{
public:

	///  @name Construction

	///  Constructs new VariableExpression expression.
	VariableExpression (pbExecutionContext* const executionContext, std::string && identifier) :
		    pbExpression (),
		executionContext (executionContext),
		      identifier (std::move (identifier)),
		              id (getId (this -> identifier))
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbBaseObject>
	copy (pbExecutionContext* const executionContext) const
	throw (pbException,
	       pbControlFlowException) final override
	{ return new VariableExpression (executionContext, std::string (identifier)); }

	///  @name Operations

	virtual
	var
	setValue (var && value) const
	throw (pbException) final override
	{
		return setProperty (executionContext, std::move (value));
	}

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	getValue () const
	throw (pbException,
	       pbControlFlowException) final override
	{
		return getProperty (executionContext);
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChild (executionContext); }

	///  @name Operations

	var
	setProperty (const ptr <pbExecutionContext> & executionContext, var && value) const
	throw (pbException)
	{
		for (const auto & localObject : executionContext -> getLocalObjects ())
		{
			try
			{
				return localObject -> setProperty (id, std::move (value));
			}
			catch (const std::out_of_range &)
			{ }
		}

		if (executionContext -> isRootContext ())
		{
			executionContext -> getGlobalObject () -> addPropertyDescriptor (id, identifier, value);

			return value;
		}

		return setProperty (executionContext -> getExecutionContext (), std::move (value));
	}

	var
	getProperty (const ptr <pbExecutionContext> & executionContext) const
	throw (pbException)
	{
		for (const auto & localObject : executionContext -> getLocalObjects ())
		{
			try
			{
				return localObject -> getProperty (id);
			}
			catch (const std::out_of_range &)
			{ }
		}

		if (executionContext -> isRootContext ())
			throw ReferenceError (identifier + " is not defined.");

		return getProperty (executionContext -> getExecutionContext ());
	}

	///  @name Members

	const ptr <pbExecutionContext> executionContext;
	const std::string              identifier;
	const size_t                   id;

};

} // pb
} // titania

#endif
