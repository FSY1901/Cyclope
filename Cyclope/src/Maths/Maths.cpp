#include "Cpch.h"
#include "Maths.h"

namespace Cyclope {

#pragma region Angle Conversion
	double RadiansToDegrees(double radians) {
		return radians * 180.0 / PI;
	}

	double DegreesToRadians(double degrees) {
		return degrees * PI / 180.0;
	}
#pragma endregion

	float Clamp(float a, float b, float value) {
		if (value < a)
			return a;
		else if (value > b)
			return b;
		else
			return value;
	}

#pragma region Decompose Matrix
	bool DecomposeTransform(const Matrix4& transform, Vector3& translation, Vector3& rotation, Vector3& scale)
	{
		// From glm::decompose in matrix_decompose.inl & https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/Math/Math.cpp

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (LocalMatrix[3][3] == static_cast<T>(0))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (LocalMatrix[0][3] != static_cast<T>(0) || LocalMatrix[1][3] != static_cast<T>(0) || LocalMatrix[2][3] != static_cast<T>(0))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		translation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}

		return true;
	}
#pragma endregion

	float AngleBetween(Vector3 v1, Vector3 v2) {
		float res = glm::dot(v1, v2) / (v1.length() * v2.length());
		float radians = cosf(res);
		return radians;
	}

	Vector3 Lerp(Vector3 v1, Vector3 v2, float t) {
		t = Clamp(0.0f, 1.0f, t);

		Vector3 res;

		res.x = v1.x + (v2.x - v1.x) * t;
		res.y = v1.y + (v2.y - v1.y) * t;
		res.z = v1.z + (v2.z - v1.z) * t;

		return res;
	}

	Vector3 ToEulerAngles(Quaternion q) {
		Vector3 angles;

		// roll (x-axis rotation)
		double sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
		double cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
		angles.x = RadiansToDegrees(std::atan2(sinr_cosp, cosr_cosp));

		// pitch (y-axis rotation)
		double sinp = 2 * (q.w * q.y - q.z * q.x);
		if (std::abs(sinp) >= 1)
			angles.y = RadiansToDegrees(std::copysign(PI / 2, sinp)); // use 90 degrees if out of range
		else
			angles.y = RadiansToDegrees(std::asin(sinp));

		// yaw (z-axis rotation)
		double siny_cosp = 2 * (q.w * q.z + q.x * q.y);
		double cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
		angles.z = RadiansToDegrees(std::atan2(siny_cosp, cosy_cosp));

		return angles;
	}

	Quaternion ToQuaternion(Vector3 eulerAngles) {

		double yaw = DegreesToRadians((double)eulerAngles.y);
		double pitch = DegreesToRadians((double)eulerAngles.x);
		double roll = DegreesToRadians((double)eulerAngles.z);

		double cy = cos(yaw * 0.5);
		double sy = sin(yaw * 0.5);
		double cx = cos(pitch * 0.5);
		double sx = sin(pitch * 0.5);
		double cz = cos(roll * 0.5);
		double sz = sin(roll * 0.5);

		Quaternion q;

		q.w = cz * cx * cy + sx * sy * sz;
		q.x = cz * sx * cy - cx * sz * sy;
		q.y = cz * cx * sy + sz * sx * cy;
		q.z = sz * cx * cy - cz * sx * sy;

		return q;

	}

	Quaternion LookAt(Vector3 direction, Vector3 forward, Vector3 up) {

		Quaternion rot1 = RotationBetweenVectors(forward, direction);
		Vector3 right = glm::cross(direction, up);
		up = glm::cross(right, direction);
		Vector3 realUp(0, 1, 0);
		Vector3 newUp = rot1 * realUp;
		Quaternion rot2 = RotationBetweenVectors(newUp, up);
		Quaternion res = rot2 * rot1;

		return Quaternion(res.x, res.y, res.z, res.w);

	}

	Quaternion RotationBetweenVectors(Vector3 forward, Vector3 direction) {
		forward = glm::normalize(forward);
		direction = glm::normalize(direction);

		float cosTheta = glm::dot(forward, direction);
		Vector3 axis;

		if (cosTheta < -1 + 0.001f) {
			// special case when vectors in opposite directions:
			// there is no "ideal" rotation axis
			// So guess one; any will do as long as it's perpendicular to start
			axis = glm::cross(Vector3(0.0f, 0.0f, 1.0f), forward);

			if (axis.length() * axis.length() < 0.01)
				axis = glm::cross(Vector3(1.0f, 0.0f, 0.0f), forward);

			axis = glm::normalize(axis);
			return Quaternion(axis.x, axis.y, axis.z, DegreesToRadians(0));
		}

		axis = cross(forward, direction);
		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return Quaternion(
			axis.x * invs,
			axis.y * invs,
			axis.z * invs,
			s * 0.5f
		);
	}

}