#version 330

#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
precision highp int;
#else
precision mediump float;
precision mediump int;
#endif

uniform mat4 x3d_TextureMatrix [x3d_MaxTextures];
uniform mat3 x3d_NormalMatrix;
uniform mat4 x3d_ProjectionMatrix;
uniform mat4 x3d_ModelViewMatrix;

uniform bool x3d_Lighting;  // true if a X3DMaterialNode is attached, otherwise false

in float x3d_FogDepth;
in vec4  x3d_Color;
in vec3  x3d_Normal;
in vec4  x3d_Vertex;

#if x3d_MaxTextures > 0
in vec4 x3d_TexCoord0;
#endif

#if x3d_MaxTextures > 1
in vec4 x3d_TexCoord1;
#endif

out float fogDepth;    // fog depth
out vec4  color;       // color
out vec3  normal;      // normalized normal vector at this point on geometry
out vec3  vertex;      // point on geometry
out vec3  localNormal; // normal vector at this point on geometry in local coordinates
out vec3  localVertex; // point on geometry in local coordinates

#if x3d_MaxTextures > 0
out vec4  texCoord0;   // texCoord0
#endif

#if x3d_MaxTextures > 1
out vec4  texCoord1;   // texCoord1
#endif

#ifdef X3D_LOGARITHMIC_DEPTH_BUFFER
out float depth;
#endif

void
main ()
{
	vec4 position = x3d_ModelViewMatrix * x3d_Vertex;

	fogDepth    = x3d_FogDepth;
	color       = x3d_Color;
	normal      = x3d_NormalMatrix * x3d_Normal;
	vertex      = position .xyz;
	localNormal = x3d_Normal;
	localVertex = x3d_Vertex .xyz;

	#if x3d_MaxTextures > 0
	texCoord0 = x3d_TextureMatrix [0] * x3d_TexCoord0;
	#endif

	#if x3d_MaxTextures > 1
	texCoord1 = x3d_TextureMatrix [1] * x3d_TexCoord1;
	#endif

	gl_Position = x3d_ProjectionMatrix * position;

	#ifdef X3D_LOGARITHMIC_DEPTH_BUFFER
	depth = 1.0 + gl_Position .w;
	#endif
}
