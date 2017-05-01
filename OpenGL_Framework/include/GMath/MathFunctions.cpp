//Colin Vaughan 
//December 2016

#include "MathFunctions.h"

namespace GMath
{
	void SetIdentity(mat4f& matrix)
	{
		for(int i=0; i<16; i++)
		{
			matrix(i) = 0.0f;
		}
		matrix(0, 0) = 1.0f;
		matrix(1, 1) = 1.0f;
		matrix(2, 2) = 1.0f;
		matrix(3, 3) = 1.0f;
	}

	void SetZero(mat4f& matrix)
	{
		for(int i = 0; i< 16; ++i)
		{
			matrix(i) = 0.0f;
		}
	}


	mat4f GetMat4Identity()
	{
		mat4f mat;
		SetIdentity(mat);

		return mat;
	}


	//Sets sets a camera view matrix
	void LookAt(mat4f& camera, vec3f eye, vec3f target, vec3f up)
	{
		vec3f forward;
		forward[0] = target[0] - eye[0];
		forward[1] = target[1] - eye[1];
		forward[2] = target[2] - eye[2];

		forward.Normalize();

		vec3f side = Cross(forward, up);
		side.Normalize();

		up = Cross(side, forward);

		SetIdentity(camera);
		camera(0, 0) = side[0];
		camera(0, 1) = side[1];
		camera(0, 2) = side[2];
		camera(0, 3) = -DotProduct(side, eye);

		camera(1, 0) = up[0];
		camera(1, 1) = up[1];
		camera(1, 2) = up[2];
		camera(1, 3) = -DotProduct(up, eye);

		camera(2, 0) = up[0];
		camera(2, 1) = up[1];
		camera(2, 2) = up[2];
		camera(2, 3) = -DotProduct(forward, eye);
	}



	void SetFrustum(mat4f& camera, float l, float r, float b, float t, float n, float f)
	{
		camera(0) = 2 * n / (r - l);
		camera(5) = 2 * n / (t - b);
		camera(8) = (r + l) / (r - l);
		camera(9) = (t + b) / (t - b);
		camera(10) = -(f + n) / (f - n);
		camera(11) = -1;
		camera(14) = -(2 * f * n) / (f - n);
		camera(15) = 0;
	}



	//Sets the projection matrix and clip coodanates. overwrites matrix
	void SetFrustumProjection(mat4f& camera, float fov, float aspect, float near, float far)
	{
		camera = mat4f(0);

		float tanHalfFov = tanf(fov / 2.0f);
		camera(0, 0) = 1.0f / (aspect * tanHalfFov);
		camera(1, 1) = 1.0f / tanHalfFov;
		camera(2, 3) = -1.0f;

		camera(2,2) = -(far + near) / (far - near);
		camera(3, 2) = -(2.0f * far * near) / (far - near);

		return;

		//-----------------------------------------------------
		//float tangent = tanf((fov / 2) * 0.01745329251);
		//float height = near * tangent;
		//float width = height * aspect;
		//float range = near - far;
		
		//float frustumDepth = far - near;
		//float oneOverDepth = 1 / frustumDepth;

		//camera(1, 1) = 1 / tanf(0.5f *fov );
		//camera(0, 0) = 1 * camera(1, 1) / aspect;
		//camera(2, 2) = far * oneOverDepth;
		//camera(3, 2) = (-far * near) * oneOverDepth;
		//camera(2, 3) = 1;
		//camera(3, 3) = 0;


		//camera(0, 0) = 1.0f / (tangent * aspect);
		//camera(1, 1) = 1.0f / tangent;
		//camera(2, 2) = (-near - far) / range;
		//camera(2, 3) = 2.0f * far * near / range;
		//camera(3, 2) = 1.0f;
		//camera(3, 3) = 0.0f;

		//SetFrustum(camera, -width, width, -height, height, near, far);
	}

	void SetOrthographic(mat4f& matrix, float top, float bottom, float left, float right, float near, float far)
	{
		SetZero(matrix);

		matrix(0, 0) =  2 / (right - left);
		matrix(1, 1) =  2 / (top - bottom);
		matrix(2, 2) = -2 / (far - near);
		
		matrix(3, 0) = -((right + left) / (right - left));
		matrix(3, 1) = -((top + bottom) / (top - bottom));
		matrix(3, 2) = -((far + near) / (far - near));
		matrix(3, 3) = 1.0f;

	}

	//Sets a rotation matrix. overrites prevoius matrix.
	void SetRotate(mat4f& matrix, Quaternion rotation)
	{
		matrix = rotation.ToMatrix();
	}

	// Sets the transform component of the matrix
	void SetTranslate(mat4f& matrix, vec3f position)
	{
		matrix(3, 0) = position[0];
		matrix(3, 1) = position[1];
		matrix(3, 2) = position[2];

	}

	void Translate(mat4f& matrix, vec3f translate)
	{
		matrix(3, 0) += translate[0];
		matrix(3, 1) += translate[1];
		matrix(3, 2) += translate[2];
	}

	//Sets the scale component of a matrix.
	void SetScale(mat4f& matrix, vec3f scale)
	{
		matrix(0, 0) = scale[0];
		matrix(1, 1) = scale[1];
		matrix(2, 2) = scale[2];
	}

	void RotateX(mat4f& matrix, float rotation)
	{
	}

	void RotateY(mat4f& matrix, float rotation)
	{
	}

	void RotateZ(mat4f& matrix, float rotation)
	{
	}


	//Ugly AF//
	///////////////////////////////////////////////////////
	mat4f GetInverse(mat4f matrix)
	{
		mat4f inv;
		mat4f invOut;
		double /*inv[16],*/ det;
		int i;

		inv(0) = matrix(5) * matrix(10) * matrix(15) -
			matrix(5) * matrix(11) * matrix(14) -
			matrix(9) * matrix(6) * matrix(15) +
			matrix(9) * matrix(7) * matrix(14) +
			matrix(13) * matrix(6) * matrix(11) -
			matrix(13) * matrix(7) * matrix(10);

		inv(4) = -matrix(4) * matrix(10) * matrix(15) +
			matrix(4) * matrix(11) * matrix(14) +
			matrix(8) * matrix(6) * matrix(15) -
			matrix(8) * matrix(7) * matrix(14) -
			matrix(12) * matrix(6) * matrix(11) +
			matrix(12) * matrix(7) * matrix(10);

		inv(8) = matrix(4) * matrix(9) * matrix(15) -
			matrix(4) * matrix(11) * matrix(13) -
			matrix(8) * matrix(5) * matrix(15) +
			matrix(8) * matrix(7) * matrix(13) +
			matrix(12) * matrix(5) * matrix(11) -
			matrix(12) * matrix(7) * matrix(9);

		inv(12) = -matrix(4) * matrix(9) * matrix(14) +
			matrix(4) * matrix(10) * matrix(13) +
			matrix(8) * matrix(5) * matrix(14) -
			matrix(8) * matrix(6) * matrix(13) -
			matrix(12) * matrix(5) * matrix(10) +
			matrix(12) * matrix(6) * matrix(9);

		inv(1) = -matrix(1) * matrix(10) * matrix(15) +
			matrix(1) * matrix(11) * matrix(14) +
			matrix(9) * matrix(2) * matrix(15) -
			matrix(9) * matrix(3) * matrix(14) -
			matrix(13) * matrix(2) * matrix(11) +
			matrix(13) * matrix(3) * matrix(10);

		inv(5) = matrix(0) * matrix(10) * matrix(15) -
			matrix(0) * matrix(11) * matrix(14) -
			matrix(8) * matrix(2) * matrix(15) +
			matrix(8) * matrix(3) * matrix(14) +
			matrix(12) * matrix(2) * matrix(11) -
			matrix(12) * matrix(3) * matrix(10);

		inv(9) = -matrix(0) * matrix(9) * matrix(15) +
			matrix(0) * matrix(11) * matrix(13) +
			matrix(8) * matrix(1) * matrix(15) -
			matrix(8) * matrix(3) * matrix(13) -
			matrix(12) * matrix(1) * matrix(11) +
			matrix(12) * matrix(3) * matrix(9);

		inv(13) = matrix(0) * matrix(9) * matrix(14) -
			matrix(0) * matrix(10) * matrix(13) -
			matrix(8) * matrix(1) * matrix(14) +
			matrix(8) * matrix(2) * matrix(13) +
			matrix(12) * matrix(1) * matrix(10) -
			matrix(12) * matrix(2) * matrix(9);

		inv(2) = matrix(1) * matrix(6) * matrix(15) -
			matrix(1) * matrix(7) * matrix(14) -
			matrix(5) * matrix(2) * matrix(15) +
			matrix(5) * matrix(3) * matrix(14) +
			matrix(13) * matrix(2) * matrix(7) -
			matrix(13) * matrix(3) * matrix(6);

		inv(6) = -matrix(0) * matrix(6) * matrix(15) +
			matrix(0) * matrix(7) * matrix(14) +
			matrix(4) * matrix(2) * matrix(15) -
			matrix(4) * matrix(3) * matrix(14) -
			matrix(12) * matrix(2) * matrix(7) +
			matrix(12) * matrix(3) * matrix(6);

		inv(10) = matrix(0) * matrix(5) * matrix(15) -
			matrix(0) * matrix(7) * matrix(13) -
			matrix(4) * matrix(1) * matrix(15) +
			matrix(4) * matrix(3) * matrix(13) +
			matrix(12) * matrix(1) * matrix(7) -
			matrix(12) * matrix(3) * matrix(5);

		inv(14) = -matrix(0) * matrix(5) * matrix(14) +
			matrix(0) * matrix(6) * matrix(13) +
			matrix(4) * matrix(1) * matrix(14) -
			matrix(4) * matrix(2) * matrix(13) -
			matrix(12) * matrix(1) * matrix(6) +
			matrix(12) * matrix(2) * matrix(5);

		inv(3) = -matrix(1) * matrix(6) * matrix(11) +
			matrix(1) * matrix(7) * matrix(10) +
			matrix(5) * matrix(2) * matrix(11) -
			matrix(5) * matrix(3) * matrix(10) -
			matrix(9) * matrix(2) * matrix(7) +
			matrix(9) * matrix(3) * matrix(6);

		inv(7) = matrix(0) * matrix(6) * matrix(11) -
			matrix(0) * matrix(7) * matrix(10) -
			matrix(4) * matrix(2) * matrix(11) +
			matrix(4) * matrix(3) * matrix(10) +
			matrix(8) * matrix(2) * matrix(7) -
			matrix(8) * matrix(3) * matrix(6);

		inv(11) = -matrix(0) * matrix(5) * matrix(11) +
			matrix(0) * matrix(7) * matrix(9) +
			matrix(4) * matrix(1) * matrix(11) -
			matrix(4) * matrix(3) * matrix(9) -
			matrix(8) * matrix(1) * matrix(7) +
			matrix(8) * matrix(3) * matrix(5);

		inv(15) = matrix(0) * matrix(5) * matrix(10) -
			matrix(0) * matrix(6) * matrix(9) -
			matrix(4) * matrix(1) * matrix(10) +
			matrix(4) * matrix(2) * matrix(9) +
			matrix(8) * matrix(1) * matrix(6) -
			matrix(8) * matrix(2) * matrix(5);

		det = matrix(0) * inv(0) + matrix(1) * inv(4) + matrix(2) * inv(8) + matrix(3) * inv(12);

		det = 1.0 / det;

		for (i = 0; i < 16; i++)
			invOut(i) = inv(i) * det;

		
		return invOut;
	}
}