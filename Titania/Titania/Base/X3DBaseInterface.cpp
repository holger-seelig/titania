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

#include "X3DBaseInterface.h"

#include <Titania/X3D/Browser/X3DBrowser.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Execution/X3DExecutionContext.h>

#include "../Browser/X3DBrowserWindow.h"

#include <Titania/String.h>

#include <cassert>

namespace titania {
namespace puck {

///  Constructor.
X3DBaseInterface::X3DBaseInterface (X3DBrowserWindow* const browserWindow, X3D::Browser* const browser) :
	     sigc::trackable (),
	X3D::X3DParentObject (browser),
	            typeName (),
	       browserWindow (browserWindow)
{
	assert (browserWindow);
}

///  Dummy constructor.
X3DBaseInterface::X3DBaseInterface () :
	     sigc::trackable (),
	X3D::X3DParentObject (nullptr),
	            typeName (),
	       browserWindow (nullptr)
{
	assert (false);
}

///  Setups this object.
void
X3DBaseInterface::setup ()
{
	X3D::X3DParentObject::setup ();

	browserWindow -> getCurrentBrowser () .addInterest (&X3DBaseInterface::set_browser, this);
}

///  Return the master browser.
const X3D::BrowserPtr &
X3DBaseInterface::getMasterBrowser () const
{
	return browserWindow -> getMasterBrowser ();
}

///  Return the current browser.
const X3D::BrowserPtr &
X3DBaseInterface::getCurrentBrowser () const
{
	return browserWindow -> getCurrentBrowser ();
}

///  Return the current world.
const X3D::WorldPtr &
X3DBaseInterface::getCurrentWorld () const
{
	return getCurrentBrowser () -> getWorld ();
}

///  Return the current scene.
const X3D::X3DScenePtr &
X3DBaseInterface::getCurrentScene () const
{
	return browserWindow -> getCurrentScene ();
}

///  Sets the current execution context.
void
X3DBaseInterface::setCurrentContext (const X3D::X3DExecutionContextPtr & executionContext)
{
	return browserWindow -> setCurrentContext (executionContext);
}

///  Return the current execution context.
const X3D::X3DExecutionContextPtr &
X3DBaseInterface::getCurrentContext () const
{
	return browserWindow -> getCurrentContext ();
}

///  Return true if the current execution context is a prototype.
bool
X3DBaseInterface::inProtoDeclaration () const
{
	return getCurrentContext () -> isType ({ X3D::X3DConstants::ProtoDeclaration });
}

///  Return true if the current execution context is a prototype instance.
bool
X3DBaseInterface::inPrototypeInstance () const
{
	static const X3D::NodeTypeSet protoInstance = { X3D::X3DConstants::X3DPrototypeInstance };

	return getCurrentContext () -> isType (protoInstance);
}

void
X3DBaseInterface::setAddWorldInfo (const X3D::X3DScenePtr & scene, const bool value)
{
	if (value)
		scene -> setMetaData ("titania-add-world-info", "true");
	else
		scene -> removeMetaData ("titania-add-world-info");
}

bool
X3DBaseInterface::getAddWorldInfo (const X3D::X3DScenePtr & scene)
{
	try
	{
		//Apr 30 2017 01:12:11
		//META "modified" "Fri, 28 Apr 2017 07:18:21 GMT"

		__LOG__ << std::endl;
		__LOG__ << __DATE__ << std::endl;
		__LOG__ << scene -> getMetaData ("modified") << std::endl;

		X3D::SFTime compiled;
		X3D::SFTime modified;

		compiled .fromLocaleString (__DATE__, "%b %d %Y", std::locale::classic ());
		modified .fromLocaleString (scene -> getMetaData ("modified"), "%a, %d %b %Y %H:%M:%S GMT", std::locale::classic ());

		__LOG__ << std::endl;
		__LOG__ << compiled << " : " << compiled .toUTCString () << std::endl; 
		__LOG__ << modified << " : " << modified .toUTCString () << std::endl;
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{ }

	try
	{
		const auto addWorldInfo = basic::tolower (scene -> getMetaData ("titania-add-world-info"), std::locale::classic ());

		return addWorldInfo == "true";
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		return false;
	}
}

///  Return the WorldInfo node from the current scene. The node is created if needed.
X3D::X3DPtr <X3D::WorldInfo>
X3DBaseInterface::createWorldInfo ()
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	try
	{
		return getWorldInfo (getAddWorldInfo (getCurrentScene ()));
	}
	catch (const X3D::X3DError &)
	{
		return X3D::MakePtr <X3D::WorldInfo> (getCurrentScene ());
	}
}

///  Return the WorldInfo node from the current scene, otherwise it throws an exception.
X3D::X3DPtr <X3D::WorldInfo>
X3DBaseInterface::getWorldInfo () const
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	return const_cast <X3DBaseInterface*> (this) -> getWorldInfo (false);
}

///  Return the WorldInfo node from the current scene. If @a create is true, the node is created if needed, otherwise it
///  throws an exception.
X3D::X3DPtr <X3D::WorldInfo>
X3DBaseInterface::getWorldInfo (const bool create)
throw (X3D::Error <X3D::NOT_SUPPORTED>)
{
	auto worldInfo = getCurrentScene () -> getWorldInfo ();

	if (not worldInfo)
	{
		if (not create)
			throw X3D::Error <X3D::NOT_SUPPORTED> ("X3DBaseInterface::getWorldInfo: not supported.");
	
		worldInfo = getCurrentScene () -> createNode <X3D::WorldInfo> ();
		worldInfo -> title () = getCurrentScene () -> getWorldURL () .basename (false);

		getCurrentScene () -> getRootNodes () .emplace_front (worldInfo);
	}

	return worldInfo;
}

///  Adds @a undoStep to the current browser.
void
X3DBaseInterface::addUndoStep (const X3D::UndoStepPtr & undoStep)
{
	browserWindow -> addUndoStep (undoStep);
}

///  Removes the last undo step from the current browser.
void
X3DBaseInterface::removeUndoStep ()
{
	browserWindow -> removeUndoStep ();
}

///  Returns the last undo step from the current browser.
const X3D::UndoStepPtr &
X3DBaseInterface::getUndoStep () const
{
	return browserWindow -> getUndoStep ();
}

std::string
X3DBaseInterface::getNodeName (const X3D::SFNode & node) const
{
	const auto nodeName = node -> getName () .empty () ? _ ("<unnamed>") : node -> getName ();

	return nodeName;
}

std::deque <std::string>
X3DBaseInterface::getNodePath (const X3D::SFNode & node) const
{
	auto path = getProtoPath (X3D::X3DExecutionContextPtr (node -> getExecutionContext ()));

	path .emplace_back (getNodeName (node));

	return path;
}

std::deque <std::string>
X3DBaseInterface::getProtoPath (X3D::X3DExecutionContext* executionContext) const
{
	auto path = std::deque <std::string> ();

	while (executionContext -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
	{
		path .emplace_front (executionContext -> getName ());

		executionContext = executionContext -> getExecutionContext ();
	}

	return path;
}

std::deque <std::string>
X3DBaseInterface::getContextPath (X3D::X3DExecutionContext* executionContext) const
{
	auto path = std::deque <std::string> ();

	const auto masterScene = executionContext -> getMasterScene ();

	while (executionContext not_eq masterScene)
	{
		if (executionContext -> isType ({ X3D::X3DConstants::X3DScene }))
		{
			path .emplace_front (executionContext -> getWorldURL ());

			executionContext = executionContext -> getExecutionContext ();
		}
		else if (executionContext -> isType ({ X3D::X3DConstants::ProtoDeclaration }))
		{
			const auto protoPath = getProtoPath (executionContext);

			path .emplace_front (basic::join (protoPath .begin (), protoPath .end (), "."));

			executionContext = executionContext -> getScene ();
		}
		else
		{
			// X3DPrototypeInstance

			path .clear ();

			executionContext = executionContext -> getScene ();
		}
	}

	path .emplace_front (masterScene -> getWorldURL ());

	return path;
}

std::string
X3DBaseInterface::getPathFromContext (const X3D::X3DExecutionContextPtr & executionContext) const
{
	const auto contextPath = getContextPath (executionContext);
	const auto string      = basic::join (contextPath .begin (), contextPath .end (), "\n");

	return string;
}

X3D::X3DExecutionContextPtr
X3DBaseInterface::getContextFromPath (X3D::X3DExecutionContext* executionContext, const std::string & string) const
{
	// Restore context from path.

	auto contextPath = std::deque <std::string> ();

	basic::split (std::back_inserter (contextPath), string, "\n");

	if (contextPath .empty () or contextPath .front () not_eq executionContext -> getWorldURL ())
		return X3D::X3DExecutionContextPtr (executionContext);

	contextPath .pop_front ();

	for (const basic::uri path : contextPath)
	{
		if (path .is_absolute ())
		{
			// Exterproto or Inline
			break;
		}
		else
		{
			// Proto

			auto protoNames = std::vector <std::string> ();
	
			basic::split (std::back_inserter (protoNames), path .str (), ".");

			for (const auto & protoName : protoNames)
			{
				try
				{
					executionContext = executionContext -> getProtoDeclaration (protoName);
				}
				catch (const X3D::X3DError &)
				{
					return X3D::X3DExecutionContextPtr (executionContext);
				}
			}
		}
	}

	return X3D::X3DExecutionContextPtr (executionContext);
}

///  Sets the current browser to the underlying X3DParentObject.
void
X3DBaseInterface::set_browser (const X3D::BrowserPtr & value)
{
	setBrowser (value);
}

///  Disposed this on_show_all_objects_activated.
void
X3DBaseInterface::dispose ()
{
	notify_callbacks ();

	X3D::X3DParentObject::dispose ();
}

/// Destructor
X3DBaseInterface::~X3DBaseInterface ()
{ }

} // puck
} // titania
