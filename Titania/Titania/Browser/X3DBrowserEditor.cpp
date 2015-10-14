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

#include "X3DBrowserEditor.h"

#include "../Base/X3DEditorObject.h"
#include "../Browser/BrowserUserData.h"
#include "../Browser/MagicImport.h"
#include "../Browser/X3DBrowserWindow.h"
#include "../Configuration/config.h"
#include "../Dialogs/FileSaveWarningDialog/FileSaveWarningDialog.h"
#include "../Widgets/OutlineEditor/OutlineTreeViewEditor.h"

#include <Titania/X3D/Components/Core/MetadataDouble.h>
#include <Titania/X3D/Components/Core/MetadataString.h>
#include <Titania/X3D/Components/Core/MetadataSet.h>
#include <Titania/X3D/Components/Core/WorldInfo.h>
#include <Titania/X3D/Components/Core/X3DPrototypeInstance.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/X3DTransformNode.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Components/Networking/Inline.h>
#include <Titania/X3D/Execution/ImportedNode.h>
#include <Titania/X3D/Execution/ExportedNode.h>
#include <Titania/X3D/Execution/World.h>
#include <Titania/X3D/Prototype/ProtoDeclaration.h>
#include <Titania/X3D/InputOutput/GoldenGate.h>
#include <Titania/X3D/Parser/Filter.h>

#include <Titania/InputOutput/MultiLineComment.h>
#include <Titania/OS.h>
#include <Titania/String.h>
#include <Titania/Utility/Map.h>

namespace titania {
namespace puck {

static constexpr double UNDO_TIME = 0.6; // Key press delay time + 0.1???

X3DBrowserEditor::X3DBrowserEditor (const X3D::BrowserPtr & browser) :
	         X3DBrowserWidget (browser),
	    X3D::X3DBrowserEditor (),
	                  enabled (false),
	             currentScene (),
	                selection (new BrowserSelection (getBrowserWindow ())),
	             undoMatrices (),
	                 undoStep (),
	                 undoTime (0),
	                     tool (NONE_TOOL)
{
	addChildren (enabled, currentScene);
}

void
X3DBrowserEditor::initialize ()
{
	X3DBrowserWidget::initialize ();

	getExecutionContext () .addInterest (this, &X3DBrowserEditor::set_executionContext);
	getBrowserWindow () -> getSelection () -> isActive () .addInterest (this, &X3DBrowserEditor::set_selection_active);
	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DBrowserEditor::set_selection);
}

void
X3DBrowserEditor::restoreSession ()
{
	// Workspace
	if (not getConfig () .hasItem ("environment"))
	   getConfig () .setItem ("environment", 1);

	if (getConfig () .getInteger ("environment") == 1)
		getEditorAction () -> set_active (true);
	else
		getBrowserAction () -> set_active (true);

	// SelectLowest
	if (getConfig () .hasItem ("selectLowest"))
		getSelectLowestAction () -> set_active (getConfig () .getBoolean ("selectLowest"));

	// FollowPrimarySelection
	if (getConfig () .hasItem ("followPrimarySelection"))
		getFollowPrimarySelectionAction () -> set_active (getConfig () .getBoolean ("followPrimarySelection"));

	// This must be done after.
	X3DBrowserWidget::restoreSession ();
}

void
X3DBrowserEditor::setBrowser (const X3D::BrowserPtr & value)
{
	getBrowser () -> initialized () .removeInterest (this, &X3DBrowserEditor::set_initialized);
	getBrowser () -> shutdown ()    .removeInterest (this, &X3DBrowserEditor::set_shutdown);
	getUndoHistory (getBrowser ())  .removeInterest (this, &X3DBrowserEditor::set_undoHistory);

	X3DBrowserWidget::setBrowser (value);
	
	currentScene = getBrowser () -> getExecutionContext ();

	getBrowser () -> initialized () .addInterest (this, &X3DBrowserEditor::set_initialized);
	getBrowser () -> shutdown ()    .addInterest (this, &X3DBrowserEditor::set_shutdown);
	getUndoHistory (getBrowser ())  .addInterest (this, &X3DBrowserEditor::set_undoHistory);

	set_undoHistory ();
}

void
X3DBrowserEditor::set_initialized ()
{
	if (getExecutionContext () not_eq currentScene)
		isModified (getBrowser (), false);

	currentScene = getExecutionContext ();
}

void
X3DBrowserEditor::set_shutdown ()
{
	if (isSaved (getBrowser ()))
		getUserData (getBrowser ()) -> dispose ();

	else                                     // Cancel shutdown
		setExecutionContext (getExecutionContext ());
}
	
void
X3DBrowserEditor::set_executionContext ()
{
	if (not getArrowButton () .get_active ())
	   return;

	try
	{
		static const std::map <std::string, X3D::ViewerType> viewerTypes = {
			std::make_pair ("EXAMINE",             X3D::ViewerType::EXAMINE),
			std::make_pair ("WALK",                X3D::ViewerType::WALK),
			std::make_pair ("FLY",                 X3D::ViewerType::FLY),
			std::make_pair ("PLANE",               X3D::ViewerType::PLANE),
			std::make_pair ("PLANE_create3000.de", X3D::ViewerType::PLANE),
			std::make_pair ("NONE",                X3D::ViewerType::NONE),
			std::make_pair ("LOOKAT",              X3D::ViewerType::LOOKAT)
		};

		const auto   worldInfo   = getWorldInfo ();
		const auto   metadataSet = worldInfo -> getMetaData <X3D::MetadataSet> ("/Titania/NavigationInfo");
		const auto & type        = metadataSet -> getValue <X3D::MetadataString> ("type") -> value ();

		setViewer (viewerTypes .at (type .at (0)));
	}
	catch (const std::exception & error)
	{
	   setViewer (X3D::ViewerType::EXAMINE);
	}

	try
	{
		const auto worldInfo   = getWorldInfo ();
		const auto metadataSet = worldInfo -> getMetaData <X3D::MetadataSet> ("/Titania/Viewpoint");

		auto & p = metadataSet -> getValue <X3D::MetadataDouble> ("position") -> value ();
		auto & o = metadataSet -> getValue <X3D::MetadataDouble> ("orientation") -> value ();
		auto & c = metadataSet -> getValue <X3D::MetadataDouble> ("centerOfRotation") -> value ();

		const auto viewpoint = getWorld () -> getActiveLayer () -> getViewpoint ();
		   
		viewpoint -> setUserPosition         (X3D::Vector3f (p .get1Value (0), p .get1Value (1), p .get1Value (2)));
		viewpoint -> setUserOrientation      (X3D::Rotation4f (o .get1Value (0), o .get1Value (1), o .get1Value (2), o .get1Value (3)));
		viewpoint -> setUserCenterOfRotation (X3D::Vector3f (c .get1Value (0), c .get1Value (1), c .get1Value (2)));

		viewpoint -> set_bind () = true;
	}
	catch (const X3D::X3DError & error)
	{ }
}

void
X3DBrowserEditor::setViewer (const X3D::ViewerType viewer)
{
	getBrowser () -> setLockViewer (false);
	getBrowser () -> setViewer (viewer);
	getBrowser () -> setLockViewer (getArrowButton () .get_active ());
}

void
X3DBrowserEditor::set_selection_active (const bool value)
{
	if (value)
	{
		for (const auto & child : getSelection () -> getChildren ())
		{
			const X3D::X3DTransformNodePtr transform (child);

			if (transform)
				undoMatrices [transform] = std::make_pair (transform -> getMatrix (), transform -> center () .getValue ());
		}
	}
	else
	{
		bool changed = false;

		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Edit Transform"));

		getSelection () -> redoRestoreSelection (undoStep);

		for (const auto & child :getSelection () -> getChildren ())
		{
			const X3D::X3DTransformNodePtr transform (child);

			if (transform)
			{
				try
				{
					const X3D::Matrix4f matrix = undoMatrices .at (transform) .first;
					const X3D::Vector3f center = undoMatrices .at (transform) .second;

					if (matrix not_eq transform -> getMatrix () or center not_eq transform -> center ())
					{
						changed = true;

						undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
						                             transform,
						                             matrix,
						                             center);

						undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter,
						                             transform,
						                             transform -> getMatrix (),
						                             transform -> center ());
					}
				}
				catch (const std::out_of_range &)
				{ }
			}
		}

		undoMatrices .clear ();

		getSelection () -> undoRestoreSelection (undoStep);

		if (changed)
			addUndoStep (undoStep);
	}
}

void
X3DBrowserEditor::set_selection (const X3D::MFNode & selection)
{
	if (selection .empty ())
		return;

	// If the parent node of the selection leader is a Switch, set whichChoice to the index of the leader.

	for (const auto & node : getParentNodes (selection .back ()))
	{
		const auto switchNode = dynamic_cast <X3D::Switch*> (node .getValue ());

		if (not switchNode)
			continue;

		const auto iter = std::find (switchNode -> children () .begin (),
		                             switchNode -> children () .end (),
		                             selection .back ());

		if (iter == switchNode -> children () .end ())
			continue;

		switchNode -> setWhichChoice (iter - switchNode -> children () .begin ());
	}
}

void
X3DBrowserEditor::isEditor (const bool value)
{
	enabled = value;
	getConfig () .setItem ("environment", value ? 1 : 0);
}

bool
X3DBrowserEditor::isSaved (const X3D::BrowserPtr & browser)
{
	const auto userData = getUserData (browser);

	if (userData -> saveConfirmed)
		return true;

	if (isModified (browser))
	{
		const auto pageNumber = getBrowserNotebook () .page_num (*browser);
		
		if (pageNumber < 0)
			return true;
	
		getBrowserNotebook () .set_current_page (pageNumber);

		const auto responseId = std::dynamic_pointer_cast <FileSaveWarningDialog> (addDialog ("FileSaveWarningDialog", false)) -> run ();

		switch (responseId)
		{
			case Gtk::RESPONSE_OK:
			{
				on_save_activated ();
				userData -> saveConfirmed = not isModified (browser);
				return userData -> saveConfirmed;
			}
			case Gtk::RESPONSE_CANCEL:
			case Gtk::RESPONSE_DELETE_EVENT:
			{
				return false;
			}
			default:
			{
				userData -> saveConfirmed = true;
				return true;
			}
		}
	}

	return true;
}

void
X3DBrowserEditor::isModified (const X3D::BrowserPtr & browser, const bool value)
{
	const auto userData = getUserData (browser);

	userData -> modified      = value;
	userData -> saveConfirmed = false;

	if (not value)
		getUndoHistory (browser) .setSaved ();

	setTitle ();
}

bool
X3DBrowserEditor::isModified (const X3D::BrowserPtr & browser) const
{
	return getUndoHistory (browser) .isModified () or getUserData (browser) -> modified;
}

// File operations

void
X3DBrowserEditor::blank ()
{
	if (isEditor ())
	{
		append (X3D::createBrowser (getBrowser ()), "");
		getBrowserNotebook () .set_current_page (getBrowsers () .size () - 1);
	}
	else
		openRecent ();
}

void
X3DBrowserEditor::open (const basic::uri & URL)
{
	if (isEditor ())
		X3DBrowserWidget::open (URL);

	else
		load (getBrowser (), URL);
}

void
X3DBrowserEditor::load (const X3D::BrowserPtr & browser, const basic::uri & URL)
{
	if (isSaved (browser))
		X3DBrowserWidget::load (browser, URL);
}

X3D::MFNode
X3DBrowserEditor::importURL (const std::vector <basic::uri> & uris, const bool importAsInline, const X3D::UndoStepPtr & undoStep)
{
	if (importAsInline)
	{
		// Import As Inline

		for (const auto & worldURL : uris)
		{
			const auto relativePath = getExecutionContext () -> getWorldURL () .relative_path (worldURL);

			std::string string;

			string += "DEF " + X3D::get_name_from_uri (worldURL) + " Transform {";
			string += "  children Inline {";
			string += "    url [";
			string += "      \"" + relativePath + "\"";
			string += "      \"" + worldURL + "\"";
			string += "    ]";
			string += "  }";
			string += "}";

			const auto scene = getBrowser () -> createX3DFromString (string);

			return importScene (scene, getExecutionContext () -> getRootNodes (), undoStep);
		}

		return X3D::MFNode ();
	}

	// Import into scene graph

	X3D::MFNode nodes;
	auto        selection = getSelection () -> getChildren ();

	for (const auto & worldURL : uris)
	{
		try
		{
			const auto scene = getBrowser () -> createX3DFromURL ({ worldURL .str () });

			if (MagicImport (getBrowserWindow ()) .import (selection, scene, undoStep))
				return selection;

			const auto importedNodes = importScene (scene, getExecutionContext () -> getRootNodes (), undoStep);

			nodes .insert (nodes .end (), importedNodes .begin (), importedNodes .end ());
		}
		catch (const X3D::X3DError & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	return nodes;
}

X3D::MFNode
X3DBrowserEditor::importScene (const X3D::X3DScenePtr & scene, X3D::MFNode & field, const X3D::UndoStepPtr & undoStep)
{
	const auto & executionContext = getExecutionContext ();

	try
	{
		const size_t size = field .size ();

		using resize = void (X3D::MFNode::*) (const X3D::MFNode::size_type);

		undoStep -> addUndoFunction ((resize) &X3D::MFNode::resize, std::ref (field), size);

		// Imported scene

		X3D::MFNode importedNodes;

		executionContext -> import (scene, importedNodes);

		const auto undoRemoveNodes = std::make_shared <X3D::UndoStep> ();

		removeNodesFromScene (executionContext, importedNodes, undoRemoveNodes, false);

		undoStep -> addUndoFunction (&X3D::UndoStep::redoChanges, undoRemoveNodes);
		undoStep -> addRedoFunction (&X3D::UndoStep::undoChanges, undoRemoveNodes);
		undoRemoveNodes -> undoChanges ();

		using append = X3D::X3DArrayField <X3D::SFNode> & (X3D::MFNode::*) (const X3D::X3DArrayField <X3D::SFNode> &);

		undoStep -> addRedoFunction ((append) &X3D::MFNode::append, std::ref (field), importedNodes);
		field .append (importedNodes);

		return importedNodes;
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
		return X3D::MFNode ();
	}
}

std::vector <std::tuple <X3D::SFNode, std::string, X3D::SFNode, std::string>>
X3DBrowserEditor::getImportedRoutes (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DScenePtr & scene) const
{
	std::vector <std::tuple <X3D::SFNode, std::string, X3D::SFNode, std::string>> routes;

	for (const auto & pair : executionContext -> getImportedNodes ())
	{
		try
		{
			const auto & inlineNode = pair .second -> getInlineNode ();

			if (inlineNode -> getInternalScene () not_eq scene)
				continue;

			const auto exportedNode = pair .second -> getExportedNode ();

			for (const auto & field : exportedNode -> getFieldDefinitions ())
			{
				for (const auto & route : field -> getInputRoutes ())
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					try
					{
						routes .emplace_back (route -> getSourceNode (), route -> getSourceField (),
						                      route -> getDestinationNode (), route -> getDestinationField ());
					}
					catch (const X3D::X3DError &)
					{ }
				}

				for (const auto & route: field -> getOutputRoutes ())
				{
					if (route -> getExecutionContext () not_eq executionContext)
						continue;

					try
					{
						routes .emplace_back (route -> getSourceNode (), route -> getSourceField (),
						                      route -> getDestinationNode (), route -> getDestinationField ());
					}
					catch (const X3D::X3DError &)
					{ }
				}
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	return routes;
}

bool
X3DBrowserEditor::save (const basic::uri & worldURL, const bool compressed, const bool copy)
{
	if (true)
	{
	   static const std::map <X3D::ViewerType, std::string> types = {
			std::make_pair (X3D::ViewerType::EXAMINE, "EXAMINE"),
			std::make_pair (X3D::ViewerType::WALK,    "WALK"),
			std::make_pair (X3D::ViewerType::FLY,     "FLY"),
			std::make_pair (X3D::ViewerType::PLANE,   "PLANE"),
			std::make_pair (X3D::ViewerType::NONE,    "NONE"),
			std::make_pair (X3D::ViewerType::LOOKAT,  "LOOKAT")
	   };

		const auto worldInfo   = createWorldInfo ();
		const auto metadataSet = worldInfo -> createMetaData <X3D::MetadataSet> ("/Titania/NavigationInfo");

		const auto type = types .find (getBrowser () -> getViewer ());

		metadataSet -> createValue <X3D::MetadataString> ("type") -> value () = { type not_eq types .end () ? type -> second : "EXAMINE" };
	}

	if (true)
	{
		const auto   worldInfo        = createWorldInfo ();
		const auto   metadataSet      = worldInfo -> createMetaData <X3D::MetadataSet> ("/Titania/Viewpoint");
		const auto & activeLayer      = getWorld () -> getActiveLayer ();
		const auto   viewpoint        = activeLayer -> getViewpoint ();
		const auto   position         = viewpoint -> getUserPosition ();
		const auto   orientation      = viewpoint -> getUserOrientation ();
		const auto   centerOfRotation = viewpoint -> getUserCenterOfRotation ();

		metadataSet -> createValue <X3D::MetadataDouble> ("position")         -> value () = { position         .x (), position         .y (), position         .z () };
		metadataSet -> createValue <X3D::MetadataDouble> ("orientation")      -> value () = { orientation      .x (), orientation      .y (), orientation      .z (), orientation .angle () };
		metadataSet -> createValue <X3D::MetadataDouble> ("centerOfRotation") -> value () = { centerOfRotation .x (), centerOfRotation .y (), centerOfRotation .z () };
	}

	if (X3DBrowserWidget::save (worldURL, compressed, copy))
	{
		if (not copy)
			isModified (getBrowser (), false);
		
		return true;
	}

	return false;
}

std::string
X3DBrowserEditor::exportNodes (X3D::MFNode & nodes) const
{
	std::ostringstream osstream;

	exportNodes (osstream, nodes);

	return osstream .str ();

}

void
X3DBrowserEditor::exportNodes (std::ostream & ostream, X3D::MFNode & nodes) const
{
	// Find proto declarations

	const auto protoNodes = getUsedPrototypes (nodes);
	const auto routes     = getConnectedRoutes (nodes);

	// Generate text

	ostream .imbue (std::locale::classic ());

	ostream
		<< "#" << X3D::LATEST_VERSION << " utf8 " << getBrowser () -> getName ()
		<< std::endl
		<< std::endl
		<< '#' << getExecutionContext () -> getWorldURL ()
		<< std::endl
		<< std::endl;

	X3D::Generator::CompactStyle ();
	X3D::Generator::EnterScope ();

	if (not protoNodes .empty ())
	{
		for (const auto & protoNode : protoNodes)
			ostream << protoNode << std::endl;

		ostream << std::endl;
	}

	for (const auto & node : nodes)
		ostream << node << std::endl;

	if (not routes .empty ())
	{
		ostream << std::endl;

		for (const auto & route : routes)
			ostream << *route << std::endl;
	}

	X3D::Generator::LeaveScope ();
}

std::vector <X3D::X3DProtoDeclarationNodePtr>
X3DBrowserEditor::getUsedPrototypes (X3D::MFNode & nodes) const
{
	std::map <X3D::X3DProtoDeclarationNodePtr, size_t> protoIndex;

	X3D::traverse (nodes, [&] (X3D::SFNode & node)
	               {
	                  const X3D::X3DPrototypeInstancePtr protoInstance (node);

	                  if (protoInstance)
	                  {
	                     X3D::traverse (node, [&] (X3D::SFNode & node)
	                                    {
	                                       if (node == protoInstance)
															return true;

	                                       const X3D::X3DPrototypeInstancePtr child (node);

	                                       if (child)
	                                       {
	                                          try
	                                          {
	                                             if (child -> getProtoNode () == getExecutionContext () -> findProtoDeclaration (child -> getTypeName (), X3D::AvailableType { }))
																	protoIndex .emplace (child -> getProtoNode (), protoIndex .size ());
															}
	                                          catch (const X3D::X3DError &)
	                                          { }
														}

	                                       return true;
													},
	                                    true,
	                                    X3D::TRAVERSE_PROTOTYPE_INSTANCES);

	                     protoIndex .emplace (protoInstance -> getProtoNode (), protoIndex .size ());
							}

	                  return true;
						});

	std::vector <X3D::X3DProtoDeclarationNodePtr> protoNodes;

	for (const auto & protoNode : basic::reverse (protoIndex))
		protoNodes .emplace_back (std::move (protoNode .second));

	return protoNodes;
}

std::vector <X3D::Route*>
X3DBrowserEditor::getConnectedRoutes (X3D::MFNode & nodes) const
{
	// Create node index

	std::set <X3D::SFNode> nodeIndex;

	X3D::traverse (nodes, [&nodeIndex] (X3D::SFNode & node)
	               {
	                  nodeIndex .emplace (node);
	                  return true;
						});

	// Find connected routes

	std::vector <X3D::Route*> routes;

	X3D::traverse (nodes, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & field: node -> getFieldDefinitions ())
	                  {
	                     for (const auto & route: field -> getOutputRoutes ())
	                     {
	                        if (route -> getExecutionContext () not_eq getExecutionContext ())
	                           continue;

                           try
                           {
                              if (nodeIndex .count (route -> getDestinationNode ()))
											routes .emplace_back (route);
									}
                           catch (const X3D::X3DError &)
                           { }
								}
							}

	                  return true;
						});

	return routes;
}

void
X3DBrowserEditor::reload ()
{
	if (isSaved (getBrowser ()))
		X3DBrowserWidget::reload ();
}

void
X3DBrowserEditor::close (const X3D::BrowserPtr & browser)
{
	getWidget () .grab_focus ();

	if (isSaved (browser))
		X3DBrowserWidget::close (browser);
}

bool
X3DBrowserEditor::quit ()
{
	getWidget () .grab_focus ();

	for (const auto & browser : getBrowsers ())
	{
		if (isSaved (browser))
			continue;

		for (const auto & browser : getBrowsers ())
			getUserData (browser) -> saveConfirmed = false;

		// Cancel quit.
		return true;
	}

	return X3DBrowserWidget::quit ();
}

// Undo/Redo operations

X3D::UndoHistory &
X3DBrowserEditor::getUndoHistory (const X3D::BrowserPtr & browser)
{
   return getUserData (browser) -> undoHistory;
}

const X3D::UndoHistory &
X3DBrowserEditor::getUndoHistory (const X3D::BrowserPtr & browser) const
{
   return getUserData (browser) -> undoHistory;
}

void
X3DBrowserEditor::addUndoStep (const X3D::UndoStepPtr & undoStep)
{
	getUndoHistory (getBrowser ()) .addUndoStep (undoStep);
}

void
X3DBrowserEditor::addUndoStep (const X3D::BrowserPtr & browser, const X3D::UndoStepPtr & undoStep)
{
	getUndoHistory (browser) .addUndoStep (undoStep);
}

void
X3DBrowserEditor::undo ()
{
	getBrowser () -> grab_focus ();

	getUndoHistory (getBrowser ()) .undoChanges ();
}

void
X3DBrowserEditor::redo ()
{
	getBrowser () -> grab_focus ();

	getUndoHistory (getBrowser ()) .redoChanges ();
}

void
X3DBrowserEditor::set_undoHistory ()
{
	const auto & undoHistory = getUndoHistory (getBrowser ());

	if (undoHistory .hasUndo ())
	{
		getUndoMenuItem () .set_label (undoHistory .getUndoDescription ());
		getUndoButton ()   .set_tooltip_text (undoHistory .getUndoDescription ());
		getUndoMenuItem () .set_sensitive (true);
		getUndoButton ()   .set_sensitive (true);
	}
	else
	{
		getUndoMenuItem () .set_label (_ ("Undo"));
		getUndoButton ()   .set_tooltip_text (_ ("Undo last action (Ctrl-Z)."));
		getUndoMenuItem () .set_sensitive (false);
		getUndoButton ()   .set_sensitive (false);
	}

	if (undoHistory .hasRedo ())
	{
		getRedoMenuItem () .set_label (undoHistory .getRedoDescription ());
		getRedoButton ()   .set_tooltip_text (undoHistory .getRedoDescription ());
		getRedoMenuItem () .set_sensitive (true);
		getRedoButton ()   .set_sensitive (true);
	}
	else
	{
		getRedoMenuItem () .set_label (_ ("Redo"));
		getRedoButton ()   .set_tooltip_text (_ ("Redo last action (Ctrl-Shift-Z)."));
		getRedoMenuItem () .set_sensitive (false);
		getRedoButton ()   .set_sensitive (false);
	}

	setTitle ();
}

// Clipboard operations

void
X3DBrowserEditor::cutNodes (X3D::MFNode nodes, const X3D::UndoStepPtr & undoStep)
{
	// Detach from group

	detachFromGroup (getExecutionContext (), nodes, true, undoStep);

	// Set clipboard text

	Gtk::Clipboard::get () -> set_text (exportNodes (nodes));

	// Remove nodes

	removeNodesFromScene (getExecutionContext (), nodes, undoStep);
}

void
X3DBrowserEditor::copyNodes (X3D::MFNode nodes) const
{
	// Detach from group

	const auto undoDetachFromGroup = std::make_shared <X3D::UndoStep> ();

	const_cast <X3DBrowserEditor*> (this) -> detachFromGroup (getExecutionContext (), nodes, true, undoDetachFromGroup);

	// Set clipboard text

	Gtk::Clipboard::get () -> set_text (exportNodes (nodes));

	// Undo detach from group

	undoDetachFromGroup -> undoChanges ();
}

void
X3DBrowserEditor::pasteNodes (X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep)
{
	try
	{
		const Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

		if (clipboard -> wait_is_text_available ())
		{
			basic::ifilestream text (clipboard -> wait_for_text ());

			text .imbue (std::locale::classic ());

			std::string header;

			if (X3D::Grammar::Comment (text, header))
			{
				std::string encoding, specificationVersion, characterEncoding, comment;

				if (X3D::Grammar::Header .FullMatch (header, &encoding, &specificationVersion, &characterEncoding, &comment))
				{
					std::string whiteSpaces;

					X3D::Grammar::WhiteSpaces (text, whiteSpaces);

					std::string worldURL;

					if (X3D::Grammar::Comment (text, worldURL))
					{
						const auto scene = getBrowser () -> createX3DFromStream (worldURL, text);

						if (not MagicImport (getBrowserWindow ()) .import (nodes, scene, undoStep))
						{
							getSelection () -> setChildren (importScene (scene, getExecutionContext () -> getRootNodes (), undoStep),
							                                undoStep);
						}
					}
				}
			}
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserEditor::updatePasteStatus ()
{
	getPasteMenuItem () .set_sensitive (getPasteStatus ());
}

bool
X3DBrowserEditor::getPasteStatus () const
{
	const Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

	if (clipboard -> wait_is_text_available ())
	{
		std::istringstream text (clipboard -> wait_for_text ());

		text .imbue (std::locale::classic ());

		std::string header;

		if (X3D::Grammar::Comment (text, header))
		{
			std::string encoding, specificationVersion, characterEncoding, comment;

			if (X3D::Grammar::Header .FullMatch (header, &encoding, &specificationVersion, &characterEncoding, &comment))
				return true;
		}
	}

	return false;
}

// Edit operations

X3D::SFNode
X3DBrowserEditor::createNode (const std::string & typeName, const X3D::UndoStepPtr & undoStep)
{
	const auto & activeLayer = getWorld () -> getActiveLayer ();
	auto &       children    = activeLayer and activeLayer not_eq getWorld () -> getLayer0 ()
	                           ? activeLayer -> children ()
	                           : getExecutionContext () -> getRootNodes ();

	undoStep -> addObjects (getExecutionContext (), activeLayer);

	const auto node = getExecutionContext () -> createNode (typeName);
	children .emplace_back (node);
	getExecutionContext () -> addUninitializedNode (node);
	getExecutionContext () -> realize ();
	getBrowserWindow () -> getSelection () -> setChildren ({ node }, undoStep);

	const auto removeUndoStep = std::make_shared <X3D::UndoStep> ("");

	removeNodesFromScene (getExecutionContext (), { node }, removeUndoStep);
	undoStep -> addUndoFunction (&X3D::UndoStep::redoChanges, removeUndoStep);
	removeUndoStep -> undoChanges ();
	undoStep -> addRedoFunction (&X3D::UndoStep::undoChanges, removeUndoStep);
	return node;
}

/***
 *  Completely removes @a nodes from @a executionContext.
 */
void
X3DBrowserEditor::removeNodesFromScene (const X3D::X3DExecutionContextPtr & executionContext, const X3D::MFNode & nodes, const X3D::UndoStepPtr & undoStep, const bool doRemoveFromSceneGraph) const
{
	getSelection () -> removeChildren (nodes, undoStep);

	X3D::X3DBrowserEditor::removeNodesFromScene (executionContext, nodes, undoStep, doRemoveFromSceneGraph);
}

// Undo functions

void
X3DBrowserEditor::translateSelection (const X3D::Vector3f & offset, const bool alongFrontPlane, const ToolType currentTool)
{
	using setValue = void (X3D::SFVec3f::*) (const X3D::Vector3f &);

	static const std::vector <const char*> undoText = {
		"Nudge Left",
		"Nudge Right",
		"Nudge Up",
		"Nudge Down",
		"Nudge Front",
		"Nudge Back"
	};

	for (const auto & node : basic::make_reverse_range (getSelection () -> getChildren ()))
	{
		X3D::X3DTransformNodePtr transform (node);

		if (transform)
		{
			if (currentTool not_eq tool or chrono::now () - undoTime > UNDO_TIME or undoStep not_eq getBrowserWindow () -> getUndoStep ())
				undoStep = std::make_shared <X3D::UndoStep> (_ (undoText [currentTool - NUDGE_LEFT]));

			tool     = currentTool;
			undoTime = chrono::now ();

			getSelection () -> redoRestoreSelection (undoStep);

			if (transform -> getKeepCenter ())
			{
				X3D::Matrix4d matrix;
				matrix .set (offset);
				matrix = transform -> getCurrentMatrix () * matrix;

				undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getMatrix (), transform -> center ());
				undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixKeepCenter, transform, matrix);
				transform -> setMatrixKeepCenter (matrix);
				
				// If we use setMatrixKeepCenter we must do the group translation by ourself.

				matrix .set (offset);
				matrix *= transform -> getTransformationMatrix ();
				
				bool first = true;
	
				for (const auto & node : basic::make_reverse_range (getSelection () -> getChildren ()))
				{
					X3D::X3DTransformNodePtr transform (node);

					if (not transform)
						continue;

					if (first)
					{
						first = false;
						continue;
					}

					undoStep -> addUndoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getMatrix (), transform -> center ());
					transform -> addAbsoluteMatrix (matrix, transform -> getKeepCenter ());
					undoStep -> addRedoFunction (&X3D::X3DTransformNode::setMatrixWithCenter, transform, transform -> getCurrentMatrix (), transform -> center ());
				}
			}
			else
			{
				undoStep -> addObjects (node);	
				undoStep -> addUndoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (transform -> translation ()), transform -> translation ());
				undoStep -> addRedoFunction ((setValue) & X3D::SFVec3f::setValue, std::ref (transform -> translation ()), transform -> translation () + offset);
				transform -> translation () += offset;
			}

			getSelection () -> undoRestoreSelection (undoStep);

			if (undoStep not_eq getBrowserWindow () -> getUndoStep ())
				addUndoStep (undoStep);
			break;
		}
	}
}

// Misc

void
X3DBrowserEditor::editCDATA (const X3D::SFNode & node)
{
	X3D::MFString* const cdata          = node -> getCDATA ();
	std::string          filename       = "/tmp/titania-XXXXXX.js";
	const int            fileDescriptor = mkstemps (&filename [0], 3);

	if (not cdata or fileDescriptor == -1)
		return;

	// Output file.

	std::ofstream ostream (filename);

	for (const auto & string : *cdata)
	{
		ostream
			<< "<![CDATA["
			<< X3D::escape_cdata (string)
			<< "]]>" << std::endl
			<< std::endl
			<< std::endl;
	}

	std::string name = node -> getName ();
	pcrecpp::RE (R"/((\*/))/") .GlobalReplace ("", &name);

	ostream
		<< "/**" << std::endl
		<< " * " << node -> getTypeName () << " " << name << std::endl
		<< " * " << _ ("This file is automaticaly generated to edit CDATA fields. Each SFString value is enclosed inside a CDATA") << std::endl
		<< " * " << _ ("section.  A CDATA section starts with \"<![CDATA[\" and ends with \"]]>\".") << std::endl
		<< " * " << std::endl
		<< " * " << _ ("If this is a Script node, a inline script must be enclosed in \"<![CDATA[javascript: your program here ]]>\".") << std::endl
		<< " * " << _ ("If this is a shader, it must be enclosed in \"<![CDATA[data:text/plain, your shader here ]]>\".  Just save") << std::endl
		<< " * " << _ ("this file to apply changes.") << std::endl
		<< " **/" << std::endl;

	// Launch Gnome TextEditor.

	Glib::RefPtr <Gio::File>        file        = Gio::File::create_for_path (filename);
	Glib::RefPtr <Gio::FileMonitor> fileMonitor = file -> monitor_file ();

	fileMonitor -> signal_changed () .connect (sigc::bind (sigc::mem_fun (*this, &X3DBrowserEditor::on_cdata_changed), node));
	getUserData (getBrowser ()) -> fileMonitors .emplace (file, fileMonitor);

	try
	{
		Gio::AppInfo::create_from_commandline (os::realpath ("/usr/bin/gnome-text-editor"), "", Gio::APP_INFO_CREATE_NONE) -> launch (file);
	}
	catch (...)
	{ }

	::close (fileDescriptor);
}

void
X3DBrowserEditor::on_cdata_changed (const Glib::RefPtr <Gio::File> & file, const Glib::RefPtr <Gio::File> &, Gio::FileMonitorEvent event, const X3D::SFNode & node)
{
	io::multi_line_comment comment ("/*", "*/");
	io::sequence           whitespaces ("\r\n \t,");
	io::inverse_string     cdata_start ("<![CDATA[");
	io::inverse_string     contents ("]]>");

	if (event not_eq Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT)
		return;

	// Parse file.

	X3D::MFString string;
	std::string   ws;

	std::ifstream istream (file -> get_path ());

	// Parse CDATA sections.

	while (istream)
	{
		whitespaces (istream, ws);
		comment (istream, ws);

		if (cdata_start (istream, ws))
		{
			std::string value;

			contents (istream, value);

			string .emplace_back (std::move (value));
		}
		else
			break;
	}

	// Set value.

	X3D::MFString* const cdata = node -> getCDATA ();

	if (string not_eq *cdata)
	{
		const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Edit Field »%s«"), cdata -> getName () .c_str ()));

		undoStep -> addObjects (node);

		undoStep -> addUndoFunction (&X3D::MFString::setValue, cdata, *cdata);
		undoStep -> addRedoFunction (&X3D::MFString::setValue, cdata, string);
		cdata -> setValue (string);

		addUndoStep (undoStep);
	}

	getBrowser () -> println (X3D::SFTime (chrono::now ()) .toUTCString (), ": ", basic::sprintf (_ ("Script »%s« saved."), node -> getName () .c_str ()));
}

X3DBrowserEditor::~X3DBrowserEditor ()
{ }

} // puck
} // titania
