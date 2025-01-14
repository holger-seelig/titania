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

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_ELEVATION_GRID_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_ELEVATION_GRID_H__

#include "../Rendering/X3DGeometryNode.h"
#include "../../Thread/TextureFuture.h"

namespace titania {
namespace X3D {

class Texture;

class ElevationGrid :
	virtual public X3DGeometryNode
{
public:

	///  @name Construction

	ElevationGrid (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	virtual
	MFFloat &
	set_height ()
	{ return *fields .set_height; }

	virtual
	const MFFloat &
	set_height () const
	{ return *fields .set_height; }

	virtual
	SFInt32 &
	xDimension ()
	{ return *fields .xDimension; }

	virtual
	const SFInt32 &
	xDimension () const
	{ return *fields .xDimension; }

	virtual
	SFInt32 &
	zDimension ()
	{ return *fields .zDimension; }

	virtual
	const SFInt32 &
	zDimension () const
	{ return *fields .zDimension; }

	virtual
	SFFloat &
	xSpacing ()
	{ return *fields .xSpacing; }

	virtual
	const SFFloat &
	xSpacing () const
	{ return *fields .xSpacing; }

	virtual
	SFFloat &
	zSpacing ()
	{ return *fields .zSpacing; }

	virtual
	const SFFloat &
	zSpacing () const
	{ return *fields .zSpacing; }

	virtual
	SFFloat &
	creaseAngle ()
	{ return *fields .creaseAngle; }

	virtual
	const SFFloat &
	creaseAngle () const
	{ return *fields .creaseAngle; }

	virtual
	SFBool &
	solid ()
	{ return *fields .solid; }

	virtual
	const SFBool &
	solid () const
	{ return *fields .solid; }

	virtual
	SFBool &
	ccw ()
	{ return *fields .ccw; }

	virtual
	const SFBool &
	ccw () const
	{ return *fields .ccw; }

	virtual
	SFBool &
	colorPerVertex ()
	{ return *fields .colorPerVertex; }

	virtual
	const SFBool &
	colorPerVertex () const
	{ return *fields .colorPerVertex; }

	virtual
	SFBool &
	normalPerVertex ()
	{ return *fields .normalPerVertex; }

	virtual
	const SFBool &
	normalPerVertex () const
	{ return *fields .normalPerVertex; }

	virtual
	MFNode &
	attrib ()
	{ return *fields .attrib; }

	virtual
	const MFNode &
	attrib () const
	{ return *fields .attrib; }

	virtual
	SFNode &
	fogCoord ()
	{ return *fields .fogCoord; }

	virtual
	const SFNode &
	fogCoord () const
	{ return *fields .fogCoord; }

	virtual
	SFNode &
	color ()
	{ return *fields .color; }

	virtual
	const SFNode &
	color () const
	{ return *fields .color; }

	virtual
	SFNode &
	texCoord ()
	{ return *fields .texCoord; }

	virtual
	const SFNode &
	texCoord () const
	{ return *fields .texCoord; }

	virtual
	SFNode &
	normal ()
	{ return *fields .normal; }

	virtual
	const SFNode &
	normal () const
	{ return *fields .normal; }

	virtual
	MFFloat &
	height ()
	{ return *fields .height; }

	virtual
	const MFFloat &
	height () const
	{ return *fields .height; }

	///  @name Height map handling

	virtual
	std::pair <float, float>
	getMinMaxHeight () const;

	virtual
	void
	loadHeightMap (const MFString & url, const float minHeight, const float maxHeight);

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const
	{ return loadState; }

	///  @name Operations

	virtual
	void
	addNormals ();

	virtual
	NodeType
	getPrimitiveType () const final override
	{ return X3DConstants::IndexedFaceSet; }

	virtual
	SFNode
	toPrimitive () const override;

	///  @name Destruction

	virtual
	~ElevationGrid () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_attrib ();

	void
	set_fogCoord ();

	void
	set_color ();

	void
	set_transparent ();

	void
	set_texCoord ();

	void
	set_normal ();

	///  @name Operations

	virtual
	Box3d
	createBBox () const final override;

	double
	getHeight (const size_t) const;

	std::vector <Vector4f>
	createTexCoord () const;

	std::vector <Vector3f>
	createNormals (const std::vector <Vector3d> & points, const std::vector <size_t> & coordIndex, const double creaseAngle) const;

	std::vector <size_t>
	createCoordIndex () const;

	std::vector <Vector3d>
	createPoints () const;

	virtual
	void
	build () final override;

	void
	setHeightMapTexture (const basic::uri & url, const TexturePtr & texture, const float minHeight, const float maxHeight);

	void
	setHeightMapImage (const Glib::RefPtr <Gdk::Pixbuf> & image, const float minHeight, const float maxHeight);

	void
	setLoadState (const LoadState value)
	{ loadState = value; }

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		MFFloat* const set_height;
		SFInt32* const xDimension;
		SFInt32* const zDimension;
		SFFloat* const xSpacing;
		SFFloat* const zSpacing;
		SFBool* const solid;
		SFBool* const ccw;
		SFFloat* const creaseAngle;
		SFBool* const colorPerVertex;
		SFBool* const normalPerVertex;
		MFNode* const attrib;
		SFNode* const fogCoord;
		SFNode* const color;
		SFNode* const texCoord;
		SFNode* const normal;
		MFFloat* const height;
	};

	Fields fields;

	///  @name Members

	X3DPtrArray <X3DVertexAttributeNode> attribNodes;
	X3DPtr <FogCoordinate>               fogCoordNode;
	X3DPtr <X3DColorNode>                colorNode;
	X3DPtr <X3DTextureCoordinateNode>    texCoordNode;
	X3DPtr <X3DNormalNode>               normalNode;
	X3DPtr <TextureFuture>               future;
	SFEnum <LoadState>                   loadState;

};

} // X3D
} // titania

#endif
