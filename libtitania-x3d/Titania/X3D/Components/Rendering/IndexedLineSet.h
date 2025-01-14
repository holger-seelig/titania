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

#ifndef __TITANIA_X3D_COMPONENTS_RENDERING_INDEXED_LINE_SET_H__
#define __TITANIA_X3D_COMPONENTS_RENDERING_INDEXED_LINE_SET_H__

#include "../Rendering/X3DLineGeometryNode.h"

namespace titania {
namespace X3D {

class IndexedLineSet :
	virtual public X3DLineGeometryNode
{
public:

	///  @name Construction

	IndexedLineSet (X3DExecutionContext* const executionContext);

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
	MFInt32 &
	set_colorIndex ()
	{ return *fields .set_colorIndex; }

	virtual
	const MFInt32 &
	set_colorIndex () const
	{ return *fields .set_colorIndex; }

	virtual
	MFInt32 &
	set_coordIndex ()
	{ return *fields .set_coordIndex; }

	virtual
	const MFInt32 &
	set_coordIndex () const
	{ return *fields .set_coordIndex; }

	virtual
	SFBool &
	colorPerVertex ()
	{ return *fields .colorPerVertex; }

	virtual
	const SFBool &
	colorPerVertex () const
	{ return *fields .colorPerVertex; }

	virtual
	MFInt32 &
	colorIndex ()
	{ return *fields .colorIndex; }

	virtual
	const MFInt32 &
	colorIndex () const
	{ return *fields .colorIndex; }

	virtual
	MFInt32 &
	coordIndex ()
	{ return *fields .coordIndex; }

	virtual
	const MFInt32 &
	coordIndex () const
	{ return *fields .coordIndex; }

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
	coord ()
	{ return *fields .coord; }

	virtual
	const SFNode &
	coord () const
	{ return *fields .coord; }

	///  @name Hidden fields

	virtual
	SFNode &
	options ()
	{ return *fields .options; }

	virtual
	const SFNode &
	options () const
	{ return *fields .options; }

	///  @name Member access

	virtual
	const X3DPtrArray <X3DVertexAttributeNode> &
	getAttrib () const
	{ return attribNodes; }

	virtual
	const X3DPtr <FogCoordinate> &
	getFogCoord () const
	{ return fogCoordNode; }

	virtual
	const X3DPtr <X3DColorNode> &
	getColor () const
	{ return colorNode; }

	virtual
	const X3DPtr <X3DCoordinateNode> &
	getCoord () const
	{ return coordNode; }

	///  @name Operations

	virtual
	std::vector <Vector3d>
	getPolylines () const;

	virtual
	NodeType
	getPrimitiveType () const final override
	{ return X3DConstants::PointSet; }

	virtual
	SFNode
	toPrimitive () const override;

	///  @name Destruction

	virtual
	~IndexedLineSet () override;


protected:

	///  @name Construction

	virtual
	void
	initialize () override;


private:

	virtual
	const X3DPtr <ComposedShader> &
	getShaderNode (X3DBrowser* const browser) final override;

	///  @name Event handler

	void
	set_attrib ();

	void
	set_fogCoord ();

	void
	set_color ();

	void
	set_transparent ();

	void
	set_coord ();

	void
	set_options ();

	///  @name Operations

	size_t
	getColorIndex (const size_t, const bool) const;

	size_t
	getColorIndex (const size_t) const;

	std::vector <std::vector <size_t>>
	getPolylineIndices () const;

	virtual
	void
	build () final override;

	virtual
	void
	shutdown () override;

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFInt32* const set_colorIndex;
		MFInt32* const set_coordIndex;
		SFBool* const colorPerVertex;
		MFInt32* const colorIndex;
		MFInt32* const coordIndex;
		MFNode* const attrib;
		SFNode* const fogCoord;
		SFNode* const color;
		SFNode* const coord;
		SFNode* const options;
	};

	Fields fields;

	X3DPtrArray <X3DVertexAttributeNode> attribNodes;
	X3DPtr <FogCoordinate>               fogCoordNode;
	X3DPtr <X3DColorNode>                colorNode;
	X3DPtr <X3DCoordinateNode>           coordNode;
	X3DPtr <LSystem>                     optionsNode;

};

} // X3D
} // titania

#endif
