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

#include "X3DParametricGeometryNode.h"

#include "../Geometry3D/IndexedFaceSet.h"
#include "../NURBS/CoordinateDouble.h"
#include "../Texturing/TextureCoordinate.h"
#include "../Texturing3D/TextureCoordinate3D.h"
#include "../Texturing3D/TextureCoordinate4D.h"

#include "../../Browser/NURBS/NURBS.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

X3DParametricGeometryNode::X3DParametricGeometryNode () :
	X3DGeometryNode ()
{
	addType (X3DConstants::X3DParametricGeometryNode);
}

std::vector <float>
X3DParametricGeometryNode::getKnots (const bool closed,
                                     const size_t order,
                                     const size_t dimension,
                                     const std::vector <double> & knot) const
{
	return NURBS::getKnots (closed, order, dimension, knot);
}

NodeType
X3DParametricGeometryNode::getPrimitiveType () const
{
	return X3DConstants::IndexedFaceSet;
}

SFNode
X3DParametricGeometryNode::toPrimitive () const
{
	const auto geometryNode   = getExecutionContext () -> createNode <IndexedFaceSet> ();
	const auto coordinateNode = getExecutionContext () -> createNode <CoordinateDouble> ();
	auto       texCoordNode   = X3DPtr <X3DTextureCoordinateNode> ();
	auto       texCoordMap    = std::map <Vector4d, size_t> ();
	auto       coordMap       = std::map <Vector3d, size_t> ();
	bool       tex3D          = false;
	bool       tex4D          = false;

	for (const auto & texCoord : getTexCoords ())
	{
		if (std::abs (texCoord .w () - 1) > 1e-5)
			tex4D = true;

		if (std::abs (texCoord .z ()) > 1e-5)
			tex3D = true;
	}

	if (tex4D)
		texCoordNode = getExecutionContext () -> createNode <TextureCoordinate4D> ();
	else if (tex3D)
		texCoordNode = getExecutionContext () -> createNode <TextureCoordinate3D> ();
	else
		texCoordNode = getExecutionContext () -> createNode <TextureCoordinate> ();

	geometryNode -> creaseAngle () = pi <float>;
	geometryNode -> solid ()       = getSolid ();
	geometryNode -> ccw ()         = getCCW ();
	geometryNode -> texCoord ()    = texCoordNode;
	geometryNode -> coord ()       = coordinateNode;

	for (const auto & texCoord : getTexCoords ())
		texCoordMap .emplace (texCoord, texCoordMap .size ());

	for (const auto & vertex : getVertices ())
		coordMap .emplace (vertex, coordMap .size ());

	for (const auto & element : getElements ())
	{
		switch (element .vertexMode ())
		{
			case GL_TRIANGLES:
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 3)
				{
					const auto & texCoord1 = getTexCoords () [i];
					const auto & texCoord2 = getTexCoords () [i + 1];
					const auto & texCoord3 = getTexCoords () [i + 2];

					const auto & vertex1 = getVertices () [i];
					const auto & vertex2 = getVertices () [i + 1];
					const auto & vertex3 = getVertices () [i + 2];

					geometryNode -> texCoordIndex () .emplace_back (texCoordMap [texCoord1]);
					geometryNode -> texCoordIndex () .emplace_back (texCoordMap [texCoord2]);
					geometryNode -> texCoordIndex () .emplace_back (texCoordMap [texCoord3]);
					geometryNode -> texCoordIndex () .emplace_back (-1);

					geometryNode -> coordIndex () .emplace_back (coordMap [vertex1]);
					geometryNode -> coordIndex () .emplace_back (coordMap [vertex2]);
					geometryNode -> coordIndex () .emplace_back (coordMap [vertex3]);
					geometryNode -> coordIndex () .emplace_back (-1);
				}

				break;
			}
			case GL_QUADS:
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 4)
				{
					const auto & texCoord1 = getTexCoords () [i];
					const auto & texCoord2 = getTexCoords () [i + 1];
					const auto & texCoord3 = getTexCoords () [i + 2];
					const auto & texCoord4 = getTexCoords () [i + 3];

					const auto & vertex1 = getVertices () [i];
					const auto & vertex2 = getVertices () [i + 1];
					const auto & vertex3 = getVertices () [i + 2];
					const auto & vertex4 = getVertices () [i + 3];

					geometryNode -> texCoordIndex () .emplace_back (texCoordMap [texCoord1]);
					geometryNode -> texCoordIndex () .emplace_back (texCoordMap [texCoord2]);
					geometryNode -> texCoordIndex () .emplace_back (texCoordMap [texCoord3]);
					geometryNode -> texCoordIndex () .emplace_back (texCoordMap [texCoord4]);
					geometryNode -> texCoordIndex () .emplace_back (-1);

					geometryNode -> coordIndex () .emplace_back (coordMap [vertex1]);
					geometryNode -> coordIndex () .emplace_back (coordMap [vertex2]);
					geometryNode -> coordIndex () .emplace_back (coordMap [vertex3]);
					geometryNode -> coordIndex () .emplace_back (coordMap [vertex4]);
					geometryNode -> coordIndex () .emplace_back (-1);
				}

				break;
			}
		}
	}

	for (const auto & [texCoord, index] : texCoordMap)
		texCoordNode -> set1Point (index, texCoord);

	for (const auto & [point, index] : coordMap)
		coordinateNode -> set1Point (index, point);

	return geometryNode;
}

} // X3D
} // titania
