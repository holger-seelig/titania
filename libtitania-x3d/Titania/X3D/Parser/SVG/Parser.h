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

#ifndef __TITANIA_X3D_PARSER_SVG_PARSER_H__
#define __TITANIA_X3D_PARSER_SVG_PARSER_H__

#include "../../Execution/X3DScene.h"
#include "../../Parser/X3DParser.h"
#include "../../Types/Geometry.h"

#include <Titania/Math/Mesh/Tessellator.h>

#include <cairomm/cairomm.h>

namespace xmlpp {

class Attribute;
class DomParser;
class Element;
class Node;

}

namespace titania {
namespace X3D {
namespace SVG {

class Parser :
	public X3D::X3DParser
{
public:

	///  @name Construction

	Parser (const X3D::X3DScenePtr &, const basic::uri &, std::istream &);

	///  @name Operations

	virtual
	void
	parseIntoScene () final override;

	///  @name Destruction

	virtual
	~Parser () final override;


private:

	using Tesselator = math::tessellator <double, size_t>;
	using Points     = std::vector <X3D::Vector2d>;

	struct Curve
	{
		Curve (Points && points, const bool closed) :
			points (std::move (points)),
			closed (closed)
		{ }

		Points points;
		bool   closed;
	};

	using CurvePtr = std::shared_ptr <Curve>;

	struct Contour
	{
		Contour (Points && points, const bool closed) :
			curve (std::make_shared <Curve> (std::move (points), closed)),
			index (0)
		{ }

		CurvePtr curve; // Curve
		size_t   index; // First coord index
	};

	using ContourPtr = std::shared_ptr <Contour>;
	using Contours   = std::vector <ContourPtr>;

	struct Gradient
	{
		Gradient () :
			               x1 (0),
			               y1 (0),
			               x2 (0),
			               y2 (0),
		                  cx (0),
		                  cy (0),
		                   r (0),
		                  fx (0),
		                  fy (0),
			    gradientUnits (),
			gradientTransform (),
		        spreadMethod (),
			            stops ()
			{ }

		double x1;
		double y1;
		double x2;
		double y2;

		double cx;
		double cy;
		double r;
		double fx;
		double fy;

		std::string                     gradientUnits;
		X3D::Matrix3d                   gradientTransform;
		std::string                     spreadMethod;
		std::map <double, X3D::Color4f> stops;
	};

	enum class ColorType
	{
		NONE,
		COLOR,
		URL,
	};

	struct Style {

		Style () :
			      display ("inline"),
			     fillType (ColorType::COLOR),
			    fillColor (),
			      fillURL (),
			  fillOpacity (1),
			     fillRule ("nonzero"),
			   strokeType (ColorType::NONE),
			  strokeColor (),
			    strokeURL (),
			strokeOpacity (1),
			  strokeWidth (1),
			      opacity (1),
			    stopColor (),
			  stopOpacity (1)
			{ }

		std::string  display;
		ColorType    fillType;
		X3D::Color3f fillColor;
		basic::uri   fillURL;
		double       fillOpacity;
		std::string  fillRule;
		ColorType    strokeType;
		X3D::Color3f strokeColor;
		basic::uri   strokeURL;
		double       strokeOpacity;
		double       strokeWidth;
		double       opacity;
		Color3f      stopColor;
		double       stopOpacity;

	};

	///  @name Operations

	X3DBrowser*
	getBrowser () const
	{ return scene -> getBrowser (); }

	void
	xmlElement (xmlpp::Element* const xmlElement);

	void
	svgElement (xmlpp::Element* const xmlElement);

	bool
	whiteSpaces (std::istream & istream);

	bool
	commaWhiteSpaces (std::istream & istream);

	void
	elements (xmlpp::Element* const xmlElement);

	void
	element (xmlpp::Element* const xmlElement);

	void
	useElement (xmlpp::Element* const xmlElement);

	bool
	isUsed (xmlpp::Element* const xmlElement);

	void
	groupElement (xmlpp::Element* const xmlElement);

	void
	switchElement (xmlpp::Element* const xmlElement);

	void
	aElement (xmlpp::Element* const xmlElement);

	void
	rectangleElement (xmlpp::Element* const xmlElement);

	void
	circleElement (xmlpp::Element* const xmlElement);

	void
	ellipseElement (xmlpp::Element* const xmlElement);

	void
	textElement (xmlpp::Element* const xmlElement);

	void
	imageElement (xmlpp::Element* const xmlElement);

	void
	polylineElement (xmlpp::Element* const xmlElement);

	void
	polygonElement (xmlpp::Element* const xmlElement);

	void
	pathElement (xmlpp::Element* const xmlElement);

	Parser::Tesselator::data_type
	combine (const X3D::X3DPtr <X3D::Coordinate> & coordinateNode, const Vector3d & point) const;

	bool
	paintURL (const basic::uri & url,
	          const X3D::Box2d & bbox,
	          const Cairo::RefPtr <Cairo::Context> & context);

	void
	paintLinearGradientElement (xmlpp::Element* const xmlElement,
	                            const X3D::Box2d & bbox,
	                            const Cairo::RefPtr <Cairo::Context> & context);

	void
	linearGradientElement (xmlpp::Element* const xmlElement, Gradient & gradient);

	void
	paintRadialGradientElement (xmlpp::Element* const xmlElement,
	                            const X3D::Box2d & bbox,
	                            const Cairo::RefPtr <Cairo::Context> & context);

	void
	radialGradientElement (xmlpp::Element* const xmlElement, Gradient & gradient);

	void
	gradientChild (xmlpp::Element* const xmlElement, Gradient & gradient);
	
	void
	stopElement (xmlpp::Element* const xmlElement, Gradient & gradient);

	void
	gradientElement (xmlpp::Element* const xmlElement, Gradient & gradient);

	void
	paintGradient (const Cairo::RefPtr <Cairo::Gradient> & cairoGradient,
	               const Gradient & gradient,
	               const X3D::Box2d & bbox,
	               const Cairo::RefPtr <Cairo::Context> & context);

	void
	idAttribute (xmlpp::Attribute* const attribute, const X3D::SFNode & node);

	bool
	viewBoxAttribute (xmlpp::Attribute* const xmlAttribute, X3D::Vector4d & value);

	bool
	transformAttribute (xmlpp::Attribute* const xmlAttribute, X3D::Matrix3d & matrix);

	bool
	lengthAttribute (xmlpp::Attribute* const xmlAttribute, double & value);

	bool
	percentAttribute (xmlpp::Attribute* const xmlAttribute, double & value);

	bool
	stringAttribute (xmlpp::Attribute* const xmlAttribute, std::string & value);

	bool
	urlAttribute (xmlpp::Attribute* const xmlAttribute, basic::uri & value);

	bool
	pointsAttribute (xmlpp::Attribute* const xmlAttribute, std::vector <X3D::Vector2d> & points);

	bool
	dAttribute (xmlpp::Attribute* const xmlAttribute, Contours & contours);

	void
	styleAttributes (xmlpp::Element* const xmlElement, Style & style);

	bool
	styleAttribute (xmlpp::Attribute* const xmlAttribute, Style & value);

	void
	displayStyle (const std::string & value, Style & style);
	
	void
	fillStyle (const std::string & value, Style & style);
	
	void
	fillOpacityStyle (const std::string & value, Style & style);

	void
	fillRuleStyle (const std::string & value, Style & style);
	
	void
	strokeStyle (const std::string & value, Style & style);
	
	void
	strokeOpacityStyle (const std::string & value, Style & style);
	
	void
	strokeWidthStyle (const std::string & value, Style & style);
	
	void
	opacityStyle (const std::string & value, Style & style);
	
	void
	stopColorStyle (const std::string & value, Style & style);
	
	void
	stopOpacityStyle (const std::string & value, Style & style);

	bool
	colorValue (std::istream & istream, X3D::Color3f & color);

	bool
	urlValue (std::istream & istream, basic::uri & url);

	X3D::X3DPtr <X3D::Transform>
	getTransform (xmlpp::Element* const xmlElement,
	              const X3D::Vector2d & translation = X3D::Vector2d (),
	              const X3D::Vector2d & scale = X3D::Vector2d (1, 1));

	X3D::X3DPtr <X3D::Appearance>
	getFillAppearance (const Style & fillStyle, X3D::Box2d bbox);

	X3D::X3DPtr <X3D::Appearance>
	getStrokeAppearance (const Style & strokeStyle);

	GLenum
	getFillRule (const Style & style);

	///  @name Static members
	using ElementsFunction      = std::function <void (Parser*, xmlpp::Element* const)>;
	using PaintGradientFunction = std::function <void (Parser*, xmlpp::Element* const, const X3D::Box2d &, const Cairo::RefPtr <Cairo::Context> &)>;
	using GradientFunction      = std::function <void (Parser*, xmlpp::Element* const, Gradient &)>;
	using StyleFunction         = std::function <void (Parser*, const std::string &, Style &)>;

	static const std::map <std::string, ElementsFunction>      xmlElementsIndex;
	static const std::map <std::string, ElementsFunction>      elementsIndex;
	static const std::map <std::string, PaintGradientFunction> paintGradientIndex;
	static const std::map <std::string, GradientFunction>      gradientIndex;
	static const std::map <std::string, StyleFunction>         styleIndex;

	///  @name Members

	const X3D::X3DScenePtr scene;
	const basic::uri       uri;
	std::istream &         istream;

	const std::unique_ptr <xmlpp::DomParser> xmlParser;

	std::vector <Style>                     styles;
	X3D::X3DPtr <X3D::LayerSet>             layerSet;
	X3D::X3DPtr <X3D::Transform>            rootTransform;
	X3D::X3DPtrArray <X3D::X3DGroupingNode> groupNodes;
	X3D::X3DPtr <X3D::OrthoViewpoint>       viewpoint;
	X3D::X3DPtr <X3D::TextureProperties>    texturePropertiesNode;
	Matrix4d                                viewMatrix;

	std::string whiteSpaceCharacters;

};

} // SVG
} // X3D
} // titania

#endif
