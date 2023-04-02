#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "../Maths/Maths.h"

namespace Cyclope {

	struct TransformComponent
	{
		Matrix4 transform{1.0f};

		Vector3 position{0.0f, 0.0f, 0.0f};
		Vector3 scale{ 1.0f, 1.0f, 1.0f };
		Quaternion rotation{ 0.0f, 0.0f, 0.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent& transform) = default;
		TransformComponent(const Matrix4& _transform) : transform(_transform) {}
		TransformComponent(const Vector3& pos, const Quaternion& rot, const Vector3& _scale) 
			: position(pos), rotation(rot), scale(_scale){}
		TransformComponent(const Vector3& pos, const Vector3& rot, const Vector3& _scale)
			: position(pos), rotation(ToQuaternion(rot)), scale(_scale) {}

		operator Matrix4& () { return transform; }
		operator const Matrix4& () const { return transform; }
	};

}

#endif