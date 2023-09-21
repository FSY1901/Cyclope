#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "../Maths/Maths.h"
#include "../Rendering/Camera.h"

namespace Cyclope {

	class Scene;

	struct TransformComponent
	{
		Vector3 position = { 0.0f, 0.0f, 0.0f };
		Quaternion rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform() const
		{
			glm::mat4 rot = glm::mat4_cast(rotation);

			return glm::translate(glm::mat4(1.0f), position)
				* rot
				* glm::scale(glm::mat4(1.0f), scale);;
		}

	};

	struct CameraComponent {
		Camera camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		operator Camera& () { return camera; }
		operator const Camera& () const { return camera; }
	};

}

#endif