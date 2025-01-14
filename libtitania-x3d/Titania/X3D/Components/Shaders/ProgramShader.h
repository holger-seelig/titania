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

#ifndef __TITANIA_X3D_COMPONENTS_SHADERS_PROGRAM_SHADER_H__
#define __TITANIA_X3D_COMPONENTS_SHADERS_PROGRAM_SHADER_H__

#include "../Networking/LoadSensor.h"
#include "../Shaders/X3DShaderNode.h"

namespace titania {
namespace X3D {

class ProgramShader :
	public X3DShaderNode
{
public:

	///  @name Construction

	ProgramShader (X3DExecutionContext* const executionContext);

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

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

	///  @name Fields

	MFNode &
	programs ()
	{ return *fields .programs; }

	const MFNode &
	programs () const
	{ return *fields .programs; }

	///  @name Operations

	virtual
	void
	enable () final override;

	virtual
	void
	disable () final override;

	///  @name Pipeline

	virtual
	void
	setGlobalUniforms (X3DRenderObject* const renderObject) final override;

	virtual
	void
	setLocalUniforms (ShapeContainer* const) final override;

	virtual
	void
	enableFloatAttrib (const std::string &, const GLuint, const size_t) final override;
	
	virtual
	void
	disableFloatAttrib (const std::string &) final override;

	virtual
	void
	enableIntegerAttrib (const std::string &, const GLuint, const size_t) final override;
	
	virtual
	void
	disableIntegerAttrib (const std::string &) final override;
	
	virtual
	void
	enableMatrix3Attrib (const std::string &, const GLuint) final override;
	
	virtual
	void
	disableMatrix3Attrib (const std::string &) final override;
	
	virtual
	void
	enableMatrix4Attrib (const std::string &, const GLuint) final override;
	
	virtual
	void
	disableMatrix4Attrib (const std::string &) final override;

	virtual
	void
	enableFogDepthAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer) final override;
	
	virtual
	void
	disableFogDepthAttrib () final override;

	virtual
	void
	enableColorAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer) final override;
	
	virtual
	void
	disableColorAttrib () final override;

	virtual
	void
	enableTexCoordAttrib (const std::vector <GLuint> &,
	                      const GLenum type,
	                      const std::vector <GLsizei> & stride = { },
	                      const std::vector <GLvoid*> & pointer = { }) final override;

	virtual
	void
	disableTexCoordAttrib () final override;

	virtual
	void
	enableNormalAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer) final override;
	
	virtual
	void
	disableNormalAttrib () final override;

	virtual
	void
	enableVertexAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer) final override;

	virtual
	void
	disableVertexAttrib () final override;

	virtual
	void
	setParticle (const size_t id, const SoftParticle & particle, const Matrix4d & modelViewMatrix) final override;

	///  @name Destruction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Operations

	void
	requestExplicitRelink ();

	///  @name Event handlers

	void
	set_activate ();

	void
	set_programs ();

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		MFNode* const programs;
	};

	Fields fields;

	X3DPtrArray <ShaderProgram> programNodes;
	X3DPtr <LoadSensor>         loadSensor;
	GLuint                      pipelineId;

};

} // X3D
} // titania

#endif
