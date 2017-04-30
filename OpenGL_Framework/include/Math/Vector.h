///////
//DO NOT REMOVE THIS INFORMATION
//Author: Emilian Cioca
//
//USE OF THIS FILE IMPLIES AGREEMENT TO THE FOLLOWING TERM
//1. THIS FILE IS NOT TO BE USED FOR A GDW(game development workshop) PROJECT
//
///////

#pragma once

class vec2
{
public:
	vec2(float X, float Y);
	vec2();
	~vec2();

	bool operator==(const vec2& RHS) const;
	bool operator!=(const vec2& RHS) const;

	vec2& operator=(const vec2& RHS);
	vec2& operator-=(const vec2& RHS);
	vec2& operator+=(const vec2& RHS);
	vec2& operator*=(float scalar);
	vec2& operator/=(float divisor);

	vec2 operator-(const vec2& RHS) const;
	vec2 operator+(const vec2& RHS) const;
	vec2 operator*(float scalar) const;
	vec2 operator/(float divisor) const;
	vec2 operator-() const;

	float operator[](unsigned int index) const;
	float &operator[](unsigned int index);

	static vec2 Zero() { return		vec2(0.0f, 0.0f); }
	static vec2 UnitX() { return	vec2(1.0f, 0.0f); }
	static vec2 UnitY() { return	vec2(0.0f, 1.0f); }
	static vec2 AllOnes() { return	vec2(1.0f, 1.0f); }

	float Length() const;
	float LengthSquared() const;
	//- Returns the angle between this vector and V
	float Angle(const vec2& vec) const;
	vec2 Direction() const;
	void Normalize();
	void Set(float value);
	void Set(float X, float Y);

	union
	{
		struct
		{
			float x, y;
		};

		float data[2];
	};

private:
};


class vec3
{
public:
	vec3(float X, float Y, float Z);
	vec3();
	~vec3();

	bool operator==(const vec3 &RHS) const;
	bool operator!=(const vec3 &RHS) const;

	vec3& operator=(const vec3 &RHS);
	vec3& operator-=(const vec3 &RHS);
	vec3& operator+=(const vec3 &RHS);
	vec3& operator*=(float scalar);
	vec3& operator/=(float divisor);

	vec3 operator-(const vec3 &RHS) const;
	vec3 operator+(const vec3 &RHS) const;
	vec3 operator*(float scalar) const;
	vec3 operator/(float divisor) const;
	vec3 operator-() const;

	float operator[](unsigned int index) const;
	float &operator[](unsigned int index);

	static vec3 Zero() { return		vec3(0.0f, 0.0f, 0.0f); }
	static vec3 UnitX() { return	vec3(1.0f, 0.0f, 0.0f); }
	static vec3 UnitY() { return	vec3(0.0f, 1.0f, 0.0f); }
	static vec3 UnitZ() { return	vec3(0.0f, 0.0f, 1.0f); }
	static vec3 AllOnes() { return	vec3(1.0f, 1.0f, 1.0f); }

	float Length() const;
	float LengthSquared() const;
	//- Returns the angle between this vector and V
	float Angle(const vec3 &vec) const;
	vec3 Direction() const;
	void Normalize();
	void Set(float value);
	void Set(float X, float Y, float Z);

	union
	{
		struct
		{
			float x, y, z;
		};

		float data[3];
	};

private:
};


class vec4
{
public:
	vec4(const vec3 &vec, float W);
	vec4(float X, float Y, float Z, float W);
	vec4();
	~vec4();

	bool operator==(const vec4 &RHS) const;
	bool operator!=(const vec4 &RHS) const;

	vec4& operator=(const vec4 &RHS);
	vec4& operator-=(const vec4 &RHS);
	vec4& operator+=(const vec4 &RHS);
	vec4& operator*=(float scalar);
	vec4& operator/=(float divisor);

	vec4 operator-(const vec4 &RHS) const;
	vec4 operator+(const vec4 &RHS) const;
	vec4 operator*(float scalar) const;
	vec4 operator/(float divisor) const;
	vec4 operator-() const;

	float operator[](unsigned int index) const;
	float &operator[](unsigned int index);

	static vec4 Zero() { return		vec4(0.0f, 0.0f, 0.0f, 0.0f); }
	static vec4 UnitX() { return	vec4(1.0f, 0.0f, 0.0f, 0.0f); }
	static vec4 UnitY() { return	vec4(0.0f, 1.0f, 0.0f, 0.0f); }
	static vec4 UnitZ() { return	vec4(0.0f, 0.0f, 1.0f, 0.0f); }
	static vec4 UnitW() { return	vec4(0.0f, 0.0f, 0.0f, 1.0f); }
	static vec4 AllOnes() { return	vec4(1.0f, 1.0f, 1.0f, 1.0f); }

	float Length() const;
	float LengthSquared() const;
	//- Returns the angle between this vector and V
	float Angle(const vec4 &vec) const;
	vec4 Direction() const;
	void Normalize();
	void Set(float value);
	void Set(float X, float Y, float Z, float W);

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

float Dot(const vec2 &v1, const vec2 &v2);
float Dot(const vec3 &v1, const vec3 &v2);
float Dot(const vec4 &v1, const vec4 &v2);
vec3 Cross(const vec3 &v1, const vec3 &v2);
