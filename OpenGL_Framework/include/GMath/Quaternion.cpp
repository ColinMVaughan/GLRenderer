#include "Quaternion.h"


namespace GMath
{

	Quaternion::Quaternion()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 1.0f;
	}


	void Quaternion::SetRotation(vec3f rotationAxis, float rotationAngle)
	{
		x = rotationAxis[0] * sin(rotationAngle / 2);
		y = rotationAxis[1] * sin(rotationAngle / 2);
		z = rotationAxis[2] * sin(rotationAngle / 2);
		w = cos(rotationAngle / 2);
	}

	void Quaternion::Rotate(vec3f rotationAxis, float rotationAngle)
	{
		Quaternion local;
		Quaternion total;
		local.x = rotationAxis[0] * sin(rotationAngle / 2);
		local.y = rotationAxis[1] * sin(rotationAngle / 2);
		local.z = rotationAxis[2] * sin(rotationAngle / 2);
		local.w = cos(rotationAngle / 2);

		total = *this;

		*this = local * total;
		
	}

	void Quaternion::Normalize()
	{
		float magnatude = sqrt(exp2(w) + exp2(x) + exp2(y) + exp2(z));
		w = w / magnatude;
		x = x / magnatude;
		y = y / magnatude;
		z = z / magnatude;
	}

	//Quaternion Multiplication is NOT communitive!
	Quaternion Quaternion::operator*(Quaternion& RHS)
	{
		Quaternion temp;

		temp.w = (w*RHS.w - x*RHS.x - y*RHS.y - z*RHS.z);
		temp.x = (w*RHS.x + x*RHS.w + y*RHS.z - z*RHS.y);
		temp.y = (w*RHS.y - x*RHS.z + y*RHS.w + z*RHS.x);
		temp.z = (w*RHS.z + x*RHS.y - y*RHS.x + z*RHS.w);

		return temp;
	}

	void Quaternion::operator=(Quaternion& RHS)
	{
		x = RHS.x;
		y = RHS.y;
		z = RHS.z;
		w = RHS.w;
	}

	bool Quaternion::operator==(Quaternion& RHS) const
	{
		if (x == RHS.x && y == RHS.y && z == RHS.z && w == RHS.w)
			return true;
		else
			return false;
	}

	//Because we are desling with unit quaternions this could be optimised
	mat4f Quaternion::ToMatrix()
	{
		mat4f temp;
		//Column 1
		temp(0, 0) = exp2(w) + exp2(x) - exp2(y) - exp2(z);
		temp(0, 1) = 2 * x*y + 2 * w*z;
		temp(0, 2) = 2 * x*z - 2 * w*y;

		//column 2
		temp(1, 0) = 2 * x*y - 2 * w*z;
		temp(1, 1) = exp2(w) - exp2(x) + exp2(y) - exp2(z);
		temp(1, 2) = 2 * y*z - 2 * w*x;

		//column 3
		temp(2, 0) = 2 * x*z + 2 * w*y;
		temp(2, 1) = 2 * y*z + 2 * w*x;
		temp(2, 2) = exp2(w) - exp2(x) - exp2(y) + exp2(z);

		//column 4
		temp(3, 3) = 1.0f;

		return temp;
	}

}
