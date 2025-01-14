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

#ifndef __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_COORDINATE_H__
#define __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_COORDINATE_H__

#include "../Geospatial/X3DGeospatialObject.h"
#include "../Rendering/X3DCoordinateNode.h"

namespace titania {
namespace X3D {

class GeoCoordinate :
	public X3DCoordinateNode, public X3DGeospatialObject
{
public:

	///  @name Construction

	GeoCoordinate (X3DExecutionContext* const executionContext);

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

	MFVec3d &
	point ()
	{ return *fields .point; }

	const MFVec3d &
	point () const
	{ return *fields .point; }

	///  @name Member access

	virtual
	Box3d
	getBBox () const final override;

	virtual
	void
	set1Point (const size_t index, const Vector3d & value) final override;

	virtual
	Vector3d
	get1Point (const size_t index) const final override;

	virtual
	Vector3f
	getNormal (const size_t index1, const size_t index2, const size_t index3) const final override;

	virtual
	Vector3f
	getNormal (const size_t index1, const size_t index2, const size_t index3, const size_t index4) const final override;

	virtual
	void
	addVertex (math::tessellator <double, size_t> & tessellator, const size_t index, const size_t i) const final override;

	virtual
	void
	addVertex (std::vector <Vector3d> & vertices, const size_t index) const final override;

	virtual
	bool
	isEmpty () const final override
	{ return point () .empty (); }

	virtual
	size_t
	getSize () const final override
	{ return point () .size (); }

	///  @name Operations

	virtual
	void
	assignPoints (const std::vector <Vector3d> points) final override;

	virtual
	void
	erasePoint (const size_t index) final override;

	virtual
	void
	resize (const size_t value) final override
	{ point () .resize (value); }

	///  @name Destruction
	
	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;
	
	void
	eventsProcessed ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFVec3d* const point;
	};

	Fields fields;

	std::vector <Vector3d> points;
	Vector3d               origin;

};

} // X3D
} // titania

#endif
