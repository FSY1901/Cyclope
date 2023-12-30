#include "Grid.h"

namespace CyclopeEditor {

	Grid::Grid() {

		float vertices[] = {
				 1.0f,  1.0f, 0.0f, // top right
				 1.0f, -1.0f, 0.0f, // bottom right
				-1.0f, -1.0f, 0.0f, // bottom left
				-1.0f,  1.0f, 0.0f, // top left 
		};
		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		auto v = VertexBuffer::Create(vertices, 12, { {ShaderDataType::Float3} });
		m_VA = VertexArray::Create(v, IndexBuffer::Create(&indices[0], 6));

		m_shader = Shader::Create("./Resources/shaders/grid.glsl");

	}

	void Grid::Render(const SceneViewCamera& svc) {

		m_shader->Bind();
		m_shader->SetMat4("proj", svc.GetCamera().GetProjectionMatrix());
		m_shader->SetMat4("view", svc.GetCamera().GetViewMatrix());
		Renderer::Submit(m_VA, m_shader);

	}

}