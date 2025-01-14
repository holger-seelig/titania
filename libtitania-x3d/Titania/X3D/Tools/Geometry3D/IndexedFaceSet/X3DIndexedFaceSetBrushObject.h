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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_INDEXED_FACE_SET_X3DINDEXED_FACE_SET_BRUSH_OBJECT_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_INDEXED_FACE_SET_X3DINDEXED_FACE_SET_BRUSH_OBJECT_H__

#include "X3DIndexedFaceSetSelectionObject.h"

namespace titania {
namespace X3D {

class X3DIndexedFaceSetBrushObject :
	virtual public X3DIndexedFaceSetSelectionObject
{
public:

	///  @name Hidden fields

	SFNode &
	brush ()
	{ return *fields .brush; }

	const SFNode &
	brush () const
	{ return *fields .brush; }

	///  @name Member access

	const SFString &
	type () const;
	
	const SFDouble &
	radius () const;
	
	const SFDouble &
	height () const;
	
	const SFDouble &
	warp () const;
	
	const SFDouble &
	hardness () const;
	
	const SFDouble &
	sharpness () const;
	
	const SFDouble &
	pressure () const;
	
	const SFDouble &
	scale () const;
	
	const SFDouble &
	spacing () const;

	///  @name Destruction

	virtual
	void
	dispose ()
	{ }

	virtual
	~X3DIndexedFaceSetBrushObject () override;


protected:

	///  @name Construction

	X3DIndexedFaceSetBrushObject ();

	virtual
	void
	initialize () override;

	///  @name Member access

	virtual
	int32_t
	getToolNumber () const = 0;

	double
	getHeight (const Vector2d & v) const;


private:

	///  @name Event handlers

	void
	set_loadState ();

	void
	set_toolType ();

	void
	set_brush ();
	
	void
	set_brush_radius ();

	void
	set_touch_sensor_over ();

	void
	set_touch_sensor_hitPoint ();

	///  @name Member access
	
	double
	getCircularHeight (const Vector2d & v, const double w, const double s, const double e) const;
	
	double
	getSquaredHeight (const Vector2d & v, const double w, const double s, const double e) const;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFNode* const brush;
	};

	Fields fields;

	///  @name Members

	X3DPtr <Switch>      toolSwitch;
	X3DPtr <TouchSensor> touchSensor;
	X3DPtr <Switch>      brushSwitch;
	X3DPtr <Transform>   brushTransform;
	SFNode               brushNode;

};

} // X3D
} // titania

#endif
