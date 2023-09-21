#include "Rendering.h"

namespace Cyclope {

	void RenderCommands::SetClearColor(float r, float g, float b) {
		glClearColor(r, g, b, 1.0f);
	}

	void RenderCommands::Clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderCommands::SetDrawMode(DrawMode mode) {
		glPolygonMode(GL_FRONT_AND_BACK, (int)mode);
	}

	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;
	Unique<RendererData> Renderer::s_data = std::make_unique<RendererData>();

	void Renderer::BeginScene(const Camera& camera) {
		s_data->viewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader) {
		shader->Bind();
		shader->SetMat4("viewProjection", s_data->viewProjectionMatrix);
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetSize(), GL_UNSIGNED_INT, 0);
		vertexArray->Unbind();
	}

	RendererAPI Renderer::GetAPI() { return s_RendererAPI; }

}