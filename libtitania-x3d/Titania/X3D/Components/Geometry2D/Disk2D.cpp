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

#include "Disk2D.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Geometry2D/Disk2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"
#include "../Geometry3D/IndexedFaceSet.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/IndexedLineSet.h"
#include "../Rendering/PointSet.h"
#include "../Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

const Component   Disk2D::component      = Component ("Geometry2D", 2);
const std::string Disk2D::typeName       = "Disk2D";
const std::string Disk2D::containerField = "geometry";

Disk2D::Fields::Fields () :
	innerRadius (new SFFloat ()),
	outerRadius (new SFFloat (1)),
	      solid (new SFBool ()),
	    options (new SFNode ())
{ }

Disk2D::Disk2D (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DGeometryNode (),
	X3DLineGeometryNode (),
	             fields (),
	        optionsNode ()
{
	addType (X3DConstants::Disk2D);

	addField (inputOutput,    "metadata",    metadata ());
	addField (initializeOnly, "innerRadius", innerRadius ());
	addField (initializeOnly, "outerRadius", outerRadius ());
	addField (initializeOnly, "solid",       solid ());
	addField (initializeOnly, "options",     options ());

	addChildObjects (optionsNode);

	innerRadius () .setUnit (UnitCategory::LENGTH);
	outerRadius () .setUnit (UnitCategory::LENGTH);

	options () .isHidden (true);
}

X3DBaseNode*
Disk2D::create (X3DExecutionContext* const executionContext) const
{
	return new Disk2D (executionContext);
}

void
Disk2D::initialize ()
{
	X3DGeometryNode::initialize ();

	options () .addInterest (&Disk2D::set_options, this);

	try
	{
		const auto metaOptions = getMetadataSet ("/Disk2D/options");

		optionsNode .set (MakePtr <Disk2DOptions> (getExecutionContext ()));

		optionsNode -> addInterest (&Disk2D::requestRebuild, this);
		optionsNode -> fromMetaData (metaOptions);
		optionsNode -> setup ();

		options () .set (optionsNode);
	}
	catch (const X3D::X3DError & error)
	{
		set_options ();
	}
}

void
Disk2D::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
		getBrowser () -> getDisk2DOptions () .removeInterest (&Disk2D::requestRebuild, this);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_options ();
}

const X3DPtr <ComposedShader> &
Disk2D::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

Box3d
Disk2D::createBBox () const
{
	const auto diameter = std::abs (std::max (innerRadius (), outerRadius ())) * 2;

	return Box3d (Vector3d (diameter, diameter, 0), Vector3d ());
}

void
Disk2D::set_options ()
{
	removeMetaData ("/Disk2D/options");

	if (optionsNode)
		optionsNode -> removeInterest (&Disk2D::requestRebuild, this);

	optionsNode .set (x3d_cast <Disk2DOptions*> (options ()));

	if (not optionsNode)
		optionsNode .set (getBrowser () -> getDisk2DOptions ());

	optionsNode -> addInterest (&Disk2D::requestRebuild, this);
}

void
Disk2D::build ()
{
	if (options ())
		optionsNode -> toMetaData (createMetadataSet ("/Disk2D/options"));

	const double minRadius = std::min (std::abs (innerRadius ()), std::abs (outerRadius ()));
	const double maxRadius = std::max (std::abs (innerRadius ()), std::abs (outerRadius ()));

	if (minRadius == maxRadius)
	{
		const double radius = minRadius;

		// Point

		if (radius == 0)
		{
			getVertices () .emplace_back (0, 0, 0);
			addElements (GL_POINTS, getVertices () .size ());
			setGeometryType (0);
			setSolid (false);
			return;
		}

		// Circle

		if (radius == 1)
		{
			getVertices () = optionsNode -> getVertices ();
		}
		else
		{
			for (const auto & vertex : optionsNode -> getVertices ())
				getVertices () .emplace_back (vertex * radius);
		}

		addElements (GL_LINE_LOOP, getVertices () .size ());
		setGeometryType (1);
		setSolid (false);
		return;
	}

	if (minRadius == 0)
	{
		// Disk

		const double radius = maxRadius;

		getMultiTexCoords () .emplace_back ();

		const auto & texCoords = optionsNode -> getTexCoords ();
		const auto & vertices  = optionsNode -> getVertices ();

		const auto t0 = Vector4f (0.5, 0.5, 0, 1);
		const auto p0 = Vector3d (0, 0, 0);

		for (size_t i = 0, size = vertices .size () - 1; i < size; ++ i)
		{
			const auto & t1 = texCoords [i];
			const auto & p1 = vertices [i];
			const auto & t2 = texCoords [i + 1];
			const auto & p2 = vertices [i + 1];

			getTexCoords () .emplace_back (t0);
			getNormals ()   .emplace_back (0, 0, 1);
			getVertices ()  .emplace_back (p0);

			getTexCoords () .emplace_back (t1);
			getNormals ()   .emplace_back (0, 0, 1);
			getVertices ()  .emplace_back (p1 * radius);

			getTexCoords () .emplace_back (t2);
			getNormals ()   .emplace_back (0, 0, 1);
			getVertices ()  .emplace_back (p2 * radius);
		}

		addElements (GL_TRIANGLES, getVertices () .size ());
		setGeometryType (2);
		setSolid (solid ());
		return;
	}

	// Disk with hole

	// Texture Coordinates

	getMultiTexCoords () .emplace_back ();

	const auto & texCoords = optionsNode -> getTexCoords ();
	const auto & vertices  = optionsNode -> getVertices ();
	const double scale     = minRadius / maxRadius;

	for (size_t i = 0, size = optionsNode -> getVertices () .size (); i < size; ++ i)
	{
		const auto i1 = (i + 1) % size;
		const auto t0 = Vector4f (texCoords [i] .x () * scale + (1 - scale) / 2, texCoords [i] .y () * scale + (1 - scale) / 2, 0, 1);
		const auto t1 = Vector4f (texCoords [i]);
		const auto t2 = Vector4f (texCoords [i1]);
		const auto t3 = Vector4f (texCoords [i1] .x () * scale + (1 - scale) / 2, texCoords [i1] .y () * scale + (1 - scale) / 2, 0, 1);
		const auto p0 = Vector3d (vertices [i]  * minRadius);
		const auto p1 = Vector3d (vertices [i]  * maxRadius);
		const auto p2 = Vector3d (vertices [i1] * maxRadius);
		const auto p3 = Vector3d (vertices [i1] * minRadius);

		// Triangle 1

		// p0
		getTexCoords () .emplace_back (t0);
		getNormals ()   .emplace_back (0, 0, 1);
		getVertices ()  .emplace_back (p0);

		// p1
		getTexCoords () .emplace_back (t1);
		getNormals ()   .emplace_back (0, 0, 1);
		getVertices ()  .emplace_back (p1);

		// p2
		getTexCoords () .emplace_back (t2);
		getNormals ()   .emplace_back (0, 0, 1);
		getVertices ()  .emplace_back (p2);

		// Triangle 2

		// p0
		getTexCoords () .emplace_back (t0);
		getNormals ()   .emplace_back (0, 0, 1);
		getVertices ()  .emplace_back (p0);

		// p2
		getTexCoords () .emplace_back (t2);
		getNormals ()   .emplace_back (0, 0, 1);
		getVertices ()  .emplace_back (p2);

		// p3
		getTexCoords () .emplace_back (t3);
		getNormals ()   .emplace_back (0, 0, 1);
		getVertices ()  .emplace_back (p3);
	}

	addElements (GL_TRIANGLES, getVertices () .size ());
	setGeometryType (2);
	setSolid (solid ());
}

bool
Disk2D::intersects (Line3d line,
                    const CollectableObjectArray & clipPlanes,
                    Matrix4d modelViewMatrix,
                    std::vector <IntersectionPtr> & intersections) const
{
	if (innerRadius () == outerRadius ())
		return X3DLineGeometryNode::intersects (line, clipPlanes, modelViewMatrix, intersections);

	return X3DGeometryNode::intersects (line, clipPlanes, modelViewMatrix, intersections);
}

bool
Disk2D::intersects (Box3d box,
                    const CollectableObjectArray & clipPlanes,
                    Matrix4d modelViewMatrix) const
{
	if (innerRadius () == outerRadius ())
		return X3DLineGeometryNode::intersects (box, clipPlanes, modelViewMatrix);

	return X3DGeometryNode::intersects (box, clipPlanes, modelViewMatrix);
}

std::vector <Vector3d>
Disk2D::intersects (X3DRenderObject* const renderObject,
                    const std::shared_ptr <FrameBuffer> & frameBuffer,
                    const std::shared_ptr <FrameBuffer> & depthBuffer,
                    std::vector <IntersectionPtr> & intersections)
{
	if (innerRadius () == outerRadius ())
		return X3DLineGeometryNode::intersects (renderObject, frameBuffer, depthBuffer, intersections);

	return X3DGeometryNode::intersects (renderObject, frameBuffer, depthBuffer, intersections);
}

void
Disk2D::depth (const X3DShapeContainer* const context)
{ }

void
Disk2D::draw (ShapeContainer* const context)
{
	if (innerRadius () == outerRadius ())
		X3DLineGeometryNode::draw (context);
	else
		X3DGeometryNode::draw (context);
}

NodeType
Disk2D::getPrimitiveType () const
{
	if (getElements () [0] .vertexMode () == GL_POINTS)
		return X3DConstants::PointSet;

	if (getElements () [0] .vertexMode () == GL_LINE_LOOP)
		return X3DConstants::IndexedLineSet;

	return X3DConstants::IndexedFaceSet;
}

SFNode
Disk2D::toPrimitive () const
{
	if (innerRadius () == outerRadius ())
	{
		const float radius = std::abs (outerRadius ());

		// Point

		if (radius == 0)
		{
			// Point

			const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
			const auto geometry = getExecutionContext () -> createNode <PointSet> ();

			geometry -> coord () = coord;

			coord -> point () .emplace_back ();

			return SFNode (geometry);
		}

		// Circle

		const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
		const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

		geometry -> coord () = coord;

		coord -> point () .assign (optionsNode -> getVertices () .cbegin (), optionsNode -> getVertices () .cend ());

		if (radius not_eq 1)
		{
			for (MFVec3f::reference vertex : coord -> point ())
				vertex = vertex .get () * radius;
		}

		for (int32_t i = 0, size = optionsNode -> getVertices () .size (); i < size; ++ i)
			geometry -> coordIndex () .emplace_back (i);

		geometry -> coordIndex () .emplace_back (0);
		geometry -> coordIndex () .emplace_back (-1);

		return SFNode (geometry);
	}

	const auto texCoord = getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> solid ()    = solid ();
	geometry -> texCoord () = texCoord;
	geometry -> coord ()    = coord;

	if (innerRadius () == 0.0f or outerRadius () == 0.0f)
	{
		// Disk

		const float radius = std::abs (std::max (innerRadius (), outerRadius ()));

		coord -> point () .assign (optionsNode -> getVertices () .cbegin (), optionsNode -> getVertices () .cend ());

		if (radius not_eq 1)
		{
			for (MFVec3f::reference vertex : coord -> point ())
				vertex = vertex .get () * radius;
		}

		for (auto & point : optionsNode -> getTexCoords ())
			texCoord -> point () .emplace_back (point .x (), point .y ());

		for (int32_t i = 0, size = optionsNode -> getVertices () .size (); i < size; ++ i)
		{
			geometry -> texCoordIndex () .emplace_back (i);
			geometry -> coordIndex ()    .emplace_back (i);
		}

		geometry -> texCoordIndex () .emplace_back (-1);
		geometry -> coordIndex ()    .emplace_back (-1);

		return SFNode (geometry);
	}

	// Disk with hole

	// Texture Coordinates

	const double maxRadius = std::abs (std::max (innerRadius (), outerRadius ()));
	const double minRadius = std::abs (std::min (innerRadius (), outerRadius ()));
	const double scale     = minRadius / maxRadius;

	coord -> point () .assign (optionsNode -> getVertices () .cbegin (), optionsNode -> getVertices () .cbegin ());

	for (auto & point : optionsNode -> getVertices ())
		coord -> point () .emplace_back (point * minRadius);

	for (auto & point : optionsNode -> getVertices ())
		coord -> point () .emplace_back (point * maxRadius);

	for (auto & point : optionsNode -> getTexCoords ())
		texCoord -> point () .emplace_back (point .x () * scale + (1 - scale) / 2, point .y () * scale + (1 - scale) / 2);

	for (auto & point : optionsNode -> getTexCoords ())
		texCoord -> point () .emplace_back (point .x (), point .y ());

	for (size_t i = 0, size = optionsNode -> getVertices () .size (); i < size; ++ i)
	{
		const auto i1 = (i + 1) % size;

		// TexCoords

		geometry -> texCoordIndex () .emplace_back (i);
		geometry -> texCoordIndex () .emplace_back (i + size);
		geometry -> texCoordIndex () .emplace_back (i1 + size);
		geometry -> texCoordIndex () .emplace_back (i1);
		geometry -> texCoordIndex () .emplace_back (-1);

		// Vertices

		geometry -> coordIndex () .emplace_back (i);
		geometry -> coordIndex () .emplace_back (i + size);
		geometry -> coordIndex () .emplace_back (i1 + size);
		geometry -> coordIndex () .emplace_back (i1);
		geometry -> coordIndex () .emplace_back (-1);
	}

	return geometry;
}

Disk2D::~Disk2D ()
{ }

} // X3D
} // titania
