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

#ifndef __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_KNIFE_OBJECT_H__
#define __TITANIA_X3D_TOOLS_GEOMETRY3D_X3DINDEXED_FACE_SET_KNIFE_OBJECT_H__

#include "X3DIndexedFaceSetCutObject.h"

namespace titania {
namespace X3D {

class Switch;
class Transform;
class TouchSensor;
class PlaneSensor;
class CoordinateDouble;
class Arc2D;

class X3DIndexedFaceSetKnifeObject :
	virtual public X3DIndexedFaceSetCutObject
{
public:

	///  @name Destruction

	virtual
	void
	dispose ()
	{ }

	virtual
	~X3DIndexedFaceSetKnifeObject () override;


protected:

	///  @name Construction

	X3DIndexedFaceSetKnifeObject ();

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_loadState ();

	void
	set_toolType ();

	void
	set_touch_sensor_hitPoint ();

	void
	set_touch_sensor_over ();

	void
	set_touch_sensor_active ();

	void
	set_plane_sensor (const X3DPtr <PlaneSensor> &, size_t);

	void
	set_plane_sensor_translation (PlaneSensor* const);

	bool
	setStartMagicSelection ();

	bool
	setEndMagicSelection (PlaneSensor* const planeSensor);

	bool
	snapToCenter (const std::pair <size_t, size_t> & edge,
	              Vector3d & point,
	              const Matrix4d & modelViewMatrix,
	              const Matrix4d & projectionMatrix,
	              const Vector4i & viewport);

	bool
	snapToVertex (const size_t face,
	              std::vector <int32_t> & points,
	              Vector3d & vertex,
	              const Matrix4d & modelViewMatrix,
	              const Matrix4d & projectionMatrix,
	              const Vector4i & viewport);

	Vector3d
	getClosestPoint (const std::pair <size_t, size_t> & edge,
	                 const std::pair <Vector3d, Vector3d> & cutPoints,
	                 const Matrix4d & modelViewMatrix,
	                 const Matrix4d & projectionMatrix,
	                 const Vector4i & viewport) const;

	void
	cut ();

	///  @name Members

	X3DPtr <Switch>           toolSwitch;
	X3DPtrArray <PlaneSensor> planeSensors;
	X3DPtr <Group>            knifeSelectionGroup;
	X3DPtr <TouchSensor>      knifeTouchSensor;
	X3DPtr <Switch>           knifeStartPointSwitch;
	X3DPtr <Transform>        knifeStartPoint;
	X3DPtr <Transform>        knifeEndPoint;
	X3DPtr <Switch>           knifeLineSwitch;
	X3DPtr <CoordinateDouble> knifeLineCoordinate;
	X3DPtr <Switch>           knifeArcSwitch;
	X3DPtr <Transform>        knifeArc;
	X3DPtr <Arc2D>            knifeArcGeometry;

	std::vector <size_t>           cutFaces;     // indices of first coord index of face
	size_t                         cutFaceIndex; // index of cut face in cutFaces
	size_t                         cutFace;      // index of first coord index of face
	std::pair <Vector3d, Vector3d> cutEdge;      // points of cut edge
	std::pair <Vector3d, Vector3d> cutPoints;    // points of visual cut line


	std::vector <int32_t>      startPoints; // One or two points, depending on whether start from point or edge
	std::vector <int32_t>      endPoints;   // One or two points, depending on whether end at point or edge
	std::pair <size_t, size_t> startEdge;   // Start edge, must be determined again when cut
	std::pair <size_t, size_t> endEdge;     // End edge, must be determined again when cut

	bool active;

};

} // X3D
} // titania

#endif
