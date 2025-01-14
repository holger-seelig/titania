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

#include "ProjectsEditor.h"

#include "../../Bits/File.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileOpenDialog/FileOpenDialog.h"
#include "../../Dialogs/FileOpenDialog/OpenFolderDialog.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"
#include "../../Dialogs/FileOpenDialog/OpenFolderDialog.h"
#include "../../Editors/ProjectsEditor/OpenEditorsEditor.h"
#include "../../Editors/ProjectsEditor/ProjectEditor.h"

#include <Titania/X3D/InputOutput/GoldenGate.h>
#include <Titania/String.h>
#include <regex>

namespace titania {
namespace puck {

ProjectsEditor::ProjectsEditor (X3DBrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DProjectsEditorInterface (get_ui ("Editors/ProjectsEditor.glade")),
	         rootFoldersOutput (),
	               rootFolders (),
	            projectEditors (),
	         openEditorsEditor (new OpenEditorsEditor (browserWindow, this))
{
	addChildObject (rootFoldersOutput);

	setup ();
}

void
ProjectsEditor::initialize ()
{
	X3DProjectsEditorInterface::initialize ();

	// Open Editors

	openEditorsEditor -> reparent (getOpenEditorsBox (), getWindow ());
}

void
ProjectsEditor::configure ()
{
	X3DProjectsEditorInterface::configure ();

	// OpenEditors Paned

	if (getConfig () -> hasItem ("openEditorsPaned"))
		getOpenEditorsPaned () .set_position (getConfig () -> getItem <int32_t> ("openEditorsPaned"));

	// Projects

	for (const auto & widget : getProjectsBox () .get_children ())
		getProjectsBox () .remove (*widget);

	auto projects = getConfig () -> getItem <X3D::MFString> ("projects");

	std::sort (projects .begin (), projects .end (), FolderCompare ());

	for (const auto & folder : basic::make_const_range (projects))
		addRootFolder (folder .raw ());

	set_execution_context ();
}

void
ProjectsEditor::on_map ()
{
	getCurrentContext () .addInterest (&ProjectsEditor::set_execution_context, this);

	set_execution_context ();
}

void
ProjectsEditor::on_unmap ()
{
	getCurrentContext () .removeInterest (&ProjectsEditor::set_execution_context, this);
}

void
ProjectsEditor::on_add_project_clicked ()
{
	const auto openFolderDialog = createDialog <OpenFolderDialog> ("OpenFolderDialog");

	if (not openFolderDialog -> run ())
		return;

	// Create root folder.

	addRootFolder (openFolderDialog -> getUrl () .path ());

	getConfig () -> setItem <X3D::MFString> ("projects", X3D::MFString (rootFolders.begin (), rootFolders .end ()));
}

void
ProjectsEditor::on_remove_project_clicked (const basic::uri & rootFolder)
{
	removeRootFolder (rootFolder);

	getConfig () -> setItem <X3D::MFString> ("projects", X3D::MFString (rootFolders.begin (), rootFolders .end ()));
}

void
ProjectsEditor::set_execution_context ()
{
	// Scroll to current file.

	const auto & worldURL = getCurrentContext () -> getWorldURL ();

	if (worldURL .is_network ())
		return;

	const auto file = Gio::File::create_for_path (worldURL .path ());

	Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &ProjectsEditor::scrollToFile), file));
}

void
ProjectsEditor::addRootFolder (const basic::uri & rootFolder)
{
	if (not rootFolders .emplace (rootFolder) .second)
		return;

	const auto projectEditor = std::make_shared <ProjectEditor> (getBrowserWindow (), rootFolder);

	projectEditor -> getRemoveProjectButton () .signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &ProjectsEditor::on_remove_project_clicked), rootFolder));

	unparent (projectEditor -> getWidget ());

	getProjectsBox () .pack_start (projectEditor -> getWidget (), true, true);

	projectEditors .emplace (rootFolder, projectEditor);

	rootFoldersOutput = getCurrentBrowser () -> getCurrentTime ();
}

void
ProjectsEditor::removeRootFolder (const basic::uri & rootFolder)
{
	rootFolders    .erase (rootFolder);
	projectEditors .erase (rootFolder);

	rootFoldersOutput = getCurrentBrowser () -> getCurrentTime ();
}

void
ProjectsEditor::scrollToFile (const Glib::RefPtr <Gio::File> & file)
{
	int32_t y = 0;

	for (const auto & [rootFolder, projectEditor] : projectEditors)
	{
		const auto iter = projectEditor -> getIter (file);

		y += projectEditor -> getWidget () .get_height ();

		if (not projectEditor -> getFileStore () -> iter_is_valid (iter))
			continue;

		const auto path      = projectEditor -> getFileStore () -> get_path (iter);
		auto       rectangle = Gdk::Rectangle ();

		projectEditor -> getFileView () .get_cell_area (path, *projectEditor -> getFileColumn () .operator -> (), rectangle);

		y -= projectEditor -> getFileView () .get_height () - rectangle .get_y ();
		y -= getScrolledWindow () .get_height () * (2 - math::phi <double>);

		getScrolledWindow () .get_vadjustment () -> set_value (std::max (0, y));
		break;
	}
}

void
ProjectsEditor::store ()
{
	getConfig () -> setItem <int32_t> ("openEditorsPaned", getOpenEditorsPaned () .get_position ());

	X3DProjectsEditorInterface::store ();
}

ProjectsEditor::~ProjectsEditor ()
{
	dispose ();
}

} // puck
} // titania
