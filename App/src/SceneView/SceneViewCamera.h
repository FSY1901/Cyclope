#pragma once
#include "Cyclope.h"

using namespace Cyclope;

namespace CyclopeEditor {

	class SceneViewCamera {

	public:
		SceneViewCamera() = default;

		void Update(float dt);

		const Camera& GetCamera() const{ return cam; }

	private:
		Camera cam;
		TransformComponent transform;

		Vector3 front{ 0.0f, 0.0f, -1.0f };
		Vector3 up{0.0f, 1.0f, 0.0f};
		Vector3 right{ 1.0f, 0.0f, 0.0f };

		float speed = 5.0f;
		float sensitivity = 0.05f;

		float yaw = 0.0f;
		float pitch = 0.0f;

		float lastX = Application::GetInstance()->GetWindow()->GetWidth() / 2.0f;
		float lastY = Application::GetInstance()->GetWindow()->GetHeight() / 2.0f;

	};

}