#include "Camera.h"

#include "../Game/Components.h"

namespace Cyclope {

	void Camera::RecalculateViewMatrix(TransformComponent transform) {
		m_viewMatrix = glm::inverse(transform.GetTransform());
		m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void Camera::SetPerspectiveMatrix(float aspectRatio) {
		m_projection = ProjectionType::Perspective;
		m_projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
	}

	void Camera::SetOrthographicMatrix(float left, float right, float bottom, float top) {
		m_projection = ProjectionType::Orthographic;
		m_projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);
	}

}