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

#include "VolumePicker.h"

#include <Titania/LOG.h>

namespace titania {
namespace X3D {

VolumePicker::VolumePicker () :
            broadphase (new btDbvtBroadphase ()),
collisionConfiguration (new btDefaultCollisionConfiguration ()),
            dispatcher (new btCollisionDispatcher (collisionConfiguration .get ())),
        collisionWorld (new btCollisionWorld (dispatcher .get (), broadphase .get (), collisionConfiguration .get ())),
        compoundShape1 (new btCompoundShape ()),
          motionState1 (new btDefaultMotionState ()),
            rigidBody1 (new btRigidBody (btRigidBody::btRigidBodyConstructionInfo (0, motionState1 .get (), compoundShape1 .get ()))),
        compoundShape2 (new btCompoundShape ()),
          motionState2 (new btDefaultMotionState ()),
            rigidBody2 (new btRigidBody (btRigidBody::btRigidBodyConstructionInfo (0, motionState2 .get (), compoundShape2 .get ())))
{
	collisionWorld -> addCollisionObject (rigidBody1 .get ());	
	collisionWorld -> addCollisionObject (rigidBody2 .get ());	
}

void
VolumePicker::setChildShape1 (const Matrix4f & matrix, const std::shared_ptr <btCompoundShape> & childShape)
{
	setChildShape (compoundShape1, matrix, childShape);
}

void
VolumePicker::setChildShape2 (const Matrix4f & matrix, const std::shared_ptr <btCompoundShape> & childShape)
{
	setChildShape (compoundShape2, matrix, childShape);
}

void
VolumePicker::setChildShape1 (const btTransform & transform, const btVector3 & localScaling, const std::shared_ptr <btCompoundShape> & childShape)
{
	setChildShape (compoundShape1, transform, localScaling, childShape);
}

void
VolumePicker::setChildShape2 (const btTransform & transform, const btVector3 & localScaling, const std::shared_ptr <btCompoundShape> & childShape)
{
	setChildShape (compoundShape2, transform, localScaling, childShape);
}

void
VolumePicker::setChildShape (const std::shared_ptr <btCompoundShape> & compoundShape,
                             const Matrix4f & matrix,
                             const std::shared_ptr <btCompoundShape> & childShape)
{
	if (compoundShape -> getNumChildShapes ())
		compoundShape -> removeChildShapeByIndex (0);

	if (childShape -> getNumChildShapes ())
	{
		auto translation = Vector3f ();
		auto rotation    = Rotation4f ();
		auto scale       = Vector3f ();

		matrix .get (translation, rotation, scale);

		childShape -> setLocalScaling (btVector3 (scale .x (), scale .y (), scale .z ()));
		compoundShape -> addChildShape (getTransform (translation, rotation), childShape .get ());
	}
}

void
VolumePicker::setChildShape (const std::shared_ptr <btCompoundShape> & compoundShape,
                             const btTransform & transform,
                             const btVector3 & localScaling,
                             const std::shared_ptr <btCompoundShape> & childShape)
{
	if (compoundShape -> getNumChildShapes ())
		compoundShape -> removeChildShapeByIndex (0);

	if (childShape -> getNumChildShapes ())
	{
		childShape -> setLocalScaling (localScaling);
		compoundShape -> addChildShape (transform, childShape .get ());
	}
}

btTransform
VolumePicker::getTransform (const Vector3f & translation, const Rotation4f & rotation) const
{
	auto l = btTransform ();
	auto m = Matrix4f ();
	
	m .set (translation, rotation);

	l .setFromOpenGLMatrix (m [0] .data ());

	return l;
}

bool
VolumePicker::contactTest ()
{
	collisionWorld -> performDiscreteCollisionDetection ();

	const auto numManifolds = dispatcher -> getNumManifolds ();

	for (int32_t i = 0; i < numManifolds; ++ i)
	{
		const auto contactManifold = dispatcher -> getManifoldByIndexInternal (i);
		const auto numContacts     = contactManifold -> getNumContacts ();

		for (int32_t j = 0; j < numContacts; ++ j)
		{
			const auto & pt = contactManifold -> getContactPoint (j);

			if (pt .getDistance () <= 0)
				return true;
		}
	}

	return false;
}

VolumePicker::~VolumePicker ()
{ }

} // X3D
} // titania
