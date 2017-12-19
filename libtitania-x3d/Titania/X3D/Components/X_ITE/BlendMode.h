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

#ifndef __TITANIA_X3D_COMPONENTS_X_ITE_BLEND_MODE_H__
#define __TITANIA_X3D_COMPONENTS_X_ITE_BLEND_MODE_H__

#include "../Grouping/X3DGroupingNode.h"

namespace titania {
namespace X3D {

class BlendMode :
	virtual public X3DGroupingNode
{
public:

	///  @name Construction

	BlendMode (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	virtual
	SFBool &
	enabled ()
	{ return *fields .enabled; }

	virtual
	const SFBool &
	enabled () const
	{ return *fields .enabled; }

	virtual
	SFString &
	sourceRGB ()
	{ return *fields .sourceRGB; }

	virtual
	const SFString &
	sourceRGB () const
	{ return *fields .sourceRGB; }

	virtual
	SFString &
	sourceAlpha ()
	{ return *fields .sourceAlpha; }

	virtual
	const SFString &
	sourceAlpha () const
	{ return *fields .sourceAlpha; }

	virtual
	SFString &
	destinationRGB ()
	{ return *fields .destinationRGB; }

	virtual
	const SFString &
	destinationRGB () const
	{ return *fields .destinationRGB; }

	virtual
	SFString &
	destinationAlpha ()
	{ return *fields .destinationAlpha; }

	virtual
	const SFString &
	destinationAlpha () const
	{ return *fields .destinationAlpha; }

	virtual
	SFString &
	equationRGB ()
	{ return *fields .equationRGB; }

	virtual
	const SFString &
	equationRGB () const
	{ return *fields .equationRGB; }

	virtual
	SFString &
	equationAlpha ()
	{ return *fields .equationAlpha; }

	virtual
	const SFString &
	equationAlpha () const
	{ return *fields .equationAlpha; }

	///  @name Member access

	GLenum
	getSourceRGB () const
	{ return sourceRGBType; }

	GLenum
	getSourceAlpha () const
	{ return sourceAlphaType; }

	GLenum
	getDestinationRGB () const
	{ return destinationRGBType; }

	GLenum
	getDestinationAlpha () const
	{ return destinationAlphaType; }

	GLenum
	getEquationRGB () const
	{ return equationRGBType; }

	GLenum
	getEquationAlpha () const
	{ return equationAlphaType; }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) override;


protected:

	virtual
	void
	initialize () override;


private:

	///  @name Event handler

	void
	set_sourceRGB ();

	void
	set_sourceAlpha ();

	void
	set_destinationRGB ();

	void
	set_destinationAlpha ();

	void
	set_equationRGB ();

	void
	set_equationAlpha ();

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	static const std::map <std::string, GLenum> blendingModes;
	static const std::map <std::string, GLenum> blendingEquations;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const enabled;
		SFString* const sourceRGB;
		SFString* const sourceAlpha;
		SFString* const destinationRGB;
		SFString* const destinationAlpha;
		SFString* const equationRGB;
		SFString* const equationAlpha;
	};

	Fields fields;

	GLenum sourceRGBType;
	GLenum sourceAlphaType;
	GLenum destinationRGBType;
	GLenum destinationAlphaType;
	GLenum equationRGBType;
	GLenum equationAlphaType;

};

} // X3D
} // titania

#endif
