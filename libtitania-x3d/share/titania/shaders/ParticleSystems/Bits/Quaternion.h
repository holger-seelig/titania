/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-*/

/* Rotation */

vec4
quaternion (in vec3 fromVector, in vec3 toVector)
{
	vec3 from = normalize (fromVector);
	vec3 to   = normalize (toVector);

	float cos_angle = dot (from, to);
	vec3  crossvec  = cross (from, to);
	float crosslen  = length (crossvec);

	if (crosslen == 0.0f)
	{
		if (cos_angle > 0.0f)
			return vec4 (0.0f, 0.0f, 0.0f, 1.0f);

		else
		{
			vec3 t = cross (from, vec3 (1.0f, 0.0f, 0.0f));

			if (dot (t, t) == 0.0f)
				t = cross (from, vec3 (0.0f, 1.0f, 0.0f));

			t = normalize (t);

			return vec4 (t, 0.0f);
		}
	}
	else
	{
		crossvec  = normalize (crossvec);
		crossvec *= sqrt (abs (1.0f - cos_angle) * 0.5f);
		return vec4 (crossvec, sqrt (abs (1.0f + cos_angle) * 0.5f));
	}
}

vec4
inverse (in vec4 quat)
{
	quat .xyz = -quat .xyz;
	return quat;
}

// The name is reversed in glsl, because in glsl matrix operations are reversed.
vec4
mult_right (in vec4 lhs, in vec4 rhs)
{
	return vec4 (lhs .w * rhs .x +
	             lhs .x * rhs .w +
	             lhs .y * rhs .z -
	             lhs .z * rhs .y,

	             lhs .w * rhs .y +
	             lhs .y * rhs .w +
	             lhs .z * rhs .x -
	             lhs .x * rhs .z,

	             lhs .w * rhs .z +
	             lhs .z * rhs .w +
	             lhs .x * rhs .y -
	             lhs .y * rhs .x,

	             lhs .w * rhs .w -
	             lhs .x * rhs .x -
	             lhs .y * rhs .y -
	             lhs .z * rhs .z);
}

// The name is reversed in glsl, because in glsl matrix operations are reversed.
vec3
mult_quat_vec (in vec4 q, in vec3 vector)
{
	float a = q .w * q .w - q .x * q .x - q .y * q .y - q .z * q .z;                     
	float b = 2 * (vector .x * q .x + vector .y * q .y + vector .z * q .z);  
	float c = 2 * q .w;                                       

	return vec3 (a * vector .x + b * q .x + c * (q .y * vector .z - q .z * vector .y),
	             a * vector .y + b * q .y + c * (q .z * vector .x - q .x * vector .z),
	             a * vector .z + b * q .z + c * (q .x * vector .y - q .y * vector .x));
}
