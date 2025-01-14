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

#ifndef __TITANIA_X3D_COMPONENTS_SHADERS_X3DPROGRAMMABLE_SHADER_OBJECT_H__
#define __TITANIA_X3D_COMPONENTS_SHADERS_X3DPROGRAMMABLE_SHADER_OBJECT_H__

#include "../../Basic/X3DBaseNode.h"
#include "../../Bits/X3DConstants.h"
#include "../../Browser/Shaders/ShadingType.h"
#include "../../Fields.h"

#include "../../Rendering/ClipPlaneContainer.h"
#include "../../Rendering/OpenGL.h"

namespace titania {
namespace X3D {

class FogContainer;
class LightContainer;
class ProjectiveTextureContainer;
class ShapeContainer;
struct SoftParticle;

class X3DProgrammableShaderObject :
	virtual public X3DBaseNode
{
public:

	///  @name User defined fields handling

	virtual
	bool
	canUserDefinedFields () const final override
	{ return true; }

	///  throws Error <INVALID_NAME>, Error <INVALID_FIELD>, Error <DISPOSED>
	virtual
	void
	addUserDefinedField (const AccessType accessType, const std::string & name, X3DFieldDefinition* const field) override;

	///  throws Error <DISPOSED>
	virtual
	void
	removeUserDefinedField (const std::string & name) override;

	///  @name Member access

	bool
	isExtensionGPUShaderFP64Available () const
	{ return extensionGPUShaderFP64; }

	virtual
	GLuint
	getProgramId () const = 0;

	///  @name Uniform location access clip planes

	GLint
	getNumClipPlanesUniformLocation () const
	{ return x3d_NumClipPlanes; }

	const std::vector <GLint> &
	getClipPlaneUniformLocation () const
	{ return x3d_ClipPlane; }

	///  @name Uniform location access fog

	GLint
	getFogTypeUniformLocation () const
	{ return x3d_FogType; }

	GLint
	getFogColorUniformLocation () const
	{ return x3d_FogColor; }

	GLint
	getFogVisibilityRangeUniformLocation () const
	{ return x3d_FogVisibilityRange; }

	GLint
	getFogMatrixUniformLocation () const
	{ return x3d_FogMatrix; }

	///  @name Uniform location access point properties

	GLint
	getPointPropertiesPointSizeScaleFactorUniformLocation () const
	{ return x3d_PointPropertiesPointSizeScaleFactor; }

	GLint
	getPointPropertiesPointSizeMinValueUniformLocation () const
	{ return x3d_PointPropertiesPointSizeMinValue; }

	GLint
	getPointPropertiesPointSizeMaxValueUniformLocation () const
	{ return x3d_PointPropertiesPointSizeMaxValue; }

	GLint
	getPointPropertiesPointSizeAttenuationUniformLocation () const
	{ return x3d_PointPropertiesPointSizeAttenuation; }

	GLint
	getPointPropertiesColorModeUniformLocation () const
	{ return x3d_PointPropertiesColorMode; }

	///  @name Uniform location access line properties

	GLint
	getLinePropertiesAppliedUniformLocation () const
	{ return x3d_LinePropertiesApplied; }

	GLint
	getLinePropertiesLinewidthScaleFactorUniformLocation () const
	{ return x3d_LinePropertiesLinewidthScaleFactor; }

	GLint
	getLinePropertiesLinetypeUniformLocation () const
	{ return x3d_LinePropertiesLinetype; }

	///  @name Hatch

	GLint
	getFillPropertiesFilledUniformLocation () const
	{ return x3d_FillPropertiesFilled; }

	GLint
	getFillPropertiesHatchedUniformLocation () const
	{ return x3d_FillPropertiesHatched; }

	GLint
	getFillPropertiesHatchColorUniformLocation () const
	{ return x3d_FillPropertiesHatchColor; }

	GLint
	getFillPropertiesHatchStyleUniformLocation () const
	{ return x3d_FillPropertiesHatchStyle; }

	///  @name Uniform location access lighting

	GLint
	getNumLightsUniformLocation () const
	{ return x3d_NumLights; }

	GLint
	getLightingUniformLocation () const
	{ return x3d_Lighting; }

	GLint
	getColorMaterialUniformLocation () const
	{ return x3d_ColorMaterial; }

	const std::vector <GLint> &
	getLightTypeUniformLocation () const
	{ return x3d_LightType; }

	const std::vector <GLint> &
	getLightColorUniformLocation () const
	{ return x3d_LightColor; }

	const std::vector <GLint> &
	getLightIntensityUniformLocation () const
	{ return x3d_LightIntensity; }

	const std::vector <GLint> &
	getLightAmbientIntensityUniformLocation () const
	{ return x3d_LightAmbientIntensity; }

	const std::vector <GLint> &
	getLightAttenuationUniformLocation () const
	{ return x3d_LightAttenuation; }

	const std::vector <GLint> &
	getLightLocationUniformLocation () const
	{ return x3d_LightLocation; }

	const std::vector <GLint> &
	getLightDirectionUniformLocation () const
	{ return x3d_LightDirection; }

	const std::vector <GLint> &
	getLightBeamWidthUniformLocation () const
	{ return x3d_LightBeamWidth; }

	const std::vector <GLint> &
	getLightCutOffAngleUniformLocation () const
	{ return x3d_LightCutOffAngle; }

	const std::vector <GLint> &
	getLightRadiusUniformLocation () const
	{ return x3d_LightRadius; }

	const std::vector <GLint> &
	getLightMatrixUniformLocation () const
	{ return x3d_LightMatrix; }

	///  @name Uniform location access shadow

	const std::vector <GLint> &
	getShadowColorUniformLocation () const
	{ return x3d_ShadowColor; }

	const std::vector <GLint> &
	getShadowIntensityUniformLocation () const
	{ return x3d_ShadowIntensity; }

	const std::vector <GLint> &
	getShadowBiasUniformLocation () const
	{ return x3d_ShadowBias; }

	const std::vector <GLint> &
	getShadowMatrixUniformLocation () const
	{ return x3d_ShadowMatrix; }

	const std::vector <GLint> &
	getShadowMapSizeUniformLocation () const
	{ return x3d_ShadowMapSize; }

	const std::vector <GLint> &
	getShadowMapUniformLocation () const
	{ return x3d_ShadowMap; }

	///  @name Uniform location access material

	GLint
	getSeparateBackColorUniformLocation () const
	{ return x3d_SeparateBackColor; }

	GLint
	getAmbientIntensityUniformLocation () const
	{ return x3d_AmbientIntensity; }

	GLint
	getDiffuseColorUniformLocation () const
	{ return x3d_DiffuseColor; }

	GLint
	getSpecularColorUniformLocation () const
	{ return x3d_SpecularColor; }

	GLint
	getEmissiveColorUniformLocation () const
	{ return x3d_EmissiveColor; }

	GLint
	getShininessUniformLocation () const
	{ return x3d_Shininess; }

	GLint
	getTransparencyUniformLocation () const
	{ return x3d_Transparency; }

	GLint
	getBackAmbientIntensityUniformLocation () const
	{ return x3d_BackAmbientIntensity; }

	GLint
	getBackDiffuseColorUniformLocation () const
	{ return x3d_BackDiffuseColor; }

	GLint
	getBackSpecularColorUniformLocation () const
	{ return x3d_BackSpecularColor; }

	GLint
	getBackEmissiveColorUniformLocation () const
	{ return x3d_BackEmissiveColor; }

	GLint
	getBackShininessUniformLocation () const
	{ return x3d_BackShininess; }

	GLint
	getBackTransparencyUniformLocation () const
	{ return x3d_BackTransparency; }

	///  @name Uniform location access texture

	GLint
	getNumTexturesUniformLocation () const
	{ return x3d_NumTextures; }

	///  @name Multi Texturing

	GLint
	getMultiTextureColorUniformLocation () const
	{ return x3d_MultiTextureColor; }

	const std::vector <GLint> &
	getMultiTextureModeUniformLocation () const
	{ return x3d_MultiTextureMode; }

	const std::vector <GLint> &
	getMultiTextureAlphaModeUniformLocation () const
	{ return x3d_MultiTextureAlphaMode; }

	const std::vector <GLint> &
	getMultiTextureSourceUniformLocation () const
	{ return x3d_MultiTextureSource; }

	const std::vector <GLint> &
	getMultiTextureFunctionUniformLocation () const
	{ return x3d_MultiTextureFunction; }

	///  @name Texture Coordinate Generator

	const std::vector <GLint> &
	getTextureCoordinateGeneratorModeUniformLocation () const
	{ return x3d_TextureCoordinateGeneratorMode; }

	const std::vector <GLint> &
	getTextureCoordinateGeneratorParameterUniformLocation () const
	{ return x3d_TextureCoordinateGeneratorParameter; }

	///  @name Projective Texture Mapping

	GLint
	getNumProjectiveTexturesUniformLocation () const
	{ return x3d_NumProjectiveTextures; }

	const std::vector <GLint> &
	getTextureTypeUniformLocation () const
	{ return x3d_TextureType; }

	const std::vector <GLint> &
	getProjectiveTextureMatrixUniformLocation () const
	{ return x3d_ProjectiveTextureMatrix; }

	const std::vector <GLint> &
	getProjectiveTextureLocationUniformLocation () const
	{ return x3d_ProjectiveTextureLocation; }

	///  @name Matrices

	GLint
	getViewportUniformLocation () const
	{ return x3d_Viewport; }

	GLint
	getProjectionMatrixUniformLocation () const
	{ return x3d_ProjectionMatrix; }

	GLint
	getModelViewMatrixUniformLocation () const
	{ return x3d_ModelViewMatrix; }

	GLint
	getNormalMatrixUniformLocation () const
	{ return x3d_NormalMatrix; }

	const std::vector <GLint> &
	getTextureMatrixUniformLocation () const
	{ return x3d_TextureMatrix; }

	GLint
	getCameraSpaceMatrixUniformLocation () const
	{ return x3d_CameraSpaceMatrix; }

	///  @name Special

	void
	setTransformFeedbackVaryings (const std::vector <std::string> & value)
	{ transformFeedbackVaryings = value; }

	void
	setTextureBuffer (const std::string & name, const GLenum textureTarget, const GLuint textureId);

	///  @name Pipeline

	size_t
	getClipPlaneIndex ();

	size_t
	getLightIndex ();

	size_t
	getProjectiveTextureIndex ();

	bool
	hasFog (FogContainer* const fogContainer);

	bool
	hasLight (const size_t index, LightContainer* const lightContainer);

	bool
	hasProjectiveTexture (const size_t index, ProjectiveTextureContainer* const projectiveTextureContainer);

	virtual
	void
	setGlobalUniforms (X3DRenderObject* const renderObject);

	virtual
	void
	setLocalUniforms (ShapeContainer* const context);

	void
	setLocalObjects (X3DRenderObject* const renderObject, const CollectableObjectArray & localObjects);

	virtual
	void
	enable ();

	virtual
	void
	disable ();

	virtual
	void
	enableFloatAttrib (const std::string & name, const GLuint buffer, const size_t components);

	virtual
	void
	disableFloatAttrib (const std::string & name);

	virtual
	void
	enableIntegerAttrib (const std::string & name, const GLuint buffer, const size_t components);

	virtual
	void
	disableIntegerAttrib (const std::string & name);

	virtual
	void
	enableMatrix3Attrib (const std::string & name, const GLuint buffer);

	virtual
	void
	disableMatrix3Attrib (const std::string & name);

	virtual
	void
	enableMatrix4Attrib (const std::string & name, const GLuint buffer);

	virtual
	void
	disableMatrix4Attrib (const std::string & name);

	virtual
	void
	enableFogDepthAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer);

	virtual
	void
	disableFogDepthAttrib ();

	virtual
	void
	enableColorAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer);

	virtual
	void
	disableColorAttrib ();

	virtual
	void
	enableTexCoordAttrib (const std::vector <GLuint> &,
	                      const GLenum type,
	                      const std::vector <GLsizei> & stride,
	                      const std::vector <GLvoid*> & pointer);

	virtual
	void
	disableTexCoordAttrib ();

	virtual
	void
	enableNormalAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer);

	virtual
	void
	disableNormalAttrib ();

	virtual
	void
	enableVertexAttrib (const GLuint buffer, const GLenum type, const GLsizei stride, const GLvoid* pointer);

	virtual
	void
	disableVertexAttrib ();

	virtual
	void
	setParticle (const size_t id, const SoftParticle & particle, const Matrix4d & modelViewMatrix);


protected:

	///  @name Construction

	X3DProgrammableShaderObject ();

	virtual
	void
	initialize () override;

	///  @name Operations

	void
	setOpenGLES (const bool value);

	void
	applyTransformFeedbackVaryings () const;

	void
	getDefaultUniforms ();

	void
	addShaderFields ();

	virtual
	void
	set_field (X3DFieldDefinition* const field);

	///  @name Destructions

	virtual
	void
	dispose () override;


private:

	///  @name Operations

	GLint
	getUniformLocation (GLuint program, const std::string & name, const std::string & depreciated) const;

	GLint
	getAttribLocation (GLuint program, const std::string & name, const std::string & depreciated) const;

	Matrix3d
	getNormalMatrix (const Matrix4d & modelViewMatrix) const;

	///  @name Event handlers

	void
	set_shading (const ShadingType & shading);

	///  @name Member types

	struct TextureValue
	{
		TextureValue (const std::string & name, const GLenum textureTarget, const GLuint textureId, const int32_t textureUnit) :
			         name (name),
			textureTarget (textureTarget),
			    textureId (textureId),
			  textureUnit (textureUnit)
		{ }

		std::string name;
		GLenum      textureTarget;
		GLuint      textureId;
		int32_t     textureUnit;
	};

	///  @name Members

	GLint x3d_LogarithmicFarFactor1_2;

	GLint               x3d_GeometryType;
	GLint               x3d_NumClipPlanes;
	std::vector <GLint> x3d_ClipPlane;

	GLint x3d_FogType;
	GLint x3d_FogColor;
	GLint x3d_FogVisibilityRange;
	GLint x3d_FogMatrix;
	GLint x3d_FogCoord;

	GLint x3d_PointPropertiesPointSizeScaleFactor;
	GLint x3d_PointPropertiesPointSizeMinValue;
	GLint x3d_PointPropertiesPointSizeMaxValue;
	GLint x3d_PointPropertiesPointSizeAttenuation;
	GLint x3d_PointPropertiesColorMode;

	GLint x3d_LinePropertiesApplied;
	GLint x3d_LinePropertiesLinewidthScaleFactor;
	GLint x3d_LinePropertiesLinetype;

	GLint x3d_FillPropertiesFilled;
	GLint x3d_FillPropertiesHatched;
	GLint x3d_FillPropertiesHatchColor;
	GLint x3d_FillPropertiesHatchStyle;

	GLint x3d_Lighting;
	GLint x3d_ColorMaterial;

	GLint               x3d_NumLights;
	std::vector <GLint> x3d_LightType;
	std::vector <GLint> x3d_LightColor;
	std::vector <GLint> x3d_LightAmbientIntensity;
	std::vector <GLint> x3d_LightIntensity;
	std::vector <GLint> x3d_LightAttenuation;
	std::vector <GLint> x3d_LightLocation;
	std::vector <GLint> x3d_LightDirection;
	std::vector <GLint> x3d_LightBeamWidth;
	std::vector <GLint> x3d_LightCutOffAngle;
	std::vector <GLint> x3d_LightRadius;
	std::vector <GLint> x3d_LightMatrix;

	std::vector <GLint> x3d_ShadowColor;
	std::vector <GLint> x3d_ShadowIntensity;
	std::vector <GLint> x3d_ShadowBias;
	std::vector <GLint> x3d_ShadowMatrix;
	std::vector <GLint> x3d_ShadowMapSize;
	std::vector <GLint> x3d_ShadowMap;

	GLint x3d_SeparateBackColor;

	GLint x3d_AmbientIntensity;
	GLint x3d_DiffuseColor;
	GLint x3d_SpecularColor;
	GLint x3d_EmissiveColor;
	GLint x3d_Shininess;
	GLint x3d_Transparency;

	GLint x3d_BackAmbientIntensity;
	GLint x3d_BackDiffuseColor;
	GLint x3d_BackSpecularColor;
	GLint x3d_BackEmissiveColor;
	GLint x3d_BackShininess;
	GLint x3d_BackTransparency;

	GLint               x3d_NumTextures;
	std::vector <GLint> x3d_TextureType;
	std::vector <GLint> x3d_Texture2D;
	std::vector <GLint> x3d_Texture3D;
	std::vector <GLint> x3d_CubeMapTexture;

	GLint               x3d_MultiTextureColor;
	std::vector <GLint> x3d_MultiTextureMode;
	std::vector <GLint> x3d_MultiTextureAlphaMode;
	std::vector <GLint> x3d_MultiTextureSource;
	std::vector <GLint> x3d_MultiTextureFunction;

	std::vector <GLint> x3d_TextureCoordinateGeneratorMode;
	std::vector <GLint> x3d_TextureCoordinateGeneratorParameter;

	GLint               x3d_NumProjectiveTextures;
	std::vector <GLint> x3d_ProjectiveTexture;
	std::vector <GLint> x3d_ProjectiveTextureMatrix;
	std::vector <GLint> x3d_ProjectiveTextureLocation;

	GLint               x3d_Viewport;
	GLint               x3d_ProjectionMatrix;
	GLint               x3d_ModelViewMatrix;
	GLint               x3d_NormalMatrix;
	std::vector <GLint> x3d_TextureMatrix;
	GLint               x3d_CameraSpaceMatrix;

	GLint               x3d_FogDepth;
	GLint               x3d_Color;
	std::vector <GLint> x3d_TexCoord;
	GLint               x3d_Normal;
	GLint               x3d_Vertex;

	GLint x3d_ParticleId;
	GLint x3d_ParticleLife;
	GLint x3d_ParticleElapsedTime;

	bool                                             extensionGPUShaderFP64;
	std::vector <std::string>                        transformFeedbackVaryings;
	size_t                                           numClipPlanes;
	size_t                                           numLights;
	size_t                                           numProjectiveTextures;
	size_t                                           numGlobalLights;
	size_t                                           numGlobalProjectiveTextures;
	FogContainer*                                    fogContainer;
	std::vector <LightContainer*>                    lightContainers;
	std::vector <ProjectiveTextureContainer*>        projectiveTextureContainers;
	std::map <GLint, std::shared_ptr <TextureValue>> textures;

};

} // X3D
} // titania

#endif
