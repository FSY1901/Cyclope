#include "Rendering.h"

namespace Cyclope {

	void RenderCommands::SetClearColor(float r, float g, float b) {
		glClearColor(r, g, b, 1.0f);
	}

	void RenderCommands::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RenderCommands::SetDrawMode(DrawMode mode) {
		glPolygonMode(GL_FRONT_AND_BACK, (int)mode);
	}

	void RenderCommands::SetViewport(int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void RenderCommands::Enable(RenderingOperation op) {
		glEnable((int)op);
		if(op == RenderingOperation::Blending)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RenderCommands::Disable(RenderingOperation op) {
		glDisable((int)op);
	}

	void Batch::AddToBatch(const Mesh& mesh, const Matrix4& transform)
	{
		for (const auto& vert : mesh.vertices) {
			glm::vec3 pos = transform * glm::vec4(vert.position, 1.0f);
			vertices.push_back(Vertex{ pos, vert.uv });
		}

		for (const auto& i : mesh.indices) {
			indices.push_back(lastTotalVertices + i);
		}

		lastTotalVertices += mesh.vertices.size();
	}

	void Batch::GenerateBatch() {
		glGenVertexArrays(1, &batchVertexArray);
		glBindVertexArray(batchVertexArray);

		glGenBuffers(1, &batchVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, batchVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &batchIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	RendererAPI Renderer::s_RendererAPI = RendererAPI::OpenGL;
	RendererData Renderer::s_data = RendererData();
	RendererStats Renderer::s_stats = RendererStats();

	void Renderer::BeginScene(const Camera& camera) {
		s_data.viewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_stats.drawCalls = 0;
		s_stats.renderedVertices = 0;
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Shared<Batch>& batch, const Shared<Shader>& shader)
	{
		glBindVertexArray(batch->batchVertexArray);
		shader->Bind();
		shader->SetMat4("viewProjection", s_data.viewProjectionMatrix);
		glDrawElements(GL_TRIANGLES, batch->indices.size() * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		s_stats.renderedVertices += batch->lastTotalVertices;
		s_stats.drawCalls++;
	}

	void Renderer::Submit(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader) {
		shader->Bind();
		shader->SetMat4("viewProjection", s_data.viewProjectionMatrix);
		vertexArray->Bind();
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetSize(), GL_UNSIGNED_INT, 0);
		vertexArray->Unbind();
		s_stats.renderedVertices += vertexArray->GetVertexBuffer()->GetVertexCount();
		s_stats.drawCalls++;
	}

	RendererAPI Renderer::GetAPI() { return s_RendererAPI; }

	RendererStats Renderer::GetRenderStats()
	{
		return s_stats;
	}

}