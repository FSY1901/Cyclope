#include "SceneViewCamera.h"

namespace CyclopeEditor {

	void SceneViewCamera::Update(float dt) {

		if (Input::ButtonPressed(Button::Button_RIGHT)) {

			Input::SetCursorMode(CursorMode::Hidden);

			if (Input::KeyPressed(Key::W))
				transform.position += speed * front * dt;
			if (Input::KeyPressed(Key::S))
				transform.position -= speed * front * dt;
			if (Input::KeyPressed(Key::D))
				transform.position += speed * right * dt;
			if (Input::KeyPressed(Key::A))
				transform.position -= speed * right * dt;

			float xoffset = Input::MouseX() - lastX;
			float yoffset = lastY - Input::MouseY();

			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw -= xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;


			transform.rotation = ToQuaternion(Vector3(pitch, yaw, 0.0f));

			front = glm::normalize(transform.rotation * Vector3(0.0f, 0.0f, -1.0f));
			right = glm::normalize(glm::cross(front, Vector3(0.0f, 1.0f, 0.0f)));
			up = glm::normalize(glm::cross(right, front));

		}
		else {
			Input::SetCursorMode(CursorMode::Shown);
		}

		lastX = Input::MouseX();
		lastY = Input::MouseY();

		cam.SetPerspectiveMatrix(Application::GetInstance()->GetWindow()->GetAspectRatio());
		cam.RecalculateViewMatrix(transform);

	}

}