///////
//DO NOT REMOVE THIS INFORMATION
//Author: Emilian Cioca
//
//USE OF THIS FILE IMPLIES AGREEMENT TO THE FOLLOWING TERM
//1. THIS FILE IS NOT TO BE USED FOR A GDW(game development workshop) PROJECT
//
///////

#pragma once

class vec2;
class vec3;
class vec4;

class mat3
{
public:
	//Indices
	enum
	{
		RightX, UpX, DirX, 
		RightY, UpY, DirY, 
		RightZ, UpZ, DirZ
	};

	mat3();
	mat3(const vec3 &right, const vec3 &up, const vec3 &forward);
	mat3(float f1, float f2, float f3, 
		float f4, float f5, float f6, 
		float f7, float f8, float f9);
	mat3(float axisX, float axisY, float axisZ, float degrees, float scaleX, float scaleY, float scaleZ);
	~mat3();

	mat3 &operator=(const mat3 &M);
	mat3 &operator*=(const mat3 &M);
	mat3 &operator*=(float scalar);
	mat3 &operator+=(const mat3 &M);
	mat3 &operator-=(const mat3 &M);
	mat3 operator*(const mat3 &M) const;
	mat3 operator+(const mat3 &M) const;
	mat3 operator-(const mat3 &M) const;
	vec3 operator*(const vec3 &V) const;
	mat3 operator*(float scalar) const;
	mat3 operator/(float scalar) const;
	float operator[](unsigned int index) const;
	float &operator[](unsigned int index);

	void LoadIdentity();
	void Transpose();
	void Inverse();
	void Flood(float value);
	mat3 GetTranspose() const;
	mat3 GetInverse() const;
	float GetDet() const;

	void Scale(float scaleX, float scaleY, float scaleZ);
	void Scale(const vec3 &V);
	void Scale(float scale);
	void Rotate(float axisX, float axisY, float axisZ, float degrees);
	void Rotate(const vec3 &V, float degrees);
	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);

	void SetRightVec(const vec3 &V);
	void SetRightVec(float x, float y, float z);
	void SetUpVec(const vec3 &V);
	void SetUpVec(float x, float y, float z);
	void SetDirectionVec(const vec3 &V);
	void SetDirectionVec(float x, float y, float z);

	vec3 GetRightVec() const;
	vec3 GetUpVec() const;
	vec3 GetDirectionVec() const;

	static mat3 Identity();

	void Set(float f1, float f2, float f3, 
		float f4, float f5, float f6, 
		float f7, float f8, float f9);

	float data[9];

private:
};

class mat4
{
public:
	//Indices
	enum
	{
		RightX, UpX, DirX, PosX, 
		RightY, UpY, DirY, PosY, 
		RightZ, UpZ, DirZ, PosZ
	};

	mat4();
	mat4(const mat3 &orientation, const vec3 &translation);
	mat4(float f1, float f2, float f3, float f4, 
		float f5, float f6, float f7, float f8, 
		float f9, float f10, float f11, float f12, 
		float f13, float f14, float f15, float f16);
	//creates a projection matrix
	mat4(float fovyDegrees, float aspect, float zNear, float zFar);
	//creates a orthographic projection matrix
	mat4(float top, float bottom, float left, float right, float near, float far);
	//creates a homogeneous rotation matrix
	mat4(float axisX, float axisY, float axisZ, float degrees, float scaleX, float scaleY, float scaleZ, float translateX, float translateY, float translateZ);
	~mat4();

	mat4 &operator=(const mat4 &M);
	mat4 &operator*=(const mat4 &M);
	mat4 &operator*=(float scalar);
	mat4 &operator+=(const mat4 &M);
	mat4 &operator-=(const mat4 &M);
	mat4 operator*(const mat4 &M) const;
	mat4 operator+(const mat4 &M) const;
	mat4 operator-(const mat4 &M) const;
	vec4 operator*(const vec4 &V) const;
	mat4 operator*(float scalar) const;
	mat4 operator/(float scalar) const;
	float operator[](unsigned int index) const;
	float &operator[](unsigned int index);

	void LoadIdentity();
	void Transpose();
	void Inverse();
	void Flood(float value);
	mat4 GetTranspose() const;
	mat4 GetInverse() const;

	void Scale(float scaleX, float scaleY, float scaleZ);
	void Scale(const vec3 &V);
	void Scale(float scale);
	void Rotate(float axisX, float axisY, float axisZ, float degrees);
	void Rotate(const vec3 &V, float degrees);
	void RotateX(float degrees);
	void RotateY(float degrees);
	void RotateZ(float degrees);
	void Translate(const vec3 &translation);
	void Translate(float translationX, float translationY, float translationZ);
	void FrustumProjection(float fovyDegrees, float aspect, float zNear, float zFar);
	void FrustumProjection(float fovyDegrees, float aspect, float zNear, float zFar, mat4 *inverseOut);
	void OrthoProjection(float top, float bottom, float left, float right, float near, float far);
	void OrthoProjection(float top, float bottom, float left, float right, float near, float far, mat4 *inverseOut);
	void LookAt(const vec3& position, const vec3& target, const vec3& upVector);

	void SetRightVec(const vec3 &V);
	void SetRightVec(float x, float y, float z);
	void SetUpVec(const vec3 &V);
	void SetUpVec(float x, float y, float z);
	void SetDirectionVec(const vec3 &V);
	void SetDirectionVec(float x, float y, float z);
	void SetPositionVec(const vec3 &V);
	void SetPositionVec(float x, float y, float z);

	vec3 GetRightVec() const;
	vec3 GetUpVec() const;
	vec3 GetDirectionVec() const;
	vec3 GetPositionVec() const;
	mat3 GetRotationMat() const;

	static mat4 Identity();

	void Set(float f1, float f2, float f3, float f4, 
		float f5, float f6, float f7, float f8, 
		float f9, float f10, float f11, float f12, 
		float f13, float f14, float f15, float f16);

	float data[16];

private:
};
