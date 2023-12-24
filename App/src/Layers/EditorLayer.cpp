#include "EditorLayer.h"

#include "../OBJLoader.h"

#include "Util.h"

namespace CyclopeEditor {

	EditorLayer* EditorLayer::s_EditorLayer = nullptr;

	void EditorLayer::OnAttach() {

		s_EditorLayer = this;

		std::vector<float> verts;
		std::vector<unsigned int> ind;

		LoadOBJFile("./Resources/objs/sphere.obj", verts, ind);

		auto v = VertexBuffer::Create(&verts[0], verts.size() * sizeof(float));
		v->SetBufferLayout(BufferLayout::Standard());
		vert = VertexArray::Create(v, IndexBuffer::Create(&ind[0], ind.size() * sizeof(unsigned int)));

		sh = Shader::Create("./Resources/shaders/shader.glsl");
		tex = Texture2D::Create("./Resources/textures/earth.jpg");

		verts.clear();
		ind.clear();

		LoadOBJFile("./Resources/objs/y.obj", verts, ind);

		auto v1 = VertexBuffer::Create(&verts[0], verts.size() * sizeof(float));
		v1->SetBufferLayout(BufferLayout::Standard());
		vert2 = VertexArray::Create(v1, IndexBuffer::Create(&ind[0], ind.size() * sizeof(unsigned int)));

		sh2 = Shader::Create("./Resources/shaders/shader.glsl");
		tex2 = Texture2D::Create("./Resources/textures/container.jpg");
		
		FramebufferSpecification fbs;
		fbs.width = 800;
		fbs.height = 600;
		fb = Framebuffer::Create(fbs);
		panelSize = ImVec2(Application::GetInstance()->GetWindow()->GetWidth(),
							Application::GetInstance()->GetWindow()->GetHeight());

		RenderCommands::SetClearColor(0.1f, 0.1f, 0.1f);

		grid = Grid();

		loader.LoadDLL(componentRegistry, nativeScriptRegistry);
		Entity e = activeScene.CreateEntity();
		e.AddComponent<NativeScriptComponent>();
		auto f = nativeScriptRegistry.at(10765104205153683754);
		f(e);
	}

	void EditorLayer::OnUpdate(float dt) {
		activeScene.Update(dt);

		svc.Update(dt);

		if (fb->GetSpecification().width != panelSize.x ||
			fb->GetSpecification().height != panelSize.y) {
			fb->GetSpecification().width = panelSize.x;
			fb->GetSpecification().height = panelSize.y;
			RenderCommands::SetViewport(panelSize.x, panelSize.y);
			fb->Invalidate();
		}
		fb->Bind();

		RenderCommands::Clear();
		
		Renderer::BeginScene(svc.GetCamera());
		tex->Bind();
		Matrix4 mat = Matrix4(1.0f);
		mat = glm::translate(mat, Vector3(0, 0, -5));
		mat = glm::scale(mat, Vector3(1.0f, 1.0f, 1.0f));
		sh->Bind();
		sh->SetMat4("transform", mat);
		Renderer::Submit(vert, sh);
		tex->Unbind();
		
		tex2->Bind();
		Matrix4 mat1 = Matrix4(1.0f);
		mat1 = glm::translate(mat1, Vector3(2, 0, -5));
		mat1 = glm::scale(mat1, Vector3(0.01f, 0.01f, 0.01f));
		sh2->Bind();
		sh2->SetMat4("transform", mat1);
		Renderer::Submit(vert2, sh2);

		if(renderGrid)
			grid.Render(svc);

		Renderer::EndScene();

		fb->Unbind();
		RenderCommands::Clear();

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

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("Project")) {
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
					
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Scene");
		ImGui::PopStyleVar();
		panelSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)fb->GetColorAttachment(),
			panelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::End();

		ImGui::Begin("Debug");
		ImGui::Text("FPS: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);
		ImGui::Checkbox("Grid", &renderGrid);
		ImGui::End();

	}

	void EditorLayer::OnDetach() {
		loader.FreeDLL();
	}

}