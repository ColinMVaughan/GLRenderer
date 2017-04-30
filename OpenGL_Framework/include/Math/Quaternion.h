///////
//DO NOT REMOVE THIS INFORMATION
//Author: Emilian Cioca
//
//USE OF THIS FILE IMPLIES AGREEMENT TO THE FOLLOWING TERM
//1. THIS FILE IS NOT TO BE USED FOR A GDW(game development workshop) PROJECT
//
///////

#pragma once

class vec3;

class quat
{
public:
	quat();
	quat(float X, float Y, float Z, float W);
	quat(const vec3 &v, float W);
	~quat();

	quat operator*(const quat &other) const;
	quat &operator*=(const quat &other);

	void Conjugate();
	quat GetConjugate() const;
	void Normalize();
	quat GetNormalized() const;

	void Rotate(const vec3 &axis, float degrees);
	void Rotate(float x, float y, float z, float degrees);
	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);
		
	static float Dot(const quat &p0, const quat &p1);
	static quat Slerp(const quat &p0, const quat &p1, float percent);

	union
	{
		struct
		{
			float x, y, z, w;
		};

		float data[4];
	};

private:
};
