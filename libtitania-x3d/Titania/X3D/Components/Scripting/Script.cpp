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

#include "Script.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../InputOutput/Loader.h"
#include "../../JavaScript/X3DJavaScriptEngine.h"
#include "../../Parser/RegEx.h"

namespace titania {
namespace X3D {

const std::string Script::componentName  = "Scripting";
const std::string Script::typeName       = "Script";
const std::string Script::containerField = "children";

Script::Fields::Fields () :
	directOutput (new SFBool ()),
	mustEvaluate (new SFBool ())
{ }

Script::Script (X3DExecutionContext* const executionContext) :
	  X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DScriptNode (),
	       fields (),
	   javaScript ()
{
	addField (inputOutput,    "metadata",     metadata ());
	addField (inputOutput,    "url",          url ());
	addField (initializeOnly, "directOutput", directOutput ());
	addField (initializeOnly, "mustEvaluate", mustEvaluate ());

	addChildren (javaScript);

	setExtendedEventHandling (false);
}

X3DBaseNode*
Script::create (X3DExecutionContext* const executionContext) const
{
	return new Script (executionContext);
}

void
Script::initialize ()
{
	X3DScriptNode::initialize ();

	url () .addInterest (this, &Script::set_url);

	requestImmediateLoad ();
}

void
Script::addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field)
{
	X3DScriptNode::addUserDefinedField (accessType, name, field);
	
	if (javaScript)
	{
		field -> isTainted (false);
		url () .addEvent ();
	}
}

void
Script::removeUserDefinedField (X3DFieldDefinition* const field)
{
	X3DScriptNode::removeUserDefinedField (field);

	if (javaScript)
		url () .addEvent ();
}

const MFString*
Script::getCDataField () const
{
	if (url () .empty ())
		return nullptr;

	return &url ();
}

bool
Script::loadDocument (const SFString & URL, std::string & ecmascript)
{
	if (RegEx::ECMAScript .FullMatch (URL .str (), &ecmascript))
	{
		setWorldURL (getExecutionContext () -> getWorldURL ());
		return true;
	}

	try
	{
		Loader loader (getExecutionContext ());
		ecmascript = std::move (loader .loadDocument (URL));
		setWorldURL (loader .getWorldURL ());
		return true;
	}
	catch (const X3DError &)
	{ }

	return false;
}

void
Script::requestImmediateLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	// Find first working script.

	size_t index = 0;

	for (const auto & URL : url ())
	{
		std::string ecmascript;

		if (loadDocument (URL, ecmascript))
		{
			try
			{
				javaScript .set (getBrowser () -> getJavaScriptEngine () -> createContext (this, ecmascript, getWorldURL (), index));
				break;
			}
			catch (const std::invalid_argument & error)
			{
				std::clog << error .what () << std::endl;
			}
		}

		++ index;
	}

	if (javaScript)
	{
		addInterest (this, &Script::eventsProcessed);

		setLoadState (COMPLETE_STATE);
	}
	else
	{
		try
		{
			// Assign an empty script if no working script is found.
			javaScript .set (getBrowser () -> getJavaScriptEngine () -> createContext (this, "", "", 0));
		}
		catch (const std::invalid_argument & error)
		{ }

		removeInterest (this, &Script::eventsProcessed);

		setLoadState (FAILED_STATE);
	}

	// Initialize.

	javaScript -> setup ();
}

void
Script::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	requestImmediateLoad ();
}

void
Script::eventsProcessed ()
{
	javaScript -> eventsProcessed ();
}

void
Script::saveState ()
{
	if (isSaved ())
		return;

	X3DScriptNode::saveState ();

	javaScript = getBrowser () -> getJavaScriptEngine () -> createContext (this, "", "", 0);

	setLoadState (NOT_STARTED_STATE);
}

void
Script::restoreState ()
{
	if (not isSaved ())
		return;

	X3DScriptNode::restoreState ();

	requestImmediateLoad ();
}

} // X3D
} // titania
