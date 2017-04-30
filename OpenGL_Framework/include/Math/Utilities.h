///////
//DO NOT REMOVE THIS INFORMATION
//Author: Emilian Cioca
//
//USE OF THIS FILE IMPLIES AGREEMENT TO THE FOLLOWING TERM
//1. THIS FILE IS NOT TO BE USED FOR A GDW(game development workshop) PROJECT
//
///////

#pragma once

class mat3;
class mat4;
class vec2;
class vec3;
class vec4;
class quat;

#define PI 3.14159265358979323846f

float ToRadians(float degrees);
float ToDegrees(float radians);

mat3 ToMat3(const quat &q);
quat ToQuat(const mat3 &mat);

template<class T>
//- clamps data to the range [min, max]
T Clamp(const T &data, const T &min, const T &max)
{
	if (data > max)
	{
		return max;
	}
	else if (data < min)
	{
		return min;
	}
	else
	{
		return data;
	}
}

//Linear interpolation
template<class T>
T LERP(const T &data1, const T &data2, float u)
{
	return data1 * (1.0f - u) + data2 * u;
}
