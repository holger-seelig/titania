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

#include "X3DBrowserPanel.h"

#include "../NotebookPage/NotebookPage.h"
#include "../../Editors/BackgroundImageEditor/BackgroundImage.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Editors/GridEditor/AngleGridTool.h"
#include "../../Editors/GridEditor/AxonometricGridTool.h"
#include "../../Editors/GridEditor/GridTool.h"

#include <Titania/X3D/Browser/Navigation/PlaneViewer.h>
#include <Titania/X3D/Browser/Selection.h>
#include <Titania/X3D/Components/Grouping/Group.h>
#include <Titania/X3D/Components/Grouping/Switch.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Layering/X3DLayerNode.h>
#include <Titania/X3D/Components/Navigation/NavigationInfo.h>
#include <Titania/X3D/Components/Navigation/OrthoViewpoint.h>
#include <Titania/X3D/Components/Navigation/Viewpoint.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Material.h>
#include <Titania/X3D/Components/Texturing/ImageTexture.h>
#include <Titania/X3D/Execution/BindableNodeStack.h>
#include <Titania/X3D/Routing/Router.h>

#include <Titania/X3D/Tools/Grids/GridTool.h>
#include <Titania/X3D/Tools/Grids/AngleGridTool.h>
#include <Titania/X3D/Tools/Grids/AxonometricGridTool.h>

#include <cassert>

namespace titania {
namespace puck {

const std::vector <BrowserPanelType> X3DBrowserPanel::defaultTypes = {
	BrowserPanelType::TOP_VIEW,
	BrowserPanelType::MAIN_VIEW,
	BrowserPanelType::RIGHT_VIEW,
	BrowserPanelType::FRONT_VIEW,
};

const std::map <BrowserPanelType, std::string> X3DBrowserPanel::names = {
	std::make_pair (BrowserPanelType::MAIN_VIEW,        ""),
	std::make_pair (BrowserPanelType::PERSPECTIVE_VIEW, "Perspective"),
	std::make_pair (BrowserPanelType::TOP_VIEW,         "Top"),
	std::make_pair (BrowserPanelType::RIGHT_VIEW,       "Right"),
	std::make_pair (BrowserPanelType::FRONT_VIEW,       "Front"),
	std::make_pair (BrowserPanelType::BOTTOM_VIEW,      "Bottom"),
	std::make_pair (BrowserPanelType::LEFT_VIEW,        "Left"),
	std::make_pair (BrowserPanelType::BACK_VIEW,        "Back")
};

const std::map <BrowserPanelType, X3D::Vector3d> X3DBrowserPanel::axes = {
	std::make_pair (BrowserPanelType::MAIN_VIEW,        X3D::Vector3f ()),
	std::make_pair (BrowserPanelType::PERSPECTIVE_VIEW, X3D::Vector3f ()),
	std::make_pair (BrowserPanelType::TOP_VIEW,         X3D::Vector3f (0, 1, 0)),
	std::make_pair (BrowserPanelType::RIGHT_VIEW,       X3D::Vector3f (1, 0, 0)),
	std::make_pair (BrowserPanelType::FRONT_VIEW,       X3D::Vector3f (0, 0, 1)),
	std::make_pair (BrowserPanelType::BOTTOM_VIEW,      X3D::Vector3f (0, -1, 0)),
	std::make_pair (BrowserPanelType::LEFT_VIEW,        X3D::Vector3f (-1, 0, 0)),
	std::make_pair (BrowserPanelType::BACK_VIEW,        X3D::Vector3f (0, 0, -1))
};

const std::map <BrowserPanelType, X3D::Vector3d> X3DBrowserPanel::positions = {
	std::make_pair (BrowserPanelType::MAIN_VIEW,        X3D::Vector3f ()),
	std::make_pair (BrowserPanelType::PERSPECTIVE_VIEW, X3D::Vector3f (8.48219, 12, 8.48219)),
	std::make_pair (BrowserPanelType::TOP_VIEW,         X3D::Vector3f (0, 10, 0)),
	std::make_pair (BrowserPanelType::RIGHT_VIEW,       X3D::Vector3f (10, 0, 0)),
	std::make_pair (BrowserPanelType::FRONT_VIEW,       X3D::Vector3f (0, 0, 10)),
	std::make_pair (BrowserPanelType::BOTTOM_VIEW,      X3D::Vector3f (0, -10, 0)),
	std::make_pair (BrowserPanelType::LEFT_VIEW,        X3D::Vector3f (-10, 0, 0)),
	std::make_pair (BrowserPanelType::BACK_VIEW,        X3D::Vector3f (0, 0, -10))
};

const std::map <BrowserPanelType, X3D::Rotation4d> X3DBrowserPanel::orientations = {
	std::make_pair (BrowserPanelType::MAIN_VIEW,        X3D::Rotation4f ()),
	std::make_pair (BrowserPanelType::PERSPECTIVE_VIEW, X3D::Rotation4f (1, 0, 0, -math::pi <float> / 4) * X3D::Rotation4f (0, 1, 0, math::pi <float> / 4)),
	std::make_pair (BrowserPanelType::TOP_VIEW,         X3D::Rotation4f (1, 0, 0, -math::pi <float> / 2)),
	std::make_pair (BrowserPanelType::RIGHT_VIEW,       X3D::Rotation4f (0, 1, 0, math::pi <float> / 2)),
	std::make_pair (BrowserPanelType::FRONT_VIEW,       X3D::Rotation4f ()),
	std::make_pair (BrowserPanelType::BOTTOM_VIEW,      X3D::Rotation4f (1, 0, 0, math::pi <float> / 2) * X3D::Rotation4f (0, 1, 0, math::pi <float>)),
	std::make_pair (BrowserPanelType::LEFT_VIEW,        X3D::Rotation4f (0, 1, 0, -math::pi <float> / 2)),
	std::make_pair (BrowserPanelType::BACK_VIEW,        X3D::Rotation4f (0, 1, 0, math::pi <float>))
};

X3DBrowserPanel::X3DBrowserPanel () :
	X3DBrowserPanelInterface (),
	                    type (defaultTypes [getId ()]),
	                 browser (getPage () -> getMainBrowser ()),
	             activeLayer (),
	               viewpoint (),
	           gridTransform (),
	              gridSwitch (),
	                    grid ()
{
	addChildObjects (browser,
	                 activeLayer,
	                 viewpoint,
	                 gridTransform,
	                 gridSwitch,
	                 grid);

}

void
X3DBrowserPanel::initialize ()
{
	X3DBrowserPanelInterface::initialize ();

	type = getBrowserPanelType (getId ());

	set_type ();
}

X3D::BrowserPtr
X3DBrowserPanel::createBrowser (const BrowserPanelType type)
{
	if (type == BrowserPanelType::MAIN_VIEW)
		return getPage () -> getMainBrowser ();

	const auto browser = X3D::createBrowser (getPage () -> getMainBrowser (), { get_ui ("Panels/BrowserPanel.x3dv") });

	// addDependentBrowser
	// Setup dependent browser.

	getPage () -> getMainBrowser () -> getFixedPipeline () .addInterest (&X3DBrowserPanel::set_fixed_pipeline, this);
	getPage () -> getMainBrowser () -> getViewer ()        .addInterest (&X3DBrowserPanel::set_viewer,         this);

	browser -> setFrameRate (30);
	browser -> setRouter    (getPage () -> getMainBrowser () -> getRouter ());
	browser -> setSelection (getPage () -> getMainBrowser () -> getSelection ());

	browser -> sensors () .addInterest (&X3DBrowserPanel::set_sensors, this);

	set_fixed_pipeline ();
	set_viewer ();

	return browser;
}

void
X3DBrowserPanel::setBrowserPanelType (const size_t id, const BrowserPanelType browserPanelType)
{
	static const std::map <BrowserPanelType, std::string> browserPanelTypes = {
		std::make_pair (BrowserPanelType::MAIN_VIEW,        "MAIN_VIEW"),
		std::make_pair (BrowserPanelType::PERSPECTIVE_VIEW, "PERSPECTIVE_VIEW"),
		std::make_pair (BrowserPanelType::TOP_VIEW,         "TOP_VIEW"),
		std::make_pair (BrowserPanelType::RIGHT_VIEW,       "RIGHT_VIEW"),
		std::make_pair (BrowserPanelType::FRONT_VIEW,       "FRONT_VIEW"),
		std::make_pair (BrowserPanelType::BOTTOM_VIEW,      "BOTTOM_VIEW"),
		std::make_pair (BrowserPanelType::LEFT_VIEW,        "LEFT_VIEW"),
		std::make_pair (BrowserPanelType::BACK_VIEW,        "BACK_VIEW"),
	};

	auto browserPanelTypeArray = createWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/BrowserPanel/type");

	browserPanelTypeArray .resize (4);

	browserPanelTypeArray [id] = browserPanelTypes .at (browserPanelType);

	createWorldInfo (getPage () -> getScene ()) -> setMetaData ("/Titania/BrowserPanel/type", browserPanelTypeArray);

	getPage () -> setModified (true);
}

BrowserPanelType
X3DBrowserPanel::getBrowserPanelType (const size_t id) const
{
	try
	{
		static const std::map <std::string, BrowserPanelType> browserPanelTypes = {
			std::make_pair ("MAIN_VIEW",        BrowserPanelType::MAIN_VIEW),
			std::make_pair ("PERSPECTIVE_VIEW", BrowserPanelType::PERSPECTIVE_VIEW),
			std::make_pair ("TOP_VIEW",         BrowserPanelType::TOP_VIEW),
			std::make_pair ("RIGHT_VIEW",       BrowserPanelType::RIGHT_VIEW),
			std::make_pair ("FRONT_VIEW",       BrowserPanelType::FRONT_VIEW),
			std::make_pair ("BOTTOM_VIEW",      BrowserPanelType::BOTTOM_VIEW),
			std::make_pair ("LEFT_VIEW",        BrowserPanelType::LEFT_VIEW),
			std::make_pair ("BACK_VIEW",        BrowserPanelType::BACK_VIEW),
		};

		const auto browserPanelTypeArray = getWorldInfo (getPage () -> getScene ()) -> getMetaData <X3D::MFString> ("/Titania/BrowserPanel/type");

		return browserPanelTypes .at (browserPanelTypeArray .at (id));
	}
	catch (const std::exception &)
	{
		return defaultTypes [id];
	}
}

void
X3DBrowserPanel::setType (const BrowserPanelType value)
{
	type = value;

	setBrowserPanelType (getId (), value);

	set_type ();
}

void
X3DBrowserPanel::set_type ()
{
	switch (type)
	{
		case BrowserPanelType::MAIN_VIEW:
			getCameraMenuItem () .set_label (_ ("Main View"));  
			break;
		case BrowserPanelType::PERSPECTIVE_VIEW:
			getCameraMenuItem () .set_label (_ ("Perspective View"));  
			break;
		case BrowserPanelType::TOP_VIEW:
			getCameraMenuItem () .set_label (_ ("Top View"));  
			break;
		case BrowserPanelType::BOTTOM_VIEW:
			getCameraMenuItem () .set_label (_ ("Bottom View"));  
			break;
		case BrowserPanelType::LEFT_VIEW:
			getCameraMenuItem () .set_label(_ ("Left View"));  
			break;
		case BrowserPanelType::RIGHT_VIEW:
			getCameraMenuItem () .set_label(_ ("Right View"));  
			break;
		case BrowserPanelType::FRONT_VIEW:
			getCameraMenuItem () .set_label (_ ("Front View"));  
			break;
		case BrowserPanelType::BACK_VIEW:
			getCameraMenuItem () .set_label (_ ("Back View"));  
			break;
	}

	setLocalBrowser (createBrowser (type));
}

BrowserPanelType
X3DBrowserPanel::getType () const
{
	return type;
}

void
X3DBrowserPanel::setLocalBrowser (const X3D::BrowserPtr & value)
{
	if (browser)
		getBrowserBox () .remove (*browser);

	browser = value;

	if (type not_eq BrowserPanelType::MAIN_VIEW)
	{
		browser -> initialized () .addInterest (&X3DBrowserPanel::set_dependent_browser, this);
		browser -> set_opacity (0);
		browser -> setName (names .at (type));
	}

	if (getWidget () .get_mapped ())
		X3DBrowserPanel::on_map ();

	browser -> signal_focus_out_event () .connect (sigc::mem_fun ((X3DPanelInterface*) this, &X3DPanelInterface::on_focus_out_event));
	browser -> signal_focus_in_event ()  .connect (sigc::mem_fun ((X3DPanelInterface*) this, &X3DPanelInterface::on_focus_in_event));
	browser -> setAntialiasing (4);
	browser -> show ();

	getBrowserBox () .pack_start (*browser, true, true, 0);
}

int32_t
X3DBrowserPanel::getPlane () const
{
	switch (type)
	{
		case BrowserPanelType::MAIN_VIEW:
		case BrowserPanelType::PERSPECTIVE_VIEW:
			return -1;
		case BrowserPanelType::TOP_VIEW:
		case BrowserPanelType::BOTTOM_VIEW:
			return 1;
		case BrowserPanelType::RIGHT_VIEW:
		case BrowserPanelType::LEFT_VIEW:
			return 0;
		case BrowserPanelType::FRONT_VIEW:
		case BrowserPanelType::BACK_VIEW:
			return 2;
	}

	return -1;
}

void
X3DBrowserPanel::set_dependent_browser ()
{
	try
	{
		// Setup dependent browser.

		browser -> initialized () .removeInterest (&X3DBrowserPanel::set_dependent_browser, this);
		browser -> set_opacity (1);

		// Setup scene.

		const auto & executionContext     = browser -> getExecutionContext ();
		const auto   gridLayer            = executionContext -> getNamedNode <X3D::X3DLayerNode> ("GridLayer");
		const auto   layer                = executionContext -> getNamedNode <X3D::X3DLayerNode> ("Layer");
		const auto   backgroundAppearance = executionContext -> getNamedNode <X3D::Appearance> ("BackgroundImageAppearance");

		const auto & gridTool            = getBrowserWindow () -> getGridTool ()            -> getTool ();
		const auto & angleGridTool       = getBrowserWindow () -> getAngleGridTool ()       -> getTool ();
		const auto & axonometricGridTool = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();

		getPage () -> getBackgroundImage () -> getTexture () -> checkLoadState () .addInterest (&X3DBrowserPanel::set_background_texture, this);
		getPage () -> getBackgroundImage () -> getTransparency () .addInterest (&X3DBrowserPanel::set_background_texture_transparency, this);
		backgroundAppearance -> texture () = getPage () -> getBackgroundImage () -> getTexture ();

		gridTransform = executionContext -> getNamedNode <X3D::Transform> ("GridTransform");
		gridSwitch    = executionContext -> getNamedNode <X3D::Switch> ("GridSwitch");
		grid          = executionContext -> getNamedNode ("Grid");

		layer -> getNavigationInfoStack () -> setLock (true);
		layer -> getViewpointStack ()      -> setLock (true);
		layer -> getBackgroundStack ()     -> setLock (true);
		layer -> getFogStack ()            -> setLock (true);

		gridSwitch -> children () .emplace_back (gridTool            -> getTool ());
		gridSwitch -> children () .emplace_back (angleGridTool       -> getTool ());
		gridSwitch -> children () .emplace_back (axonometricGridTool -> getTool ());

		if (type == BrowserPanelType::PERSPECTIVE_VIEW)
		{
			executionContext -> getNamedNode <X3D::NavigationInfo> ("Viewer") -> type () = { "EXAMINE" };
			viewpoint = executionContext -> getNamedNode <X3D::Viewpoint> ("PerspectiveViewpoint");
		}
		else
			viewpoint = executionContext -> getNamedNode <X3D::OrthoViewpoint> ("OrthoViewpoint");

		gridLayer -> getViewpointStack () -> pushOnTop (viewpoint, true);
		layer     -> getViewpointStack () -> pushOnTop (viewpoint, true);

		viewpoint -> setPosition (positions .at (type));
		viewpoint -> setOrientation (orientations .at (type));

		grid -> setField <X3D::SFRotation> ("rotation", X3D::Rotation4d (1, 0, 0, math::pi <double> / 2) * orientations .at (type));

		// Connect to active layer.

		getPage () -> getMainBrowser () -> getActiveLayer () .addInterest (&X3DBrowserPanel::set_activeLayer, this);

		set_background_texture ();
		set_background_texture_transparency ();
		set_activeLayer ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserPanel::set_fixed_pipeline ()
{
	browser -> setFixedPipeline (getPage () -> getMainBrowser () -> getFixedPipeline ());
}

void
X3DBrowserPanel::set_viewer ()
{
	const auto & mainViewer = getPage () -> getMainBrowser () -> getViewer ();

	if (mainViewer -> isType ({ X3D::X3DConstants::X3DSelector, X3D::X3DConstants::LightSaber }))
	{
		browser -> setPrivateViewer (mainViewer -> getType () .back ());
	}
	else
	{
		browser -> setPrivateViewer (X3D::X3DConstants::DefaultViewer);
	}
}

void
X3DBrowserPanel::set_sensors ()
{
	getPage () -> getMainBrowser () -> sensors () .processInterests ();
}

void
X3DBrowserPanel::set_background_texture ()
{
	try
	{
		const auto & executionContext = browser -> getExecutionContext ();
		const auto   imageSwitch      = executionContext -> getNamedNode <X3D::Switch> ("BackgroundImageSwitch");

		if (getPage () -> getBackgroundImage () -> getTexture () -> checkLoadState () == X3D::COMPLETE_STATE)
		{
			const auto   transform = executionContext -> getNamedNode <X3D::Transform> ("BackgroundImageTransform");
			const double width     = getPage () -> getBackgroundImage () -> getTexture () -> getImageWidth ();
			const double height    = getPage () -> getBackgroundImage () -> getTexture () -> getImageHeight ();

			transform -> scale ()         = X3D::Vector3f (width / height, 1, 1);
			imageSwitch -> whichChoice () = 0;
		}
		else
			imageSwitch -> whichChoice () = -1;
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserPanel::set_background_texture_transparency ()
{
	try
	{
		const auto & executionContext = browser -> getExecutionContext ();
		const auto   material         = executionContext -> getNamedNode <X3D::Material> ("BackgroundImageMaterial");

		material -> transparency () = getPage () -> getBackgroundImage () -> getTransparency ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserPanel::set_activeLayer ()
{
	try
	{
		if (activeLayer)
		{
			const auto group = browser -> getExecutionContext () -> getNamedNode <X3D::Group> ("Group");

			activeLayer -> children () .removeInterest (group -> children ());
		}
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	activeLayer = getPage () -> getMainBrowser () -> getActiveLayer ();

	try
	{
		const auto group = browser -> getExecutionContext () -> getNamedNode <X3D::Group> ("Group");

		group -> setPrivate (true);

		if (activeLayer)
		{
			const auto userPosition         = getMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/position", positions .at (type));
			const auto userOrientation      = getMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/orientation", orientations .at (type));
			const auto userCenterOfRotation = getMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/centerOfRotation", X3D::Vector3d ());
			const auto fieldOfViewScale     = getMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/fieldOfViewScale", 1.0);

			activeLayer -> children () .addInterest (group -> children ());
			group -> children () = activeLayer -> children ();

			viewpoint -> addInterest (&X3DBrowserPanel::connectViewpoint, this);

			viewpoint -> positionOffset ()         = userPosition - viewpoint -> getPosition ();
			viewpoint -> orientationOffset ()      = ~viewpoint -> getOrientation () * userOrientation;
			viewpoint -> centerOfRotationOffset () = userCenterOfRotation - viewpoint -> getCenterOfRotation ();
			viewpoint -> fieldOfViewScale ()       = fieldOfViewScale;
		}
		else
		{
			group -> children () .clear ();
		}

		set_grid ();
	}
	catch (const X3D::X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DBrowserPanel::connectViewpoint ()
{
	viewpoint -> removeInterest (&X3DBrowserPanel::connectViewpoint, this);
	viewpoint -> addInterest (&X3DBrowserPanel::set_viewpoint, this);

	browser -> getExecutionContext () -> getNamedNode <X3D::NavigationInfo> ("Viewer") -> transitionType () = { "ANIMATE" };
}

void
X3DBrowserPanel::set_viewpoint ()
{
	setMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/position",         viewpoint -> getUserPosition ());
	setMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/orientation",      viewpoint -> getUserOrientation ());
	setMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/centerOfRotation", viewpoint -> getUserCenterOfRotation ());
	setMetaData (getPage () -> getMainBrowser (), "/Titania/BrowserPanel/viewpoints/" + names .at (type) + "Viewpoint/fieldOfViewScale", viewpoint -> fieldOfViewScale ());

	getPage () -> setModified (true);

	if (type not_eq BrowserPanelType::PERSPECTIVE_VIEW)
		gridTransform -> translation () = viewpoint -> getUserPosition () * axes .at (type) - X3D::Vector3d (0, 0, 10) * viewpoint -> getUserOrientation ();
}

void
X3DBrowserPanel::set_grid ()
{
	try
	{
		// <plane, type> = <x, y>
		static const std::map <std::pair <int32_t, BrowserPanelType>, X3D::Vector2i> mappings = {
			// x-plane
			std::make_pair (std::make_pair (0, BrowserPanelType::TOP_VIEW),   X3D::Vector2i (1, 0)),
			std::make_pair (std::make_pair (0, BrowserPanelType::RIGHT_VIEW), X3D::Vector2i (0, 2)),
			std::make_pair (std::make_pair (0, BrowserPanelType::FRONT_VIEW), X3D::Vector2i (1, 2)),
			// y-plane
			std::make_pair (std::make_pair (1, BrowserPanelType::TOP_VIEW),   X3D::Vector2i (0, 2)),
			std::make_pair (std::make_pair (1, BrowserPanelType::RIGHT_VIEW), X3D::Vector2i (2, 1)),
			std::make_pair (std::make_pair (1, BrowserPanelType::FRONT_VIEW), X3D::Vector2i (0, 1)),
			// z-plane
			std::make_pair (std::make_pair (2, BrowserPanelType::TOP_VIEW),   X3D::Vector2i (0, 1)),
			std::make_pair (std::make_pair (2, BrowserPanelType::RIGHT_VIEW), X3D::Vector2i (1, 2)),
			std::make_pair (std::make_pair (2, BrowserPanelType::FRONT_VIEW), X3D::Vector2i (0, 2)),
			// x-plane
			std::make_pair (std::make_pair (0, BrowserPanelType::BOTTOM_VIEW), X3D::Vector2i (1, 0)),
			std::make_pair (std::make_pair (0, BrowserPanelType::LEFT_VIEW),   X3D::Vector2i (0, 2)),
			std::make_pair (std::make_pair (0, BrowserPanelType::BACK_VIEW),   X3D::Vector2i (1, 2)),
			// y-plane
			std::make_pair (std::make_pair (1, BrowserPanelType::BOTTOM_VIEW), X3D::Vector2i (0, 2)),
			std::make_pair (std::make_pair (1, BrowserPanelType::LEFT_VIEW),   X3D::Vector2i (2, 1)),
			std::make_pair (std::make_pair (1, BrowserPanelType::BACK_VIEW),   X3D::Vector2i (0, 1)),
			// z-plane
			std::make_pair (std::make_pair (2, BrowserPanelType::BOTTOM_VIEW), X3D::Vector2i (0, 1)),
			std::make_pair (std::make_pair (2, BrowserPanelType::LEFT_VIEW),   X3D::Vector2i (1, 2)),
			std::make_pair (std::make_pair (2, BrowserPanelType::BACK_VIEW),   X3D::Vector2i (0, 2)),
		};
	
		if (getPage () -> getMainBrowser () not_eq getCurrentBrowser ())
			return;
	
		if (getBrowserWindow () -> getGridTool () -> getVisible ())
		{
			const auto plane = getBrowserWindow () -> getGridTool () -> getPlane ();
	
			if (type == BrowserPanelType::PERSPECTIVE_VIEW or plane == getPlane ())
			{
				gridSwitch -> whichChoice () = 1;
			}
			else
			{
				const auto & tool    = getBrowserWindow () -> getGridTool () -> getTool ();
				const auto   mapping = mappings .at (std::make_pair (plane, type));
				const auto   one     = X3D::Vector3f (1, 1, 1) - X3D::Vector3f (max (axes .at (type), -axes .at (type)));

				gridSwitch -> whichChoice () = 0;

				grid -> setField <X3D::SFVec3f> ("translation", tool -> translation () * one);
				grid -> setField <X3D::SFVec3f> ("scale",       tool -> scale ());
				grid -> setField <X3D::MFInt32> ("dimension",   X3D::MFInt32 ({ tool -> dimension () .at (mapping .x ()), 0, tool -> dimension () .at (mapping .y ()) }));
	
				for (size_t i = 0; i < tool -> majorLineEvery () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 0, tool -> majorLineEvery () .at (i + mapping .x ()));
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 2, tool -> majorLineEvery () .at (i + mapping .y ()));
				}
	
				for (size_t i = 0; i < tool -> majorLineOffset () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 0, tool -> majorLineOffset () .at (i + mapping .x ()));
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 2, tool -> majorLineOffset () .at (i + mapping .y ()));
				}
	
				grid -> setField <X3D::SFColor> ("color",          X3D::Color3f (tool -> color ()          .getRed (), tool -> color ()          .getGreen (), tool -> color ()          .getBlue ()));
				grid -> setField <X3D::SFColor> ("lineColor",      X3D::Color3f (tool -> lineColor ()      .getRed (), tool -> lineColor ()      .getGreen (), tool -> lineColor ()      .getBlue ()));
				grid -> setField <X3D::SFColor> ("majorLineColor", X3D::Color3f (tool -> majorLineColor () .getRed (), tool -> majorLineColor () .getGreen (), tool -> majorLineColor () .getBlue ()));
	
				grid -> setField <X3D::SFFloat> ("transparency",          1 - tool -> color ()          .getAlpha ());
				grid -> setField <X3D::SFFloat> ("lineTransparency",      1 - tool -> lineColor ()      .getAlpha ());
				grid -> setField <X3D::SFFloat> ("majorLineTransparency", 1 - tool -> majorLineColor () .getAlpha ());
			}
		}
		else if (getBrowserWindow () -> getAngleGridTool () -> getVisible ())
		{
			const auto plane = getBrowserWindow () -> getAngleGridTool () -> getPlane ();

			if (type == BrowserPanelType::PERSPECTIVE_VIEW or plane == getPlane ())
			{
				gridSwitch -> whichChoice () = 2;
			}
			else
			{
				const auto & tool = getBrowserWindow () -> getAngleGridTool () -> getTool ();
				const auto   one  = X3D::Vector3f (1, 1, 1) - X3D::Vector3f (max (axes .at (type), -axes .at (type)));

				gridSwitch -> whichChoice () = 0;
	
				grid -> setField <X3D::SFVec3f> ("translation", tool -> translation () * one);
				grid -> setField <X3D::SFVec3f> ("scale",       tool -> scale ());
				grid -> setField <X3D::MFInt32> ("dimension",   X3D::MFInt32 ({ tool -> dimension () .at (2), 0, tool -> dimension () .at (2) }));
	
				for (size_t i = 0; i < tool -> majorLineEvery () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i + 2, tool -> majorLineEvery () .at (i + 2));
				}
	
				for (size_t i = 0; i < tool -> majorLineOffset () .size (); i += 3)
				{
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i + 2, tool -> majorLineOffset () .at (i + 2));
				}
	
				grid -> setField <X3D::SFColor> ("color",          X3D::Color3f (tool -> color ()          .getRed (), tool -> color ()          .getGreen (), tool -> color ()          .getBlue ()));
				grid -> setField <X3D::SFColor> ("lineColor",      X3D::Color3f (tool -> lineColor ()      .getRed (), tool -> lineColor ()      .getGreen (), tool -> lineColor ()      .getBlue ()));
				grid -> setField <X3D::SFColor> ("majorLineColor", X3D::Color3f (tool -> majorLineColor () .getRed (), tool -> majorLineColor () .getGreen (), tool -> majorLineColor () .getBlue ()));
	
				grid -> setField <X3D::SFFloat> ("transparency",          1 - tool -> color ()          .getAlpha ());
				grid -> setField <X3D::SFFloat> ("lineTransparency",      1 - tool -> lineColor ()      .getAlpha ());
				grid -> setField <X3D::SFFloat> ("majorLineTransparency", 1 - tool -> majorLineColor () .getAlpha ());
			}
		}
		else if (getBrowserWindow () -> getAxonometricGridTool () -> getVisible ())
		{
			const auto plane = getBrowserWindow () -> getAxonometricGridTool () -> getPlane ();

			if (type == BrowserPanelType::PERSPECTIVE_VIEW or plane == getPlane ())
			{
				gridSwitch -> whichChoice () = 3;
			}
			else
			{
				const auto & tool = getBrowserWindow () -> getAxonometricGridTool () -> getTool ();
				const auto   one  = X3D::Vector3f (1, 1, 1) - X3D::Vector3f (max (axes .at (type), -axes .at (type)));

				gridSwitch -> whichChoice () = 0;
	
				grid -> setField <X3D::SFVec3f> ("translation", tool -> translation () * one);
				grid -> setField <X3D::SFVec3f> ("scale",       tool -> scale ());
				grid -> setField <X3D::MFInt32> ("dimension",   X3D::MFInt32 ({ tool -> dimension () .at (1), 0, tool -> dimension () .at (1) }));
	
				for (size_t i = 0; i < tool -> majorLineEvery () .size () / 4; ++ i)
				{
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i * 3 + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i * 3 + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineEvery") .set1Value (i * 3 + 2, tool -> majorLineEvery () .at (i * 4 + 3));
				}
	
				for (size_t i = 0; i < tool -> majorLineOffset () .size () / 4; ++ i)
				{
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i * 3 + 0, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i * 3 + 1, 0);
					grid -> getField <X3D::MFInt32> ("majorLineOffset") .set1Value (i * 3 + 2, tool -> majorLineOffset () .at (i * 4 + 3));
				}
	
				grid -> setField <X3D::SFColor> ("color",          X3D::Color3f (tool -> color ()          .getRed (), tool -> color ()          .getGreen (), tool -> color ()          .getBlue ()));
				grid -> setField <X3D::SFColor> ("lineColor",      X3D::Color3f (tool -> lineColor ()      .getRed (), tool -> lineColor ()      .getGreen (), tool -> lineColor ()      .getBlue ()));
				grid -> setField <X3D::SFColor> ("majorLineColor", X3D::Color3f (tool -> majorLineColor () .getRed (), tool -> majorLineColor () .getGreen (), tool -> majorLineColor () .getBlue ()));
	
				grid -> setField <X3D::SFFloat> ("transparency",          1 - tool -> color ()          .getAlpha ());
				grid -> setField <X3D::SFFloat> ("lineTransparency",      1 - tool -> lineColor ()      .getAlpha ());
				grid -> setField <X3D::SFFloat> ("majorLineTransparency", 1 - tool -> majorLineColor () .getAlpha ());
			}
		}
		else
		{
			gridSwitch -> whichChoice () = -1;
		}
	}
	catch (const X3D::Error <X3D::DISPOSED> & error)
	{ }
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;

		if (gridSwitch)
			gridSwitch -> whichChoice () = -1;
	}
}

void
X3DBrowserPanel::on_map ()
{
	if (type == BrowserPanelType::MAIN_VIEW)
		return;

	getPage () -> getMainBrowser () -> changed () .addInterest (&X3D::Browser::addEvent, browser .getValue ()); // addDependentBrowser

	getBrowserWindow () -> getGridTool ()            -> getTool () -> addInterest (&X3DBrowserPanel::set_grid, this);
	getBrowserWindow () -> getAngleGridTool ()       -> getTool () -> addInterest (&X3DBrowserPanel::set_grid, this);
	getBrowserWindow () -> getAxonometricGridTool () -> getTool () -> addInterest (&X3DBrowserPanel::set_grid, this);
}

void
X3DBrowserPanel::on_unmap ()
{
	if (type == BrowserPanelType::MAIN_VIEW)
		return;

	getPage () -> getMainBrowser () -> changed () .removeInterest (&X3D::Browser::addEvent, browser .getValue ()); // addDependentBrowser

	getBrowserWindow () -> getGridTool ()            -> getTool () -> removeInterest (&X3DBrowserPanel::set_grid, this);
	getBrowserWindow () -> getAngleGridTool ()       -> getTool () -> removeInterest (&X3DBrowserPanel::set_grid, this);
	getBrowserWindow () -> getAxonometricGridTool () -> getTool () -> removeInterest (&X3DBrowserPanel::set_grid, this);
}

void
X3DBrowserPanel::dispose ()
{
	const auto parent = browser -> get_parent ();

	if (parent)
		parent -> remove (*browser);

	X3DBrowserPanelInterface::dispose ();
}

X3DBrowserPanel::~X3DBrowserPanel ()
{ }

} // puck
} // titania
