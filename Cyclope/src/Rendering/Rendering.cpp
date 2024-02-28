#include "Cpch.h"
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
			vertices.push_back(Vertex{ pos, vert.normal, vert.uv });
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

	void Renderer::BeginScene(Shared<Scene> scene, const Camera& camera) {
		s_data.viewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_data.scene = scene;
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
#pragma region Lighting
		//TODO: Add Shadertypes
		auto directionalView = s_data.scene->View<DirectionalLightComponent>();
		if (!directionalView.empty()) {
			auto entity = directionalView[0];
			Entity e = Entity(entity, s_data.scene.get());
			auto light = e.GetComponent<DirectionalLightComponent>();
			shader->SetVec3("directionalLight.direction", e.GetComponent<TransformComponent>().rotation * Vector3(0.0f, 0.0f, -1.0f));
			shader->SetVec3("directionalLight.diffuse", light.diffuse);
			shader->SetVec3("directionalLight.ambient", light.ambient);
		}
		else {
			shader->SetVec3("directionalLight.direction", Vector3());
			shader->SetVec3("directionalLight.diffuse", Vector3());
			shader->SetVec3("directionalLight.ambient", Vector3());
		}

		//TODO: find 4 closest lights(like Unity) or do another aproach
		int i = 0;
		s_data.scene->View<PointLightComponent>().each([&](auto entity, PointLightComponent& light) {
			if (i < 4) {
				Entity e = Entity(entity, s_data.scene.get());
				shader->SetFloat("pointLights[" + std::to_string(i) + "].intensity", light.intensity);
				shader->SetFloat("pointLights[" + std::to_string(i) + "].radius", light.radius);
				shader->SetFloat("pointLights[" + std::to_string(i) + "].cutOff", light.cutOff);
				shader->SetVec3("pointLights[" + std::to_string(i) + "].diffuse", light.diffuse);
				shader->SetVec3("pointLights[" + std::to_string(i) + "].ambient", light.ambient);
				shader->SetVec3("pointLights[" + std::to_string(i) + "].position", e.GetComponent<TransformComponent>().position);
			}
			i++;
			});
		for (; i < 4; i++) {
			shader->SetFloat("pointLights[" + std::to_string(i) + "].intensity", 0.0f);
			shader->SetFloat("pointLights[" + std::to_string(i) + "].radius", 0.0f);
			shader->SetFloat("pointLights[" + std::to_string(i) + "].cutOff", 0.0f);
			shader->SetVec3("pointLights[" + std::to_string(i) + "].diffuse", Vector3());
			shader->SetVec3("pointLights[" + std::to_string(i) + "].ambient", Vector3());
			shader->SetVec3("pointLights[" + std::to_string(i) + "].position", Vector3());
			shader->SetVec3("pointLights[" + std::to_string(i) + "].direction", Vector3());
		}

		
		int j = 0;
		s_data.scene->View<SpotLightComponent>().each([&](auto entity, SpotLightComponent& light) {
			if (j < 2) {
				Entity e = Entity(entity, s_data.scene.get());
				shader->SetVec3("spotLights[" + std::to_string(j) + "].diffuse", light.diffuse);
				shader->SetVec3("spotLights[" + std::to_string(j) + "].ambient", light.ambient);
				shader->SetVec3("spotLights[" + std::to_string(j) + "].position", e.GetComponent<TransformComponent>().position);
				shader->SetVec3("spotLights[" + std::to_string(j) + "].direction", e.GetComponent<TransformComponent>().rotation * Vector3(0.0f, 0.0f, -1.0f));
				shader->SetFloat("spotLights[" + std::to_string(j) + "].quadratic", light.quadratic);
				shader->SetFloat("spotLights[" + std::to_string(j) + "].intensity", light.intensity);
				shader->SetFloat("spotLights[" + std::to_string(j) + "].cutOff", glm::cos(glm::radians(light.cutOff)));
				shader->SetFloat("spotLights[" + std::to_string(j) + "].outerCutOff", glm::cos(glm::radians(light.outerCutOff)));
			}
			j++;
			});
		for (; j < 2; j++) {
			shader->SetFloat("spotLights[" + std::to_string(j) + "].intensity", 0.0f);
			shader->SetFloat("spotLights[" + std::to_string(j) + "].cutOff", 0.0f);
			shader->SetFloat("spotLights[" + std::to_string(j) + "].outerCutOff", 0.0f);
			shader->SetFloat("spotLights[" + std::to_string(j) + "].linear", 0.0f);
			shader->SetFloat("spotLights[" + std::to_string(j) + "].quadratic", 0.0f);
			shader->SetVec3("spotLights[" + std::to_string(j) + "].diffuse", Vector3());
			shader->SetVec3("spotLights[" + std::to_string(j) + "].ambient", Vector3());
			shader->SetVec3("spotLights[" + std::to_string(j) + "].position", Vector3());
			shader->SetVec3("spotLights[" + std::to_string(i) + "].direction", Vector3());
		}
		
#pragma endregion
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