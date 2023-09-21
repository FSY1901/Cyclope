#pragma once

#include "Core.h"

#include <glm.hpp>

namespace Cyclope {

	class TransformComponent;

	enum class CYCLOPE_API ProjectionType {
		Perspective,
		Orthographic
	};

	class CYCLOPE_API Camera {

	public:
		Camera() = default;

		float zNear = 0.1f;
		float zFar = 1000.0f;
		float fov = 45.0f;

		void RecalculateViewMatrix(TransformComponent transform);

		void SetPerspectiveMatrix(float aspectRatio);
		void SetOrthographicMatrix(float left, float right, float bottom, float top);

		const glm::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_viewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

		ProjectionType GetProjectionType() { return m_projection; }

	private:
		ProjectionType m_projection = ProjectionType::Perspective;

		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_viewProjectionMatrix;

	};

}