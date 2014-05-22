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

#include "X3DBrowserWidget.h"

#include "../Browser/BrowserWindow.h"
#include "../Browser/Image.h"
#include "../Configuration/config.h"

#include <Titania/String.h>
#include <Titania/gzstream.h>

#include <fstream>

#include <gdk/gdkx.h>

namespace titania {
namespace puck {

X3DBrowserWidget::X3DBrowserWidget (int argc, char** argv) :
	X3DBrowserWindowInterface (get_ui ("BrowserWindow.xml"), gconf_dir ()),
	                    world (getExecutionContext ())
{
	addChildren (world);

	parseOptions (argc, argv);
}

void
X3DBrowserWidget::parseOptions (int argc, char** argv)
{
	// Create and intialize option parser.

	// Add Remaining options to option group.

	Glib::OptionGroup mainGroup ("example_group", "description of example group", "help description of example group");

	Glib::OptionEntry              remaining;
	Glib::OptionGroup::vecustrings remainingOptions;

	remaining .set_long_name (G_OPTION_REMAINING);
	remaining .set_arg_description (G_OPTION_REMAINING);

	mainGroup .add_entry (remaining, remainingOptions);

	// Intialize OptionContext.

	Glib::OptionContext optionContext;

	optionContext .set_main_group (mainGroup);

	// Parse options.

	try
	{
		optionContext .parse (argc, argv);

		if (remainingOptions .empty ())
			getConfig () .setItem ("url", "");
		else
			getConfig () .setItem ("url", remainingOptions [0]);
	}
	catch (const Glib::Error & error)
	{
		std::clog << "Exception: " << error .what () << std::endl;
	}
}

void
X3DBrowserWidget::initialize ()
{
	X3DBrowserWindowInterface::initialize ();

	// Enable splash screen.
	X3D::getBrowser () -> getBrowserOptions () -> splashScreen () = true;

	// Connect event handler.
	getBrowser () -> getConsole () -> string_changed () .addInterest (this, &X3DBrowserWidget::set_console);
	getBrowser () -> getUrlError () .addInterest (this, &X3DBrowserWidget::set_urlError);
	getBrowser () -> initialized () .addInterest (this, &X3DBrowserWidget::set_splashScreen);

	// Insert Surface, this will initialize the Browser.
	getSurfaceBox () .pack_start (*getBrowser (), true, true, 0);

	// Show Surface and start the X3D Main Loop.
	getBrowser () -> show ();
}

void
X3DBrowserWidget::set_splashScreen ()
{
	// Initialized

	getBrowser () -> initialized () .removeInterest (this, &X3DBrowserWidget::set_splashScreen);
	getWorld () .addInterest (this, &X3DBrowserWidget::set_world);

	if (getConfig () .getString ("url") .size ())
	{
		if (getConfig () .getString ("url") == "about:blank")
			blank ();
		else
			open (getConfig () .getString ("url") .raw ());
	}
	else if (getConfig () .getString ("worldURL") .size ())
		open (getConfig () .getString ("worldURL") .raw ());

	else
		open (get_page ("about/home.wrl"));
}

void
X3DBrowserWidget::restoreSession ()
{
	X3DBrowserWindowInterface::restoreSession ();

	// Restore Menu Configuration from Config

	// ToolBar
	if (getConfig () .hasItem ("toolBar"))
		getToolBarMenuItem () .set_active (getConfig () .getBoolean ("toolBar"));

	// SideBar
	if (getConfig () .hasItem ("sideBar"))
		getSideBarMenuItem () .set_active (getConfig () .getBoolean ("sideBar"));

	// Footer
	if (getConfig () .hasItem ("footer"))
		getFooterMenuItem () .set_active (getConfig () .getBoolean ("footer"));

	// Shading
	if (getConfig () .getString ("shading") == "PHONG")
		getPhongMenuItem () .set_active (true);

	else if (getConfig () .getString ("shading") == "GOURAUD")
		getGouraudMenuItem () .set_active (true);

	else if (getConfig () .getString ("shading") == "FLAT")
		getFlatMenuItem () .set_active (true);

	else if (getConfig () .getString ("shading") == "WIREFRAME")
		getWireFrameMenuItem () .set_active (true);

	else if (getConfig () .getString ("shading") == "POINTSET")
		getPointSetMenuItem () .set_active (true);

	// PrimitiveQuality
	if (getConfig () .getString ("primitiveQuality") == "HIGH")
		getHighQualityMenuItem () .set_active (true);

	else if (getConfig () .getString ("primitiveQuality") == "MEDIUM")
		getMediumQualityMenuItem () .set_active (true);

	else if (getConfig () .getString ("primitiveQuality") == "LOW")
		getLowQualityMenuItem () .set_active (true);

	// RenderingProperties
	if (getConfig () .hasItem ("renderingProperties"))
		getRenderingPropertiesMenuItem () .set_active (getConfig () .getBoolean ("renderingProperties"));

	// Rubberband
	if (getConfig () .hasItem ("rubberBand"))
		getRubberbandMenuItem () .set_active (getConfig () .getBoolean ("rubberBand"));

	if (not getConfig () .hasItem ("isLive"))
		getConfig () .setItem ("isLive", true);

	isLive (getConfig () .getBoolean ("isLive"));

	// VPaned
	if (getConfig () .hasItem ("vPaned"))
		getVPaned () .set_position (getConfig () .getInteger ("vPaned"));

	// HPaned
	if (getConfig () .hasItem ("hPaned"))
		getHPaned () .set_position (getConfig () .getInteger ("hPaned"));

	getSideBarNotebook () .set_current_page (getConfig () .getInteger ("sideBarCurrentPage")); // XXX Produces a width/height -1 warning
	getFooterNotebook ()  .set_current_page (getConfig () .getInteger ("footerCurrentPage"));
}

void
X3DBrowserWidget::saveSession ()
{
	getConfig () .setItem ("vPaned", getVPaned () .get_position ());
	getConfig () .setItem ("hPaned", getHPaned () .get_position ());

	getConfig () .setItem ("sideBarCurrentPage", getSideBarNotebook () .get_current_page ());
	getConfig () .setItem ("footerCurrentPage",  getFooterNotebook ()  .get_current_page ());

	if (getWorld () -> getWorldURL () .size ())
		getConfig () .setItem ("worldURL", getWorld () -> getWorldURL ());

	X3DBrowserWindowInterface::saveSession ();
}

void
X3DBrowserWidget::isLive (const bool value)
{
	if (value)
		getPlayPauseButton () .set_stock_id (Gtk::StockID ("gtk-media-pause"));

	else
		getPlayPauseButton () .set_stock_id (Gtk::StockID ("gtk-media-play"));

	getWorld () -> isLive (value);
}

void
X3DBrowserWidget::updateTitle (const bool edited) const
{
	getWindow () .set_title (getWorld () -> getTitle ()
	                         + " · "
	                         + getWorld () -> getWorldURL ()
	                         + (edited ? "*" : "")
	                         + " · "
	                         + getBrowser () -> getName ());
}

void
X3DBrowserWidget::blank ()
{
	try
	{
		getBrowser () -> replaceWorld (X3D::ScenePtr ());
		world = getExecutionContext ();
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBrowserWidget::open (const basic::uri & worldURL)
{
	loadTime = chrono::now ();

	try
	{
		getBrowser () -> loadURL ({ worldURL .str () });
		world = getExecutionContext ();
		world -> isLive (getConfig () .getBoolean ("isLive"));
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DBrowserWidget::save (const basic::uri & worldURL, const bool compressed)
{
	const auto suffix           = worldURL .suffix ();
	const auto executionContext = X3D::X3DExecutionContextPtr (getRootContext ());

	executionContext -> setWorldURL (worldURL);
	executionContext -> isCompressed (compressed);

	// Save

	if (suffix == ".x3d" or suffix == ".xml")
	{
		if (executionContext -> getVersion () == X3D::VRML_V2_0)
		{
			executionContext -> setEncoding ("X3D");
			executionContext -> setSpecificationVersion (X3D::XMLEncode (X3D::LATEST_VERSION));
		}

		if (compressed)
		{
			ogzstream file (worldURL .path ());

			file
				<< X3D::SmallestStyle
				<< X3D::XMLEncode (executionContext);
		}
		else
		{
			std::ofstream file (worldURL .path ());

			file
				<< X3D::CompactStyle
				<< X3D::XMLEncode (executionContext);
		}
	}
	else
	{
		if (suffix == ".wrl")
		{
			if (executionContext -> getVersion () not_eq X3D::VRML_V2_0)
			{
				executionContext -> setEncoding ("VRML");
				executionContext -> setSpecificationVersion ("2.0");
			}
		}
		else  // ".x3dv"
		{
			if (executionContext -> getVersion () == X3D::VRML_V2_0)
			{
				executionContext -> setEncoding ("X3D");
				executionContext -> setSpecificationVersion (X3D::XMLEncode (X3D::LATEST_VERSION));
			}
		}

		if (compressed)
		{
			ogzstream file (worldURL .path ());

			file
				<< X3D::SmallestStyle
				<< executionContext;
		}
		else
		{
			std::ofstream file (worldURL .path ());

			file
				<< X3D::NicestStyle
				<< executionContext;
		}
	}
}

void
X3DBrowserWidget::reload ()
{
	open (getWorld () -> getWorldURL ());
}

void
X3DBrowserWidget::set_world ()
{
	loadTime = chrono::now () - loadTime;

	timeout .disconnect ();
	timeout = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DBrowserWidget::statistics), 10 * 1000);

	loadIcon ();
	updateTitle (false);

	// Remember last local file

	const auto worldURL = getWorld () -> getWorldURL ();

	if (not worldURL .empty () and worldURL .is_local ())
		getFileOpenDialog () .set_uri (worldURL .filename () .str ());
}

bool
X3DBrowserWidget::statistics ()
{
	std::string title = getWorld () -> getWorldURL ();

	try
	{
		title = getWorld () -> getMetaData ("title");
		std::clog << "Statistics for: " << title << std::endl;
	}
	catch (...)
	{ }

	std::clog << "Load Time: " << loadTime << std::endl;
	std::clog << "FPS: " << getBrowser () -> getRenderingProperties () -> getFPS () << std::endl;

	return false;
}

void
X3DBrowserWidget::set_console ()
{
	const auto buffer = getConsole () .get_buffer ();

	// Insert

	for (const auto & string : getBrowser () -> getConsole () -> string_changed ())
		buffer -> insert (buffer -> end (), string .getValue ());

	buffer -> place_cursor (buffer -> end ());

	getConsole () .scroll_to (buffer -> get_insert ());

	// Erase

	static const int CONSOLE_LIMIT = std::pow (2, 20);

	if (buffer -> size () > CONSOLE_LIMIT)
	{
		const int char_offset = buffer -> size () - CONSOLE_LIMIT;

		buffer -> erase (buffer -> begin (), buffer -> get_iter_at_offset (char_offset));
	}
}

void
X3DBrowserWidget::set_urlError (const X3D::MFString & urlError)
{
	getMessageDialog () .set_message (_ ("Invalid X3D"));
	getMessageDialog () .set_secondary_text ("<span font_desc=\"mono\">"
	                                         + Glib::Markup::escape_text (basic::join (urlError, "\n"))
	                                         + "</span>",
	                                         true);
	getMessageDialog () .present ();
}

void
X3DBrowserWidget::loadIcon ()
{
	const basic::uri & worldURL = getWorld () -> getWorldURL ();

	const Gtk::StockID stockId = Gtk::StockID (worldURL .str ());

	Glib::RefPtr <Gtk::IconSet> iconSet;

	try
	{
		// Create Stock Icon

		basic::uri uri;

		try
		{
			uri = getWorld () -> getMetaData ("icon");
		}
		catch (const X3D::Error <X3D::INVALID_NAME> &)
		{
			if (worldURL .is_local ())
				throw;

			uri = "/favicon.ico";
		}

		const titania::Image icon (X3D::Loader (getWorld ()) .loadDocument (uri));

		iconSet = Gtk::IconSet::create (Gdk::Pixbuf::create_from_data (icon .getData (),
		                                                               Gdk::COLORSPACE_RGB,
		                                                               icon .getTransparency (),
		                                                               sizeof (Image::value_type) * 8,
		                                                               icon .getWidth (),
		                                                               icon .getHeight (),
		                                                               icon .getWidth () * icon .getComponents ()) -> copy ());
	}
	catch (const std::exception & error)
	{
		iconSet = Gtk::IconSet::lookup_default (Gtk::StockID ("BlankIcon"));
	}

	getIconFactory () -> add (stockId, iconSet);
	Gtk::Stock::add (Gtk::StockItem (stockId, worldURL .str ()));
}

void
X3DBrowserWidget::setTransparent (const bool value)
{
	if (value)
	{
		Glib::RefPtr <Gdk::Visual> visual = getWindow () .get_screen () -> get_rgba_visual ();

		if (visual)
			gtk_widget_set_visual (GTK_WIDGET (getWindow () .gobj ()), visual -> gobj ());
	}
}

X3DBrowserWidget::~X3DBrowserWidget ()
{ }

} // puck
} // titania
