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

#ifndef __TITANIA_WIDGETS_NOTEBOOK_PAGE_NOTEBOOK_PAGE_H__
#define __TITANIA_WIDGETS_NOTEBOOK_PAGE_NOTEBOOK_PAGE_H__

#include "../../UserInterfaces/X3DNotebookPageInterface.h"
#include "../../Browser/BrowserHistory.h"

#include <Titania/X3D/Editing/Undo/UndoHistory.h>

namespace titania {
namespace puck {

class NotebookPage :
	virtual public X3DNotebookPageInterface
{
public:

	///  @name Construction

	NotebookPage (X3DBrowserWindow* const browserWindow, const basic::uri & startUrl);

	///  @name Member access

	int32_t
	getPageNumber () const;

	const X3D::BrowserPtr &
	getMainBrowser () const
	{ return mainBrowser; }

	const basic::uri &
	getWorldURL () const;

	BrowserHistory &
	getBrowserHistory ()
	{	return browserHistory; }

	const BrowserHistory &
	getBrowserHistory () const
	{	return browserHistory; }

	X3D::UndoHistory &
	getUndoHistory ()
	{	return undoHistory; }

	const X3D::UndoHistory &
	getUndoHistory () const
	{	return undoHistory; }

	void
	setModified (const bool value);

	bool
	getModified () const;

	void
	setSaveConfirmed (const bool value)
	{ saveConfirmed = value; }

	bool
	getSaveConfirmed () const
	{ return saveConfirmed; }

	void
	addFileMonitor (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::FileMonitor> & fileMonitor);

	///  @name Destruction

	void
	reset ();

	virtual
	~NotebookPage () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_initialized (const size_t index);

	void
	set_activeLayer ();

	bool
	on_focus_out_event (GdkEventFocus* event, const size_t index);

	bool
	on_focus_in_event (GdkEventFocus* event, const size_t index);

	virtual
	bool
	on_box1_key_release_event (GdkEventKey* event) final override;

	virtual
	bool
	on_box2_key_release_event (GdkEventKey* event) final override;

	virtual
	bool
	on_box3_key_release_event (GdkEventKey* event) final override;

	virtual
	bool
	on_box4_key_release_event (GdkEventKey* event) final override;

	bool
	on_box_key_release_event (GdkEventKey* event, const size_t index);

	///  @name Members

	X3D::BrowserPtr                 mainBrowser;
	X3D::BrowserPtr                 topBrowser;
	X3D::BrowserPtr                 rightBrowser;
	X3D::BrowserPtr                 frontBrowser;
	X3D::X3DPtrArray <X3D::Browser> browsers;
	X3D::X3DPtr <X3D::X3DLayerNode> activeLayer;
	size_t                          initialized;
	basic::uri                      url; // Start URL
	BrowserHistory                  browserHistory;
	X3D::UndoHistory                undoHistory;
	bool                            modified;
	bool                            saveConfirmed;

	std::vector <std::pair <Glib::RefPtr <Gio::File>, Glib::RefPtr <Gio::FileMonitor>>> fileMonitors;

	std::vector <Gtk::Widget*> widgets;
	bool                       multiView;
	X3D::MFVec3f               positions;
	X3D::MFRotation            orientations;

};

} // puck
} // titania

#endif
