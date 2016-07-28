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

#include "Browser/BrowserWindow.h"

#include "Configuration/config.h"

#include <Titania/OS/env.h>
#include <Titania/X3D.h>

namespace titania {
namespace puck {

class BrowserApplication :
	public Gtk::Application
{
public:

	///  @name Construction

	BrowserApplication (int & argc, char** & argv) :
		Gtk::Application (argc, argv, "de.create3000.titania", Gio::APPLICATION_HANDLES_OPEN),
		   browserWindow ()
	{
		Glib::set_application_name ("Titania");
	}


private:

	///  @name Operations

	void
	realize ()
	{
		browserWindow .reset (new BrowserWindow (X3D::createBrowser ({ get_ui ("Logo.x3dv") })));

		add_window (browserWindow -> getWindow ());

		browserWindow -> getWindow () .present ();
	}

	///  @name Event handlers

	virtual
	void
	on_activate () final override
	{
		if (browserWindow)
		{
			browserWindow -> blank ();
			browserWindow -> getWindow () .present ();
		}
		else
			realize ();
	}

	virtual
	void
	on_open (const Gio::Application::type_vec_files & files, const Glib::ustring & hint) final override
	{
		if (not browserWindow)
			realize ();

		for (const auto & file : files)
			browserWindow -> open ("file://" + file -> get_path ());

		browserWindow -> getWindow () .present ();

		//Call the base class's implementation:
		Gtk::Application::on_open (files, hint);
	}

	virtual
	void
	on_window_removed (Gtk::Window* window) final override
	{
		quit ();
	}

	///  @name Members

	std::unique_ptr <BrowserWindow> browserWindow;

};

} // puck
} // titania

int
main (int argc, char** argv)
{
	try
	{
		using namespace titania;
		using namespace titania::puck;
	
		#ifdef TITANIA_DEBUG
		std::clog
			<< std::boolalpha
			<< "Titania started ..." << std::endl
			<< " Compiled at " << __DATE__ << " " << __TIME__ << std::endl
			<< std::endl;
		#endif

		// Replace the C++ global locale as well as the C locale with the user-preferred locale.
		std::locale::global (std::locale (""));

		// XXX: This fixes the bug with images in menu items and with no 'active' event for the scene menu item.
		os::env ("GDK_SYNCHRONIZE",       "1");  // Synchrone catch X errors.
		os::env ("UBUNTU_MENUPROXY",      "0");  // Disable global menu.
		//os::env ("GTK_OVERLAY_SCROLLING", "0");  // Disable Gnome overlay scrollbars. // Can be done one each ScrolledWindow
		//os::env ("LIBOVERLAY_SCROLLBAR",  "0");  // Disable Unity overlay scrollbars. // Can be done one each ScrolledWindow
	
		{
			BrowserApplication browserApplication (argc, argv);
	
			browserApplication .run ();
		}
	
		#ifdef TITANIA_DEBUG
		std::clog
			<< std::endl
			<< "Titania finished." << std::endl;
		#endif
	
		return 0;
	}
	catch (const std::exception & error)
	{
		std::clog << error .what () << std::endl;

		return 1;
	}
}
