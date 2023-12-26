#include "SceneViewCamera.h"
#include "../Layers/EditorLayer.h"

namespace CyclopeEditor {

	void SceneViewCamera::Update(float dt) {

		if (Input::ButtonPressed(Button::Button_RIGHT) && (sceneWindowHovered || controlling)) {

			controlling = true;

			Input::SetCursorMode(CursorMode::Hidden);

			if (Input::KeyPressed(Key::LEFT_SHIFT))
				speed = 10.0f;
			else
				speed = 5.0f;

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
			controlling = false;
		}

		lastX = Input::MouseX();
		lastY = Input::MouseY();

		//cam.SetPerspectiveMatrix(Application::GetInstance()->GetWindow()->GetAspectRatio());
		float aspect = (float)EditorLayer::GetEditorLayer()->GetPanelSize().x / EditorLayer::GetEditorLayer()->GetPanelSize().y;
		cam.SetPerspectiveMatrix(aspect);//need to do this because scene is rendered in a panel
		cam.RecalculateViewMatrix(transform);

	}

}