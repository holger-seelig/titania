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

#include "Browser.h"

#include "../Browser/Viewer/ExamineViewer.h"
#include "../Browser/Viewer/FlyViewer.h"
#include "../Browser/Viewer/NoneViewer.h"
#include "../Browser/Viewer/WalkViewer.h"
#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Navigation/NavigationInfo.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

namespace titania {
namespace X3D {

Browser::Browser () :
	    X3DBaseNode (this, this),
	opengl::Surface (),
	     X3DBrowser (),
	        viewer  (new NoneViewer (this)),
	pointingDevice  (this)
{
	add_events (Gdk::BUTTON_PRESS_MASK |
	            Gdk::POINTER_MOTION_MASK |
	            Gdk::POINTER_MOTION_HINT_MASK |
	            Gdk::BUTTON_RELEASE_MASK |
	            Gdk::SCROLL_MASK |
	            Gdk::KEY_PRESS_MASK |
	            Gdk::KEY_RELEASE_MASK);

	set_can_focus (true);

	changed .addInterest (static_cast <Gtk::Widget*> (this), &Browser::queue_draw);
}

X3DBaseNode*
Browser::create (X3DExecutionContext* const) const
{
	return new Browser ();
}

void
Browser::construct ()
{
	setup ();

	setCursor (Gdk::ARROW);
}

void
Browser::setViewer (ViewerType type, NavigationInfo* navigationInfo)
{
	if (viewer -> getType () not_eq type or viewer -> getNavigationInfo () not_eq navigationInfo)
	{
		switch (type)
		{
			case ViewerType::NONE:
			{
				viewer .reset (new NoneViewer (this));
				break;
			}
			case ViewerType::FLY:
			{
				viewer .reset (new FlyViewer (this, navigationInfo));
				break;
			}
			case ViewerType::EXAMINE:
			{
				viewer .reset (new ExamineViewer (this, navigationInfo));
				break;
			}
			case ViewerType::WALK:
			{
				viewer .reset (new WalkViewer (this, navigationInfo));
				break;
			}
		}

		viewer -> setup ();
	}
}

void
Browser::set_navigationInfo ()
{
	NavigationInfo* navigationInfo = getActiveNavigationInfo ();

	if (navigationInfo)
	{
		for (const auto & type : navigationInfo -> type ())
		{
			if (type == "NONE")
			{
				setViewer (ViewerType::NONE, nullptr);
				goto END;
			}

			else if (type == "WALK")
			{
				setViewer (ViewerType::WALK, navigationInfo);
				goto END;
			}

			else if (type == "FLY")
			{
				setViewer (ViewerType::FLY, navigationInfo);
				goto END;
			}

			else if (type == "LOOKAT")
			{ }

			else if (type == "ANY")
			{ }

			else
			{
				setViewer (ViewerType::EXAMINE, navigationInfo);
				goto END;
			}
		}

		setViewer (ViewerType::EXAMINE, navigationInfo);

END:;
	}
	else
		setViewer (ViewerType::NONE, nullptr);
}

void
Browser::reshape ()
{
	reshaped .processInterests ();
}

void
Browser::dispose ()
{
	viewer .reset ();
	pointingDevice .dispose ();

	opengl::Surface::dispose ();
	X3DBrowser::dispose ();
}

} // X3D
} // titania
