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

#ifndef __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_JOINT_H__
#define __TITANIA_X3D_COMPONENTS_H_ANIM_HANIM_JOINT_H__

#include "../Grouping/X3DTransformNode.h"

namespace titania {
namespace X3D {

class HAnimJoint :
	virtual public X3DTransformNode
{
public:

	///  @name Construction

	HAnimJoint (X3DExecutionContext* const executionContext);

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
	SFString &
	name ()
	{ return *fields .name; }

	virtual
	const SFString &
	name () const
	{ return *fields .name; }

	virtual
	MFFloat &
	llimit ()
	{ return *fields .llimit; }

	virtual
	const MFFloat &
	llimit () const
	{ return *fields .llimit; }

	virtual
	MFFloat &
	ulimit ()
	{ return *fields .ulimit; }

	virtual
	const MFFloat &
	ulimit () const
	{ return *fields .ulimit; }

	virtual
	SFRotation &
	limitOrientation ()
	{ return *fields .limitOrientation; }

	virtual
	const SFRotation &
	limitOrientation () const
	{ return *fields .limitOrientation; }

	virtual
	MFFloat &
	stiffness ()
	{ return *fields .stiffness; }

	virtual
	const MFFloat &
	stiffness () const
	{ return *fields .stiffness; }

	virtual
	MFInt32 &
	skinCoordIndex ()
	{ return *fields .skinCoordIndex; }

	virtual
	const MFInt32 &
	skinCoordIndex () const
	{ return *fields .skinCoordIndex; }

	virtual
	MFFloat &
	skinCoordWeight ()
	{ return *fields .skinCoordWeight; }

	virtual
	const MFFloat &
	skinCoordWeight () const
	{ return *fields .skinCoordWeight; }

	virtual
	MFNode &
	displacers ()
	{ return *fields .displacers; }

	virtual
	const MFNode &
	displacers () const
	{ return *fields .displacers; }

	///  @name Member access

	virtual
	const SFBool &
	isCameraObject () const override
	{ return cameraObject; }

	virtual
	const X3DPtrArray <HAnimDisplacer> &
	getDisplacers () const
	{ return displacerNodes; }

	virtual
	const Matrix4d &
	getModelMatrix () const
	{ return modelMatrix; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;

	///  @name Destruction

	virtual
	~HAnimJoint () override;


protected:

	///  @name Contruction

	virtual
	void
	initialize () override;


private:

	///  @name Event handlers

	void
	set_cameraObject ();

	void
	set_displacers ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFString* const name;
		MFFloat* const llimit;
		MFFloat* const ulimit;
		SFRotation* const limitOrientation;
		MFFloat* const stiffness;
		MFInt32* const skinCoordIndex;
		MFFloat* const skinCoordWeight;
		MFNode* const displacers;
	};

	Fields fields;

	///  @name Members

	SFBool                       cameraObject;
	X3DPtrArray <HAnimDisplacer> displacerNodes;
	Matrix4d                     modelMatrix;

};

} // X3D
} // titania

#endif
