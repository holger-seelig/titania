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

#include "SceneLoader.h"

#include "../Browser/X3DBrowser.h"
#include "../Execution/X3DScene.h"

#include <sys/types.h>
#include <unistd.h>

namespace titania {
namespace X3D {

// See http://www.web3d.org/files/specifications/19775-1/V3.3/Part01/components/networking.html#X3DUrlObject for
// how to handle the profile and component arguments/statements of inline nodes.

const ComponentType SceneLoader::component      = ComponentType::TITANIA;
const std::string   SceneLoader::typeName       = "SceneLoader";
const std::string   SceneLoader::containerField = "future";

SceneLoader::SceneLoader (X3DExecutionContext* const executionContext, const MFString & url, const SceneLoaderCallback & callback) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DFuture (),
	  callback (callback),
	    loader (nullptr, executionContext -> getWorldURL ()),
	     scene (),
	  urlError (),
	    future (getFuture (url /*, executionContext -> getProfile (), executionContext -> getComponents () */))
{
	getBrowser () -> prepareEvents () .addInterest (&SceneLoader::set_scene, this, true);
	getBrowser () -> addEvent ();
}

X3DBaseNode*
SceneLoader::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("SceneLoader::create");
}

std::future <X3DScenePtr> 
SceneLoader::getFuture (const MFString & url)
{
	return std::async (std::launch::async, std::mem_fn (&SceneLoader::loadAsync), this, url);
}

void
SceneLoader::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const bool prepareEvents = getBrowser () -> prepareEvents () .hasInterest (&SceneLoader::set_scene, this);

	getBrowser () -> prepareEvents () .removeInterest (&SceneLoader::set_scene, this);

	X3DFuture::setExecutionContext (executionContext);

	if (prepareEvents)
	{
		getBrowser () -> prepareEvents () .addInterest (&SceneLoader::set_scene, this, true);

		getBrowser () -> addEvent ();
	}
}

void
SceneLoader::wait ()
{
	try
	{
		checkForInterrupt ();

		if (not future .valid ())
		{
			stop ();
			return;
		}

		future .wait ();

		scene = future .get ();

		scene -> requestImmediateLoadOfExternProtos ();

		callback (std::move (scene));

		dispose ();
	}
	catch (const InterruptThreadException &)
	{
	   // Interrupt
	}
	catch (const std::exception & error)
	{
		urlError = { error .what () };

		callback (nullptr);

		dispose ();
	}
}

X3DScenePtr
SceneLoader::loadAsync (const MFString & url)
{
	try
	{
		checkForInterrupt ();
	
		const auto mutex = getBrowser () -> getDownloadMutex ();
	
		checkForInterrupt ();
	
		std::lock_guard <std::mutex> lock (*mutex);
	
		checkForInterrupt ();
	
		const auto scene = getBrowser () -> createScene (false);
	
		checkForInterrupt ();
	
		loader .parseIntoScene (scene, url);
			
		checkForInterrupt ();
	
		getBrowser () -> println ("Done loading scene '", loader .getWorldURL (), "'.");
			
		checkForInterrupt ();
	
		return scene;
	}
	catch (const InterruptThreadException &)
	{
		throw;
	}
	catch (const std::exception & error)
	{
		checkForInterrupt ();

		getBrowser () -> println (error .what ());

		throw;
	}
}

void
SceneLoader::set_scene (const bool addEvent)
{
	try
	{
		checkForInterrupt ();
	
		if (addEvent)
			getBrowser () -> addEvent ();
	
		if (not future .valid ())
			return;
	
		const auto status = future .wait_for (std::chrono::milliseconds (0));
	
		if (status not_eq std::future_status::ready)
			return;
	
		getBrowser () -> prepareEvents () .removeInterest (&SceneLoader::set_scene, this);
	
		scene = future .get ();

		scene -> getExternProtosLoadCount () .addInterest (&SceneLoader::set_loadCount, this);

		scene -> requestAsyncLoadOfExternProtos ();
	}
	catch (const InterruptThreadException &)
	{
	   // Interrupt
	}
	catch (const std::exception & error)
	{
		urlError = { error .what () };

		callback (nullptr);

		dispose ();;
	}
}

void
SceneLoader::set_loadCount (const int32_t loadCount)
{
	try
	{
		checkForInterrupt ();

		if (loadCount)
		   return;

		scene -> getExternProtosLoadCount () .removeInterest (&SceneLoader::set_loadCount, this);
	
		callback (std::move (scene));

		dispose ();
	}
	catch (const InterruptThreadException &)
	{
	   // Interrupt
	}
}

void
SceneLoader::dispose ()
{
	stop ();

	loader .stop ();

	scene .dispose ();

	X3DFuture::dispose ();

	// This is very important, otherwise not all nodes do dispose as they could be bound in the callback!
	callback = [ ] (X3DScenePtr &&) { };
}

SceneLoader::~SceneLoader ()
{
	if (future .valid ())
		future .wait ();
}

} // X3D
} // titania
