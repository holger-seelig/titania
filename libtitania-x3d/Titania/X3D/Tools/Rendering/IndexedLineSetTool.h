/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_TOOLS_RENDERING_INDEXED_LINE_SET_TOOL_H__
#define __TITANIA_X3D_TOOLS_RENDERING_INDEXED_LINE_SET_TOOL_H__

#include "../../Components/Rendering/IndexedLineSet.h"
#include "../Rendering/X3DLineGeometryNodeTool.h"

namespace titania {
namespace X3D {

class IndexedLineSetTool :
	virtual public IndexedLineSet,
	public X3DLineGeometryNodeTool
{
public:

	///  @name Construction

	IndexedLineSetTool (X3DBaseNode* const node);

	///  @name Fields

	virtual
	MFInt32 &
	set_colorIndex () final override
	{ return getNode <IndexedLineSet> () -> set_colorIndex (); }

	virtual
	const MFInt32 &
	set_colorIndex () const final override
	{ return getNode <IndexedLineSet> () -> set_colorIndex (); }

	virtual
	MFInt32 &
	set_coordIndex () final override
	{ return getNode <IndexedLineSet> () -> set_coordIndex (); }

	virtual
	const MFInt32 &
	set_coordIndex () const final override
	{ return getNode <IndexedLineSet> () -> set_coordIndex (); }

	virtual
	SFBool &
	colorPerVertex () final override
	{ return getNode <IndexedLineSet> () -> colorPerVertex (); }

	virtual
	const SFBool &
	colorPerVertex () const final override
	{ return getNode <IndexedLineSet> () -> colorPerVertex (); }

	virtual
	MFInt32 &
	colorIndex () final override
	{ return getNode <IndexedLineSet> () -> colorIndex (); }

	virtual
	const MFInt32 &
	colorIndex () const final override
	{ return getNode <IndexedLineSet> () -> colorIndex (); }

	virtual
	MFInt32 &
	coordIndex () final override
	{ return getNode <IndexedLineSet> () -> coordIndex (); }

	virtual
	const MFInt32 &
	coordIndex () const final override
	{ return getNode <IndexedLineSet> () -> coordIndex (); }

	virtual
	MFNode &
	attrib () final override
	{ return getNode <IndexedLineSet> () -> attrib (); }

	virtual
	const MFNode &
	attrib () const final override
	{ return getNode <IndexedLineSet> () -> attrib (); }

	virtual
	SFNode &
	fogCoord () final override
	{ return getNode <IndexedLineSet> () -> fogCoord (); }

	virtual
	const SFNode &
	fogCoord () const final override
	{ return getNode <IndexedLineSet> () -> fogCoord (); }

	virtual
	SFNode &
	color () final override
	{ return getNode <IndexedLineSet> () -> color (); }

	virtual
	const SFNode &
	color () const final override
	{ return getNode <IndexedLineSet> () -> color (); }

	virtual
	SFNode &
	coord () final override
	{ return getNode <IndexedLineSet> () -> coord (); }

	virtual
	const SFNode &
	coord () const final override
	{ return getNode <IndexedLineSet> () -> coord (); }

	///  @name Hidden fields

	virtual
	SFNode &
	options () final override
	{ return getNode <IndexedLineSet> () -> options (); }

	virtual
	const SFNode &
	options () const final override
	{ return getNode <IndexedLineSet> () -> options (); }

	///  @name Test

	virtual
	const X3DPtrArray <X3DVertexAttributeNode> &
	getAttrib () const final override
	{ return getNode <IndexedLineSet> () -> getAttrib (); }

	virtual
	const X3DPtr <FogCoordinate> &
	getFogCoord () const final override
	{ return getNode <IndexedLineSet> () -> getFogCoord (); }

	virtual
	const X3DPtr <X3DColorNode> &
	getColor () const final override
	{ return getNode <IndexedLineSet> () -> getColor (); }

	virtual
	const X3DPtr <X3DCoordinateNode> &
	getCoord () const final override
	{ return getNode <IndexedLineSet> () -> getCoord (); }

	///  @name Operations

	virtual
	std::vector <Vector3d>
	getPolylines () const final override
	{ return getNode <IndexedLineSet> () -> getPolylines (); }

	virtual
	SFNode
	toPrimitive () const final override
	{ return getNode <IndexedLineSet> () -> toPrimitive (); }

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~IndexedLineSetTool () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Members

	struct Fields
	{
		Fields ();

	};

	Fields fields;

};

} // X3D
} // titania

#endif
