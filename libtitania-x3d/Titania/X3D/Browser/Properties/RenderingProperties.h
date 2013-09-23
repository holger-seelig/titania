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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_X3D_BROWSER_PROPERTIES_RENDERING_PROPERTIES_H__
#define __TITANIA_X3D_BROWSER_PROPERTIES_RENDERING_PROPERTIES_H__

#include "../../Components/Core/X3DNode.h"
#include <Titania/Chrono/StopWatch.h>

#include <set>
#include <stack>

namespace titania {
namespace X3D {

// Property Name     Value data type      Description
// Shading           String               The type of shading algorithm in use. Typical values are Flat, Gouraud, Phong, Wireframe.
// MaxTextureSize    Integer              The maximum texture size supported. The format shall be WIDTHxHEIGHT describing the number of pixels in each direction (for example 1024x1024).
// TextureUnits      Integer              The number of texture units supported for doing multitexture.
// AntiAliased       Boolean              True or false if the rendering is currently anti-aliased or not
// ColorDepth        Integer              The number of bits of colour depth supported by the screen. Allows for optimized selection of textures, particularly for lower colour depth screen capabilities.
// TextureMemory     Float                The amount of memory in megabytes available for textures to be placed on the video card.

class RenderingProperties :
	public X3DNode
{
public:

	RenderingProperties (X3DExecutionContext* const);

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final
	{ return containerField; }

	///  @name Fields

	SFBool &
	enabled ()
	{ return fields .enabled; }

	const SFBool &
	enabled () const
	{ return fields .enabled; }

	SFFloat &
	cycleInterval ()
	{ return fields .cycleInterval; }

	const SFFloat &
	cycleInterval () const
	{ return fields .cycleInterval; }

	SFString &
	vendor ()
	{ return *fields .vendor; }

	const SFString &
	vendor () const
	{ return *fields .vendor; }

	SFString &
	renderer ()
	{ return *fields .renderer; }

	const SFString &
	renderer () const
	{ return *fields .renderer; }

	SFString &
	version ()
	{ return *fields .version; }

	const SFString &
	version () const
	{ return *fields .version; }

	SFInt32 &
	maxThreads ()
	{ return *fields .maxThreads; }

	const SFInt32 &
	maxThreads () const
	{ return *fields .maxThreads; }

	SFString &
	shading ();

	const SFString &
	shading () const;

	SFInt32 &
	maxTextureSize ()
	{ return *fields .maxTextureSize; }

	const SFInt32 &
	maxTextureSize () const
	{ return *fields .maxTextureSize; }

	SFInt32 &
	textureUnits ()
	{ return *fields .textureUnits; }

	const SFInt32 &
	textureUnits () const
	{ return *fields .textureUnits; }

	SFInt32 &
	maxLights ()
	{ return *fields .maxLights; }

	const SFInt32 &
	maxLights () const
	{ return *fields .maxLights; }

	SFBool &
	antialiased ();

	const SFBool &
	antialiased () const;

	SFInt32 &
	colorDepth ()
	{ return *fields .colorDepth; }

	const SFInt32 &
	colorDepth () const
	{ return *fields .colorDepth; }

	SFDouble &
	textureMemory ()
	{ return *fields .textureMemory; }

	const SFDouble &
	textureMemory () const
	{ return *fields .textureMemory; }

	double
	fps () const
	{ return 1 / clock .average (); }

	bool
	hasExtension (const std::string &);

	size_t
	getAvailableTextureMemory ();

	void
	prepare ();

	void
	display ();

	virtual
	void
	toStream (std::ostream &) const final;

	virtual
	void
	dispose () final;


private:

	virtual
	RenderingProperties*
	create (X3DExecutionContext* const) const final;

	virtual
	void
	initialize () final;

	void
	set_enabled ();

	void
	set_fontFamily ();

	void
	reset ();

	void
	build ();

	void
	update ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool enabled;
		SFFloat cycleInterval;
		SFString* const vendor;
		SFString* const renderer;
		SFString* const version;
		SFInt32* const maxThreads;
		SFInt32* const maxTextureSize;
		SFInt32* const textureUnits;
		SFInt32* const maxLights;
		SFInt32* const colorDepth;
		SFDouble* const textureMemory;
	};

	Fields fields;

	std::set <std::string> extensions;

	chrono::stopwatch <double> clock;
	chrono::stopwatch <double> renderClock;

	SFString                  fontFamily;
	size_t                    fontHeigth;
	GLuint                    fontListBase;
	XFontStruct*              fontInfo;
	GLuint                    listId;
	std::vector <std::string> string;

};

} // X3D
} // titania

#endif
