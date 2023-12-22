#include "EditorLayer.h"

#include "../OBJLoader.h"

#include "Util.h"

namespace CyclopeEditor {

	void EditorLayer::OnAttach() {

		float vertices[] = {
				 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
				-0.5f,  0.5f, 0.0f,  0.0f, 1.0f // top left 
		};
		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};

		std::vector<float> verts;
		std::vector<unsigned int> ind;

		LoadOBJFile("./Resources/objs/y.obj", verts, ind);

		sh = Shader::Create("./Resources/shader.glsl");
		tex = Texture2D::Create("./Resources/container.jpg");
		auto v = VertexBuffer::Create(&verts[0], verts.size() * sizeof(float));
		//auto v = VertexBuffer::Create(vertices, sizeof(vertices));
		v->SetBufferLayout(BufferLayout::Standard());
		vert = VertexArray::Create(v, IndexBuffer::Create(&ind[0], ind.size() * sizeof(unsigned int)));
		//vert = VertexArray::Create(v, IndexBuffer::Create(indices, sizeof(indices)));
		RenderCommands::SetClearColor(0.2f, 0.3f, 0.3f);
		loader.LoadDLL(componentRegistry, nativeScriptRegistry);
		Entity e = s.CreateEntity();
		e.AddComponent<NativeScriptComponent>();
		auto f = nativeScriptRegistry.at(10765104205153683754);
		f(e);
	}

	void EditorLayer::OnUpdate(float dt) {

		RenderCommands::Clear();
		svc.Update(dt);
		Renderer::BeginScene(svc.GetCamera());
		tex->Bind();
		Matrix4 mat = Matrix4(1.0f);
		mat = glm::translate(mat, Vector3(0, -1, -5));
		mat = glm::scale(mat, Vector3(0.01f, 0.01f, 0.01f));
		//mat = glm::rotate(mat, (float)Time::GetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//mat = glm::rotate(mat, (float)Time::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		sh->SetMat4("transform", mat);
		Renderer::Submit(vert, sh);
		Renderer::EndScene();

		if (Input::KeyPressed(Key::Q)) {
			std::cout << "Pressed";
		}

		s.Update(dt);

	}

	void EditorLayer::OnImGuiRender() {

		ImGuiWindowFlags winFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.0f);
		ImGui::Begin("Win", NULL, winFlags);
		ImGui::PopStyleVar(3);
		ImGuiID id = ImGui::GetID("WinDockspace");
		ImGui::DockSpace(id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();

		ImGui::Begin("ImGui Window");
		ImGui::Text("Test Text");
		ImGui::End();

	}

	void EditorLayer::OnDetach() {
		loader.FreeDLL();
	}

}