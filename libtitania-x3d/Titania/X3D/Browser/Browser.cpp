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

#include "Browser.h"

#include "../Browser/ContextLock.h"
#include "../Browser/KeyDeviceSensor/KeyDevice.h"
#include "../Browser/Navigation/ExamineViewer.h"
#include "../Browser/Navigation/FlyViewer.h"
#include "../Browser/Navigation/LookAtViewer.h"
#include "../Browser/Navigation/NoneViewer.h"
#include "../Browser/Navigation/PlaneViewer.h"
#include "../Browser/Navigation/WalkViewer.h"
#include "../Browser/Navigation/X3DViewer.h"
#include "../Browser/PointingDeviceSensor/PointingDevice.h"
#include "../Browser/Rendering/BackgroundTexture.h"
#include "../Browser/Tools/LightSaber.h"
#include "../Browser/Tools/LassoSelection.h"
#include "../Browser/Tools/RectangleSelection.h"
#include "../Execution/World.h"

#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Navigation/NavigationInfo.h"

#include <glibmm/main.h>
#include <gtkmm/container.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

#include "../Fields/X3DWeakPtrArray.h"

namespace titania {
namespace X3D {

Browser::Browser (const MFString & url, const MFString & parameter) :
	    X3DBaseNode (this, this),
	     X3DBrowser (url, parameter),
	opengl::Surface (),
	        viewer  (new NoneViewer (this)),
	      keyDevice (new KeyDevice (this)),
	pointingDevice  (new PointingDevice (this)),
	         cursor ("default"),
	     background (new BackgroundTexture ())
{
	__LOG__ << "Constructing browser " << this << "." << std::endl;

	addType (X3DConstants::Browser);

	addChildObjects (viewer,
	                 keyDevice,
	                 pointingDevice,
	                 cursor);
}

Browser::Browser (const Browser & other, const MFString & url, const MFString & parameter) :
	    X3DBaseNode (this, this),
	     X3DBrowser (url, parameter),
	opengl::Surface (other),
	        viewer  (new NoneViewer (this)),
	      keyDevice (new KeyDevice (this)),
	pointingDevice  (new PointingDevice (this)),
	         cursor ("default"),
	     background (new BackgroundTexture ())
{
	addType (X3DConstants::Browser);

	addChildObjects (viewer,
	                 keyDevice,
	                 pointingDevice,
	                 cursor);
}

Browser*
Browser::create (X3DExecutionContext* const executionContext) const
{
	return new Browser ({ }, { });
}

void
Browser::initialize ()
{
	try
	{
		ContextLock lock (this);

		opengl::Surface::initialize ();
		X3DBrowser::initialize ();
	
		get_style_context () -> add_class ("background");
		get_style_context () -> add_class ("titania-surface");
	
		//swapInterval (0);
	
		viewer         -> setup ();
		keyDevice      -> setup ();
		pointingDevice -> setup ();
		background     -> setup ();
	
		getCursor ()        .addInterest (&Browser::set_cursor, this);
		getViewerType ()    .addInterest (&Browser::set_viewer, this);
		getPrivateViewer () .addInterest (&Browser::set_viewer, this);
		changed ()          .addInterest (std::bind (&Glib::SignalIdle::connect_once, Glib::signal_idle (), sigc::mem_fun (this, &Gtk::Widget::queue_draw), Glib::PRIORITY_DEFAULT_IDLE));
	
		add_events (Gdk::BUTTON_PRESS_MASK |
		            Gdk::POINTER_MOTION_MASK |
		            Gdk::POINTER_MOTION_HINT_MASK |
		            Gdk::BUTTON_RELEASE_MASK |
		            Gdk::FOCUS_CHANGE_MASK |
		            Gdk::LEAVE_NOTIFY_MASK |
		            Gdk::SCROLL_MASK |
		            Gdk::KEY_PRESS_MASK |
		            Gdk::KEY_RELEASE_MASK);
	
		set_can_focus (true);
		//set_focus_on_click (true); // uncomment if gtkmm 3.20 and see PointingDevice button press event
		grab_focus ();
	
		setCursor ("default");
	}
	catch (const std::exception & exception)
	{ }
}

void
Browser::on_style_updated ()
{
	try
	{
		ContextLock lock (this);

		opengl::Surface::on_style_updated ();

		background -> configure (get_style_context (), get_width (), get_height ());
	}
	catch (const std::exception & exception)
	{ }
}

void
Browser::on_map ()
{
	opengl::Surface::on_map ();

	if (isLive ())
		getExecutionContext () -> beginUpdate ();

	grab_focus ();
	queue_draw ();
}

void
Browser::on_unmap ()
{
	if (isLive ())
	{
		getClock () -> advance ();

		getExecutionContext () -> endUpdate ();

		try
		{
			ContextLock lock (this);
		
			processEvents ();
		}
		catch (const Error <INVALID_OPERATION_TIMING> &)
		{ }
	}

	opengl::Surface::on_unmap ();
}

void
Browser::set_cursor (const String & value)
{
	get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), value));
}

void
Browser::set_viewer ()
{
	const auto type = getCurrentViewer ();

	setCursor ("default");

	viewer .addEvent ();

	if (type not_eq viewer -> getType () .back ())
	{
		switch (type)
		{
			case X3DConstants::NoneViewer:
			{
				viewer .setValue (new NoneViewer (this));
				break;
			}
			case X3DConstants::ExamineViewer:
			{
				viewer .setValue (new ExamineViewer (this));
				break;
			}
			case X3DConstants::WalkViewer:
			{
				viewer .setValue (new WalkViewer (this));
				break;
			}
			case X3DConstants::FlyViewer:
			{
				viewer .setValue (new FlyViewer (this));
				break;
			}
			case X3DConstants::PlaneViewer:
			{
				viewer .setValue (new PlaneViewer (this));
				break;
			}
			case X3DConstants::LookAtViewer:
			{
				viewer .setValue (new LookAtViewer (this));
				break;
			}
			case X3DConstants::RectangleSelection:
			{
				viewer .setValue (new RectangleSelection (this));
				break;
			}
			case X3DConstants::LassoSelection:
			{
				viewer .setValue (new LassoSelection (this));
				break;
			}
			case X3DConstants::LightSaber:
			{
				viewer .setValue (new LightSaber (this));
				break;
			}
			default:
				viewer .setValue (new NoneViewer (this));
				break;
		}

		viewer -> setup ();
	}
}

bool
Browser::makeCurrent ()
noexcept (true)
{
	return opengl::Surface::makeCurrent ();
}

void
Browser::reshape (const Vector4i & viewport)
noexcept (true)
{
	try
	{
		ContextLock lock (this);

		background -> configure (get_style_context (), get_width (), get_height ());
	
		X3DBrowser::reshape (viewport);
	}
	catch (const std::exception & exception)
	{ }
}

void
Browser::renderBackground ()
{
	background -> draw ();
}

void
Browser::update ()
noexcept (true)
{
	X3DBrowser::update ();
}

void
Browser::dispose ()
{
	X3DBrowser::dispose ();

	opengl::Surface::dispose ();
}

Browser::~Browser ()
{ }

} // X3D
} // titania
