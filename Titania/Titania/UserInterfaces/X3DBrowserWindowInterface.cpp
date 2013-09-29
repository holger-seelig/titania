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
#include "X3DBrowserWindowInterface.h"

namespace titania {
namespace puck {

const std::string X3DBrowserWindowInterface::m_widgetName = "BrowserWindow";

void
X3DBrowserWindowInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	// Get objects.
	m_fileFilterX3D      = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterX3D"));
	m_iconFactory        = Glib::RefPtr <Gtk::IconFactory>::cast_dynamic (m_builder -> get_object ("IconFactory"));
	m_fileFilterAllFiles = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAllFiles"));
	m_fileFilterAudio    = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterAudio"));
	m_fileFilterImage    = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterImage"));
	m_fileFilterVideo    = Glib::RefPtr <Gtk::FileFilter>::cast_dynamic (m_builder -> get_object ("FileFilterVideo"));
	m_menuAccelGroup     = Glib::RefPtr <Gtk::AccelGroup>::cast_dynamic (m_builder -> get_object ("MenuAccelGroup"));

	// Get widgets.
	m_builder -> get_widget ("ExamineViewerImage", m_examineViewerImage);
	m_examineViewerImage -> set_name ("ExamineViewerImage");
	m_builder -> get_widget ("FileImportDialog", m_fileImportDialog);
	m_fileImportDialog -> set_name ("FileImportDialog");
	m_builder -> get_widget ("FileImportImage", m_fileImportImage);
	m_fileImportImage -> set_name ("FileImportImage");
	m_builder -> get_widget ("FileOpenDialog", m_fileOpenDialog);
	m_fileOpenDialog -> set_name ("FileOpenDialog");
	m_builder -> get_widget ("FileSaveDialog", m_fileSaveDialog);
	m_fileSaveDialog -> set_name ("FileSaveDialog");
	m_builder -> get_widget ("SaveCompressedButton", m_saveCompressedButton);
	m_saveCompressedButton -> set_name ("SaveCompressedButton");
	m_builder -> get_widget ("FileSaveWarningDialog", m_fileSaveWarningDialog);
	m_fileSaveWarningDialog -> set_name ("FileSaveWarningDialog");
	m_builder -> get_widget ("FlyViewerImage", m_flyViewerImage);
	m_flyViewerImage -> set_name ("FlyViewerImage");
	m_builder -> get_widget ("ImportImage", m_importImage);
	m_importImage -> set_name ("ImportImage");
	m_builder -> get_widget ("WalkViewerImage", m_walkViewerImage);
	m_walkViewerImage -> set_name ("WalkViewerImage");
	m_builder -> get_widget ("ViewerTypeMenu", m_viewerTypeMenu);
	m_viewerTypeMenu -> set_name ("ViewerTypeMenu");
	m_builder -> get_widget ("ExamineViewerMenuItem", m_examineViewerMenuItem);
	m_examineViewerMenuItem -> set_name ("ExamineViewerMenuItem");
	m_builder -> get_widget ("WalkViewerMenuItem", m_walkViewerMenuItem);
	m_walkViewerMenuItem -> set_name ("WalkViewerMenuItem");
	m_builder -> get_widget ("FlyViewerMenuItem", m_flyViewerMenuItem);
	m_flyViewerMenuItem -> set_name ("FlyViewerMenuItem");
	m_builder -> get_widget ("NoneViewerMenuItem", m_noneViewerMenuItem);
	m_noneViewerMenuItem -> set_name ("NoneViewerMenuItem");
	m_builder -> get_widget ("MessageDialog", m_messageDialog);
	m_messageDialog -> set_name ("MessageDialog");
	m_builder -> get_widget ("NoneViewerImage", m_noneViewerImage);
	m_noneViewerImage -> set_name ("NoneViewerImage");
	m_builder -> get_widget ("OpenLocationDialog", m_openLocationDialog);
	m_openLocationDialog -> set_name ("OpenLocationDialog");
	m_builder -> get_widget ("OpenLocationEntry", m_openLocationEntry);
	m_openLocationEntry -> set_name ("OpenLocationEntry");
	m_builder -> get_widget ("OpenLocationImage", m_openLocationImage);
	m_openLocationImage -> set_name ("OpenLocationImage");
	m_builder -> get_widget ("Window", m_window);
	m_window -> set_name ("Window");
	m_builder -> get_widget ("Widget", m_widget);
	m_widget -> set_name ("Widget");
	m_builder -> get_widget ("MenuBar", m_menuBar);
	m_menuBar -> set_name ("MenuBar");
	m_builder -> get_widget ("FileMenuItem", m_fileMenuItem);
	m_fileMenuItem -> set_name ("FileMenuItem");
	m_builder -> get_widget ("NewMenuItem", m_newMenuItem);
	m_newMenuItem -> set_name ("NewMenuItem");
	m_builder -> get_widget ("OpenMenuItem", m_openMenuItem);
	m_openMenuItem -> set_name ("OpenMenuItem");
	m_builder -> get_widget ("OpenLocationMenuItem", m_openLocationMenuItem);
	m_openLocationMenuItem -> set_name ("OpenLocationMenuItem");
	m_builder -> get_widget ("ImportMenuItem", m_importMenuItem);
	m_importMenuItem -> set_name ("ImportMenuItem");
	m_builder -> get_widget ("SaveMenuItem", m_saveMenuItem);
	m_saveMenuItem -> set_name ("SaveMenuItem");
	m_builder -> get_widget ("SaveAsMenuItem", m_saveAsMenuItem);
	m_saveAsMenuItem -> set_name ("SaveAsMenuItem");
	m_builder -> get_widget ("RevertMenuItem", m_revertMenuItem);
	m_revertMenuItem -> set_name ("RevertMenuItem");
	m_builder -> get_widget ("QuitMenuItem", m_quitMenuItem);
	m_quitMenuItem -> set_name ("QuitMenuItem");
	m_builder -> get_widget ("EditMenuItem", m_editMenuItem);
	m_editMenuItem -> set_name ("EditMenuItem");
	m_builder -> get_widget ("DeleteMenuItem", m_deleteMenuItem);
	m_deleteMenuItem -> set_name ("DeleteMenuItem");
	m_builder -> get_widget ("GroupSelectedNodesMenuItem", m_groupSelectedNodesMenuItem);
	m_groupSelectedNodesMenuItem -> set_name ("GroupSelectedNodesMenuItem");
	m_builder -> get_widget ("UngroupMenuItem", m_ungroupMenuItem);
	m_ungroupMenuItem -> set_name ("UngroupMenuItem");
	m_builder -> get_widget ("AddToGroupMenuItem", m_addToGroupMenuItem);
	m_addToGroupMenuItem -> set_name ("AddToGroupMenuItem");
	m_builder -> get_widget ("DetachFromGroupMenuItem", m_detachFromGroupMenuItem);
	m_detachFromGroupMenuItem -> set_name ("DetachFromGroupMenuItem");
	m_builder -> get_widget ("CreateParentGroupMenuItem", m_createParentGroupMenuItem);
	m_createParentGroupMenuItem -> set_name ("CreateParentGroupMenuItem");
	m_builder -> get_widget ("ViewMenuItem", m_viewMenuItem);
	m_viewMenuItem -> set_name ("ViewMenuItem");
	m_builder -> get_widget ("ToolBarMenuItem", m_toolBarMenuItem);
	m_toolBarMenuItem -> set_name ("ToolBarMenuItem");
	m_builder -> get_widget ("SideBarMenuItem", m_sideBarMenuItem);
	m_sideBarMenuItem -> set_name ("SideBarMenuItem");
	m_builder -> get_widget ("FooterMenuItem", m_footerMenuItem);
	m_footerMenuItem -> set_name ("FooterMenuItem");
	m_builder -> get_widget ("WorkspacesMenuItem", m_workspacesMenuItem);
	m_workspacesMenuItem -> set_name ("WorkspacesMenuItem");
	m_builder -> get_widget ("BrowserMenuItem", m_browserMenuItem);
	m_browserMenuItem -> set_name ("BrowserMenuItem");
	m_builder -> get_widget ("EditorMenuItem", m_editorMenuItem);
	m_editorMenuItem -> set_name ("EditorMenuItem");
	m_builder -> get_widget ("ShadingMenuItem", m_shadingMenuItem);
	m_shadingMenuItem -> set_name ("ShadingMenuItem");
	m_builder -> get_widget ("PhongMenuItem", m_phongMenuItem);
	m_phongMenuItem -> set_name ("PhongMenuItem");
	m_builder -> get_widget ("GouraudMenuItem", m_gouraudMenuItem);
	m_gouraudMenuItem -> set_name ("GouraudMenuItem");
	m_builder -> get_widget ("FlatMenuItem", m_flatMenuItem);
	m_flatMenuItem -> set_name ("FlatMenuItem");
	m_builder -> get_widget ("WireFrameMenuItem", m_wireFrameMenuItem);
	m_wireFrameMenuItem -> set_name ("WireFrameMenuItem");
	m_builder -> get_widget ("PointSetMenuItem", m_pointSetMenuItem);
	m_pointSetMenuItem -> set_name ("PointSetMenuItem");
	m_builder -> get_widget ("PrimitiveQualtityMenuItem", m_primitiveQualtityMenuItem);
	m_primitiveQualtityMenuItem -> set_name ("PrimitiveQualtityMenuItem");
	m_builder -> get_widget ("HighQualityMenuItem", m_highQualityMenuItem);
	m_highQualityMenuItem -> set_name ("HighQualityMenuItem");
	m_builder -> get_widget ("MediumQualityMenuItem", m_mediumQualityMenuItem);
	m_mediumQualityMenuItem -> set_name ("MediumQualityMenuItem");
	m_builder -> get_widget ("LowQualityMenuItem", m_lowQualityMenuItem);
	m_lowQualityMenuItem -> set_name ("LowQualityMenuItem");
	m_builder -> get_widget ("RenderingPropertiesMenuItem", m_renderingPropertiesMenuItem);
	m_renderingPropertiesMenuItem -> set_name ("RenderingPropertiesMenuItem");
	m_builder -> get_widget ("FullScreenMenuItem", m_fullScreenMenuItem);
	m_fullScreenMenuItem -> set_name ("FullScreenMenuItem");
	m_builder -> get_widget ("UnFullScreenMenuItem", m_unFullScreenMenuItem);
	m_unFullScreenMenuItem -> set_name ("UnFullScreenMenuItem");
	m_builder -> get_widget ("NavigationMenuItem", m_navigationMenuItem);
	m_navigationMenuItem -> set_name ("NavigationMenuItem");
	m_builder -> get_widget ("HeadlightMenuItem", m_headlightMenuItem);
	m_headlightMenuItem -> set_name ("HeadlightMenuItem");
	m_builder -> get_widget ("RubberbandMenuItem", m_rubberbandMenuItem);
	m_rubberbandMenuItem -> set_name ("RubberbandMenuItem");
	m_builder -> get_widget ("EnableInlineViewpointsMenuItem", m_enableInlineViewpointsMenuItem);
	m_enableInlineViewpointsMenuItem -> set_name ("EnableInlineViewpointsMenuItem");
	m_builder -> get_widget ("LookAtAllMenuItem", m_lookAtAllMenuItem);
	m_lookAtAllMenuItem -> set_name ("LookAtAllMenuItem");
	m_builder -> get_widget ("ToolsMenuItem", m_toolsMenuItem);
	m_toolsMenuItem -> set_name ("ToolsMenuItem");
	m_builder -> get_widget ("MotionBlurMenuItem", m_motionBlurMenuItem);
	m_motionBlurMenuItem -> set_name ("MotionBlurMenuItem");
	m_builder -> get_widget ("HelpMenuItem", m_helpMenuItem);
	m_helpMenuItem -> set_name ("HelpMenuItem");
	m_builder -> get_widget ("StandardSizeMenuItem", m_standardSizeMenuItem);
	m_standardSizeMenuItem -> set_name ("StandardSizeMenuItem");
	m_builder -> get_widget ("InfoMenuItem", m_infoMenuItem);
	m_infoMenuItem -> set_name ("InfoMenuItem");
	m_builder -> get_widget ("ToolBar", m_toolBar);
	m_toolBar -> set_name ("ToolBar");
	m_builder -> get_widget ("NewButton", m_newButton);
	m_newButton -> set_name ("NewButton");
	m_builder -> get_widget ("OpenButton", m_openButton);
	m_openButton -> set_name ("OpenButton");
	m_builder -> get_widget ("ImportButton", m_importButton);
	m_importButton -> set_name ("ImportButton");
	m_builder -> get_widget ("SaveButton", m_saveButton);
	m_saveButton -> set_name ("SaveButton");
	m_builder -> get_widget ("RefreshButton", m_refreshButton);
	m_refreshButton -> set_name ("RefreshButton");
	m_builder -> get_widget ("NodePropertiesButton", m_nodePropertiesButton);
	m_nodePropertiesButton -> set_name ("NodePropertiesButton");
	m_builder -> get_widget ("VPaned", m_vPaned);
	m_vPaned -> set_name ("VPaned");
	m_builder -> get_widget ("HPaned", m_hPaned);
	m_hPaned -> set_name ("HPaned");
	m_builder -> get_widget ("SurfaceBox", m_surfaceBox);
	m_surfaceBox -> set_name ("SurfaceBox");
	m_builder -> get_widget ("Dashboard", m_dashboard);
	m_dashboard -> set_name ("Dashboard");
	m_builder -> get_widget ("DashboardToolBar", m_dashboardToolBar);
	m_dashboardToolBar -> set_name ("DashboardToolBar");
	m_builder -> get_widget ("HandButton", m_handButton);
	m_handButton -> set_name ("HandButton");
	m_builder -> get_widget ("ArrowButton", m_arrowButton);
	m_arrowButton -> set_name ("ArrowButton");
	m_builder -> get_widget ("ViewerButton", m_viewerButton);
	m_viewerButton -> set_name ("ViewerButton");
	m_builder -> get_widget ("LookAtAllButton", m_lookAtAllButton);
	m_lookAtAllButton -> set_name ("LookAtAllButton");
	m_builder -> get_widget ("LookAtButton", m_lookAtButton);
	m_lookAtButton -> set_name ("LookAtButton");
	m_builder -> get_widget ("Footer", m_footer);
	m_footer -> set_name ("Footer");
	m_builder -> get_widget ("FooterNotebook", m_footerNotebook);
	m_footerNotebook -> set_name ("FooterNotebook");
	m_builder -> get_widget ("ConsoleBox", m_consoleBox);
	m_consoleBox -> set_name ("ConsoleBox");
	m_builder -> get_widget ("Console", m_console);
	m_console -> set_name ("Console");
	m_builder -> get_widget ("SideBar", m_sideBar);
	m_sideBar -> set_name ("SideBar");
	m_builder -> get_widget ("SideBarLabel", m_sideBarLabel);
	m_sideBarLabel -> set_name ("SideBarLabel");
	m_builder -> get_widget ("SideBarNotebook", m_sideBarNotebook);
	m_sideBarNotebook -> set_name ("SideBarNotebook");
	m_builder -> get_widget ("ViewpointListBox", m_viewpointListBox);
	m_viewpointListBox -> set_name ("ViewpointListBox");
	m_builder -> get_widget ("HistoryEditorBox", m_historyEditorBox);
	m_historyEditorBox -> set_name ("HistoryEditorBox");
	m_builder -> get_widget ("LibraryViewBox", m_libraryViewBox);
	m_libraryViewBox -> set_name ("LibraryViewBox");
	m_builder -> get_widget ("OutlineEditorBox", m_outlineEditorBox);
	m_outlineEditorBox -> set_name ("OutlineEditorBox");
	m_builder -> get_widget ("WorkspacesImage", m_workspacesImage);
	m_workspacesImage -> set_name ("WorkspacesImage");

	// Connect object Gtk::ImageMenuItem with id 'ExamineViewerMenuItem'.
	connections .emplace_back (m_examineViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_examine_viewer_activate)));
	connections .emplace_back (m_walkViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_walk_viewer_activate)));
	connections .emplace_back (m_flyViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fly_viewer_activate)));
	connections .emplace_back (m_noneViewerMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_none_viewer_activate)));

	// Connect object Gtk::MessageDialog with id 'MessageDialog'.
	connections .emplace_back (m_messageDialog -> signal_response () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_messageDialog_response)));

	// Connect object Gtk::Entry with id 'OpenLocationEntry'.
	connections .emplace_back (m_openLocationEntry -> signal_changed () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_location_entry_changed)));
	connections .emplace_back (m_openLocationEntry -> signal_key_release_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_location_entry_key_release_event)));

	// Connect object Gtk::Window with id 'Window'.
	connections .emplace_back (m_window -> signal_key_release_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_key_release_event)));
	connections .emplace_back (m_window -> signal_key_press_event () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_key_press_event)));

	// Connect object Gtk::ImageMenuItem with id 'NewMenuItem'.
	connections .emplace_back (m_newMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new)));
	connections .emplace_back (m_openMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open)));
	connections .emplace_back (m_openLocationMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open_location)));
	connections .emplace_back (m_importMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import)));
	connections .emplace_back (m_saveMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save)));
	connections .emplace_back (m_saveAsMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save_as)));
	connections .emplace_back (m_revertMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved)));
	connections .emplace_back (m_quitMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_close)));
	connections .emplace_back (m_deleteMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_delete_nodes_activate)));

	// Connect object Gtk::MenuItem with id 'GroupSelectedNodesMenuItem'.
	connections .emplace_back (m_groupSelectedNodesMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_group_selected_nodes_activate)));
	connections .emplace_back (m_ungroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_ungroup_node_activate)));
	connections .emplace_back (m_addToGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_add_to_group_activate)));
	connections .emplace_back (m_detachFromGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_detach_from_group_activate)));
	connections .emplace_back (m_createParentGroupMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_create_parent_group_activate)));

	// Connect object Gtk::CheckMenuItem with id 'ToolBarMenuItem'.
	connections .emplace_back (m_toolBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolBar_toggled)));
	connections .emplace_back (m_sideBarMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_sideBar_toggled)));
	connections .emplace_back (m_footerMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_footer_toggled)));

	// Connect object Gtk::RadioMenuItem with id 'BrowserMenuItem'.
	connections .emplace_back (m_browserMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_browser_toggled)));
	connections .emplace_back (m_editorMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_editor_toggled)));
	connections .emplace_back (m_phongMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::phong_activate)));
	connections .emplace_back (m_gouraudMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::gouraud_activate)));
	connections .emplace_back (m_flatMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::flat_activate)));
	connections .emplace_back (m_wireFrameMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::wireframe_activate)));
	connections .emplace_back (m_pointSetMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::pointset_activate)));
	connections .emplace_back (m_highQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_high_quality_activate)));
	connections .emplace_back (m_mediumQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_medium_quality_activate)));
	connections .emplace_back (m_lowQualityMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_low_quality_activate)));

	// Connect object Gtk::CheckMenuItem with id 'RenderingPropertiesMenuItem'.
	connections .emplace_back (m_renderingPropertiesMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_rendering_properties_toggled)));

	// Connect object Gtk::ImageMenuItem with id 'FullScreenMenuItem'.
	connections .emplace_back (m_fullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_fullscreen)));
	connections .emplace_back (m_unFullScreenMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_unfullscreen)));

	// Connect object Gtk::CheckMenuItem with id 'HeadlightMenuItem'.
	connections .emplace_back (m_headlightMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_headlight_toggled)));
	connections .emplace_back (m_rubberbandMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_rubberband_toggled)));
	connections .emplace_back (m_enableInlineViewpointsMenuItem -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_enable_inline_viewpoints_toggled)));

	// Connect object Gtk::ImageMenuItem with id 'LookAtAllMenuItem'.
	connections .emplace_back (m_lookAtAllMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_all_activate)));

	// Connect object Gtk::MenuItem with id 'MotionBlurMenuItem'.
	connections .emplace_back (m_motionBlurMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_motion_blur_editor_activate)));

	// Connect object Gtk::ImageMenuItem with id 'StandardSizeMenuItem'.
	connections .emplace_back (m_standardSizeMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_standard_size)));
	connections .emplace_back (m_infoMenuItem -> signal_activate () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_info)));

	// Connect object Gtk::Toolbar with id 'ToolBar'.
	connections .emplace_back (m_toolBar -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_toolbar_drag_data_received)));

	// Connect object Gtk::ToolButton with id 'NewButton'.
	connections .emplace_back (m_newButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_new)));

	// Connect object Gtk::MenuToolButton with id 'OpenButton'.
	connections .emplace_back (m_openButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_open)));

	// Connect object Gtk::ToolButton with id 'ImportButton'.
	connections .emplace_back (m_importButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_import)));
	connections .emplace_back (m_saveButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_save)));
	connections .emplace_back (m_refreshButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_revert_to_saved)));
	connections .emplace_back (m_nodePropertiesButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_node_properties)));

	// Connect object Gtk::HBox with id 'SurfaceBox'.
	connections .emplace_back (m_surfaceBox -> signal_drag_data_received () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_surface_box_drag_data_received)));

	// Connect object Gtk::RadioToolButton with id 'HandButton'.
	connections .emplace_back (m_handButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_hand_button_toggled)));
	connections .emplace_back (m_arrowButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_arrow_button_toggled)));

	// Connect object Gtk::ToolButton with id 'LookAtAllButton'.
	connections .emplace_back (m_lookAtAllButton -> signal_clicked () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_all_clicked)));

	// Connect object Gtk::ToggleToolButton with id 'LookAtButton'.
	connections .emplace_back (m_lookAtButton -> signal_toggled () .connect (sigc::mem_fun (*this, &X3DBrowserWindowInterface::on_look_at_toggled)));

	// Call construct handler of base class.
	construct ();
}

X3DBrowserWindowInterface::~X3DBrowserWindowInterface ()
{
	delete m_window;
	delete m_openLocationDialog;
	delete m_fileOpenDialog;
	delete m_fileImportDialog;
	delete m_messageDialog;
	delete m_fileSaveDialog;
	delete m_fileSaveWarningDialog;
}

} // puck
} // titania
