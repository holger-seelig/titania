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
#include "X3DLibraryViewInterface.h"

namespace titania {
namespace puck {

void
X3DLibraryViewInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_TreeStore            = Glib::RefPtr <Gtk::TreeStore>::cast_dynamic (m_builder -> get_object ("TreeStore"));
	m_FileColumn           = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FileColumn"));
	m_IconRenderer         = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("IconRenderer"));
	m_NameRenderer         = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("NameRenderer"));
	m_ExperimantalColumn   = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ExperimantalColumn"));
	m_ExperimantalRenderer = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("ExperimantalRenderer"));
	m_TitaniaColumn        = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("TitaniaColumn"));
	m_TitaniaRenderer      = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("TitaniaRenderer"));
	m_CobwebColumn         = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("CobwebColumn"));
	m_CobwebRenderer       = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("CobwebRenderer"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("Notebook", m_Notebook);
	m_builder -> get_widget ("FilesBox", m_FilesBox);
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_builder -> get_widget ("TreeView", m_TreeView);
	m_builder -> get_widget ("PaletteBox", m_PaletteBox);
	m_builder -> get_widget ("PalettePreviewBox", m_PalettePreviewBox);
	m_builder -> get_widget ("ChangePaletteBox", m_ChangePaletteBox);
	m_builder -> get_widget ("PaletteComboBoxText", m_PaletteComboBoxText);
	m_builder -> get_widget ("PalettePreviousButton", m_PalettePreviousButton);
	m_builder -> get_widget ("PaletteNextButton", m_PaletteNextButton);
	m_builder -> get_widget ("EditPaletteDialog", m_EditPaletteDialog);
	m_builder -> get_widget ("EditPaletteCancelButton", m_EditPaletteCancelButton);
	m_builder -> get_widget ("EditPaletteOkButton", m_EditPaletteOkButton);
	m_builder -> get_widget ("PaletteNameEntry", m_PaletteNameEntry);
	m_builder -> get_widget ("PaletteMenu", m_PaletteMenu);
	m_builder -> get_widget ("AddPaletteMenuItem", m_AddPaletteMenuItem);
	m_builder -> get_widget ("RemovePaletteMenuItem", m_RemovePaletteMenuItem);
	m_builder -> get_widget ("EditPaletteMenuItem", m_EditPaletteMenuItem);
	m_builder -> get_widget ("AddObjectToPaletteMenuItem", m_AddObjectToPaletteMenuItem);
	m_builder -> get_widget ("UpdateObjectInPaletteMenuItem", m_UpdateObjectInPaletteMenuItem);
	m_builder -> get_widget ("RemoveObjectFromPaletteMenuItem", m_RemoveObjectFromPaletteMenuItem);

	// Connect object Gtk::TreeView with id 'TreeView'.
	m_TreeView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_row_activated));

	// Connect object Gtk::Box with id 'PalettePreviewBox'.
	m_PalettePreviewBox -> signal_button_press_event () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_palette_button_press_event));

	// Connect object Gtk::ComboBoxText with id 'PaletteComboBoxText'.
	m_PaletteComboBoxText -> signal_changed () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_palette_changed));

	// Connect object Gtk::Button with id 'PalettePreviousButton'.
	m_PalettePreviousButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_palette_previous_clicked));
	m_PaletteNextButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_palette_next_clicked));
	m_EditPaletteCancelButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_edit_palette_cancel_clicked));
	m_EditPaletteOkButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_edit_palette_ok_clicked));

	// Connect object Gtk::Entry with id 'PaletteNameEntry'.
	m_PaletteNameEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_palette_name_changed));
	m_PaletteNameEntry -> signal_delete_text () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_palette_name_delete_text), false);
	m_PaletteNameEntry -> signal_insert_text () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_palette_name_insert_text), false);

	// Connect object Gtk::ImageMenuItem with id 'AddPaletteMenuItem'.
	m_AddPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_add_palette_activate));
	m_RemovePaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_remove_palette_activate));
	m_EditPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_edit_palette_activate));
	m_AddObjectToPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_add_object_to_palette_activate));
	m_UpdateObjectInPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_update_object_in_palette_activate));
	m_RemoveObjectFromPaletteMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_remove_object_from_palette_activate));

	// Call construct handler of base class.
	construct ();
}

X3DLibraryViewInterface::~X3DLibraryViewInterface ()
{
	delete m_Window;
	delete m_EditPaletteDialog;
}

} // puck
} // titania
