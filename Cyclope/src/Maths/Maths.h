#ifndef CYMATHS_H
#define CYMATHS_H

#include <iostream>
#include <cmath>

#include "Core.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/matrix_decompose.hpp>

namespace Cyclope {

	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;
	using Quaternion = glm::quat;
	using Matrix4 = glm::mat4;


	const double PI = 3.1415926;

	double CYCLOPE_API RadiansToDegrees(double radians);

	double CYCLOPE_API DegreesToRadians(double degrees);

	float CYCLOPE_API Clamp(float a, float b, float value);

	//Calculates the angle between to Vector3s and returns the result in RADIANS.
	float CYCLOPE_API AngleBetween(Vector3 v1, Vector3 v2);

	Vector3 CYCLOPE_API Lerp(Vector3 v1, Vector3 v2, float t);
	Vector3 CYCLOPE_API ToEulerAngles(Quaternion q);

	Quaternion CYCLOPE_API ToQuaternion(Vector3 eulerAngles);
	Quaternion CYCLOPE_API LookAt(Vector3 direction, Vector3 forward = { 0, 0, 1 }, Vector3 up = { 0, 1, 0 });

	Quaternion RotationBetweenVectors(Vector3 forward, Vector3 direction);

	bool DecomposeTransform(const Matrix4& transform, Vector3& translation, Vector3& rotation, Vector3& scale);

}

#endif // !MATHS_H