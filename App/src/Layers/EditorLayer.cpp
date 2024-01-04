#include "EditorLayer.h"

#include "../OBJLoader.h"

#include "Util.h"

namespace CyclopeEditor {

	static void DrawVec3Control(const std::string& label, glm::vec3& values)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, 80.0f);
		ImGui::AlignTextToFramePadding();
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		//ImGui::Dummy(ImVec2(0.0f, 0.0f));
		//ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "X");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 0, 0, 255));
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Y");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 255, 0, 255));
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopStyleColor();
		ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		ImGui::TextColored(ImVec4(0.2f, 0.2f, 1.0f, 1.0f), "Z");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 255, 255));
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopStyleColor();
		ImGui::NextColumn();
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity& entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = 16;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();

			if (open)
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}

				bool removeComponent = false;
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;

					ImGui::EndPopup();
				}
				uiFunction(component);
				ImGui::TreePop();
				if (removeComponent)
					entity.RemoveComponent<T>();
			}
		}
	}

	EditorLayer* EditorLayer::s_EditorLayer = nullptr;

	void EditorLayer::OnAttach() {
		s_EditorLayer = this;

		std::vector<float> verts;
		std::vector<unsigned int> ind;

		LoadOBJFile("./Resources/objs/cube.obj", verts, ind);
		//LoadOBJFile("./Resources/objs/cube.obj", m.vertices, m.indices);

		auto v = VertexBuffer::Create(&verts[0], verts.size(), BufferLayout::Standard());
		vert = VertexArray::Create(v, IndexBuffer::Create(&ind[0], ind.size()));

		sh = Shader::Create("./Resources/shaders/shader.glsl");
		tex = Texture2D::Create("./Resources/textures/earth.jpg");

		verts.clear();
		ind.clear();

		batch = MakeShared<Batch>();

		/*const int bound = 1250;
		for (int i = 0; i < bound; i++) {
			for (int j = 0; j < bound; j++) {
				Matrix4 mat = Matrix4(1.0f);
				mat = glm::translate(mat, Vector3((i-0.5f) - bound*0.5f, 0, (j-0.5f) - bound*0.5f));
				mat = glm::scale(mat, Vector3(0.3f, 0.3f, 0.3f));
				//sh->SetMat4("transform", mat);
				batch->AddToBatch(m, mat);
			}
		}

		batch->GenerateBatch();*/

		/*LoadOBJFile("./Resources/objs/cube.obj", verts, ind);

		auto v1 = VertexBuffer::Create(&verts[0], verts.size() * sizeof(float));
		v1->SetBufferLayout(BufferLayout::Standard());
		vert2 = VertexArray::Create(v1, IndexBuffer::Create(&ind[0], ind.size() * sizeof(unsigned int)));

		sh2 = Shader::Create("./Resources/shaders/shader.glsl");
		tex2 = Texture2D::Create("./Resources/textures/container.jpg");*/
		
		FramebufferSpecification fbs;
		fbs.width = 800;
		fbs.height = 600;
		fb = Framebuffer::Create(fbs);
		fb2 = Framebuffer::Create(fbs);
		panelSize = ImVec2(Application::GetInstance()->GetWindow()->GetWidth(),
							Application::GetInstance()->GetWindow()->GetHeight());

		float vertices[] = {
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,// bottom left
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,// top left 
		};
		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		auto v1 = VertexBuffer::Create(vertices, 20, BufferLayout::Standard());
		fbVA = VertexArray::Create(v1, IndexBuffer::Create(indices, 6));
		fbShader = Shader::Create("./Resources/shaders/framebuffer.glsl");

		RenderCommands::Enable(RenderingOperation::DepthTest);
		RenderCommands::Enable(RenderingOperation::Blending);
		RenderCommands::SetClearColor(0.1f, 0.1f, 0.1f);

		grid = Grid();

		activeScene = MakeShared<Scene>();

		loader.LoadDLL(componentRegistry, componentNamesList, nativeScriptRegistry, nativeScriptNamesList);
		auto hash = std::hash<std::string>{}(nativeScriptNamesList[1]);
		Entity e = activeScene->CreateEntity();
		activeScene->CreateEntity();
		e.AddComponent<NativeScriptComponent>();
		//auto f = nativeScriptRegistry.at(hash);
		//f(e);

		DisplayComponent = loader.Load();
	}

	void EditorLayer::OnUpdate(float dt) {
		CYCLOPE_PROFILE_FUNCTION();
		activeScene->Update(dt);

		svc.Update(dt);
		{
			CYCLOPE_PROFILE_SCOPE("Framebuffer Scope"); //Example Usage
			RenderCommands::Enable(RenderingOperation::DepthTest);
			if (fb->GetSpecification().width != panelSize.x ||
				fb->GetSpecification().height != panelSize.y) {
				fb->GetSpecification().width = panelSize.x;
				fb->GetSpecification().height = panelSize.y;
				fb2->GetSpecification().width = panelSize.x;
				fb2->GetSpecification().height = panelSize.y;
				RenderCommands::SetViewport(panelSize.x, panelSize.y);
				fb->Invalidate();
				fb2->Invalidate();
			}
			fb->Bind();
		}

		RenderCommands::Clear();
		
		Renderer::BeginScene(svc.GetCamera());

		/*tex->Bind();
		Matrix4 mat = Matrix4(1.0f);
		mat = glm::translate(mat, Vector3(0, 0, -5));
		mat = glm::scale(mat, Vector3(1.0f, 1.0f, 1.0f));
		sh->Bind();
		sh->SetMat4("transform", mat);
		Renderer::Submit(vert, sh);
		tex->Unbind();*/

		/*
		tex->Bind();
		Renderer::Submit(batch, sh);
		tex->Unbind();
		*/

		if(renderGrid)
			grid.Render(svc);

		Renderer::EndScene();

		fb->Unbind();
		RenderCommands::Clear();

		fb2->Bind();
		fbShader->Bind();
		//Draw
		fbVA->Bind();
		RenderCommands::Disable(RenderingOperation::DepthTest);
		//RenderCommands::Disable(RenderingOperation::CullFace);
		fb->BindTexture(fb->GetColorAttachment());
		fbShader->Bind();
		fbShader->SetFloat("iTime", Time::GetTime());
		Renderer::Submit(fbVA, fbShader);
		//Draw End
		fb2->Unbind();
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

		DrawScenePanel();
		DrawSceneHierarchyPanel();
		DrawInspectorPanel();
		DrawDebugPanel();

	}

	void EditorLayer::OnDetach() {
		
	}

	void EditorLayer::DrawScenePanel() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Scene");
		ImGui::PopStyleVar();
		panelSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)fb2->GetColorAttachment(),
			panelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		if (ImGui::IsWindowHovered())
			svc.sceneWindowHovered = true;
		else
			svc.sceneWindowHovered = false;

		ImGui::End();

	}

	void EditorLayer::DrawSceneHierarchyPanel() {

		ImGui::Begin("Scene Hierarchy");

		activeScene->ForEach([&](Entity& e) {
			bool node = ImGui::TreeNodeEx((e.Tag() + std::string("##") + std::to_string(e)).c_str(),
				ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);
			if (node) {
				ImGui::TreePop();
			}
			if (ImGui::IsItemClicked()) {
				selectedEntity = e;
			}
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Entity")) {
					activeScene->DestroyEntity(e);
					if (selectedEntity == e)
						selectedEntity = {};
				}
				ImGui::EndPopup();
			}
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			selectedEntity = {};

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
				activeScene->CreateEntity();

			ImGui::EndPopup();
		}

		ImGui::End();

	}

	void EditorLayer::DrawInspectorPanel() {

		ImGui::Begin("Inspector");
		if (selectedEntity) {
			if (selectedEntity.HasComponent<TagComponent>())
			{
				auto& tag = selectedEntity.Tag();

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
				if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			}

			ImGui::PushItemWidth(-1);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				DisplayAddComponentEntry<NativeScriptComponent>("NativeScript");
				DisplayAddComponentEntry<CameraComponent>("Camera");
				ImGui::TextColored(ImVec4{0.3f, 0.3f, 0.3f, 1.0f}, "Custom:");
				for (auto& name : componentNamesList) {
					DisplayAddCustomComponentEntry(name);
				}

				ImGui::EndPopup();
			}
			DrawComponent<TransformComponent>("Transform", selectedEntity, [](auto& component)
				{
					DrawVec3Control("Position", component.position);
					Vector3 rot = ToEulerAngles(component.rotation);
					DrawVec3Control("Rotation", rot);
					component.rotation = ToQuaternion(rot);
					DrawVec3Control("Scale", component.scale);
					ImGui::Columns(1);
				});
			
			DrawComponent<NativeScriptComponent>("NativeScript", selectedEntity, [&](auto& component) {
				ImGuiComboFlags flags = 0;
				int item_current_idx = 0;
				for (auto& s : nativeScriptNamesList) {
					if (component.scriptName == s)
						break;

					item_current_idx++;
				}
				const char* combo_preview_value = nativeScriptNamesList[item_current_idx].c_str();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo("##", combo_preview_value, flags))
				{
					for (int n = 0; n < nativeScriptNamesList.size(); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(nativeScriptNamesList[n].c_str(), is_selected)) {
							component.scriptName = nativeScriptNamesList[n];
							if (component.scriptName != "None") {
								auto func = nativeScriptRegistry.at(std::hash<std::string>{}(nativeScriptNamesList[n]));
								func(selectedEntity);
								selectedEntity.GetComponent<NativeScriptComponent>().instance = nullptr;
							}
							else {
								selectedEntity.GetComponent<NativeScriptComponent>().Unbind();
							}
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				});

			DrawComponent<CameraComponent>("Camera", selectedEntity, [](auto& component)
				{
					
				});


			ImGui::Dummy(ImVec2(0.0f, 0.0f));
			ImGui::Text("Custom Components:");

			for (auto& name : componentNamesList) {
				DisplayCustomComponent(name);
			}
		}
		ImGui::End();

	}

	void EditorLayer::DrawDebugPanel() {

		ImGui::Begin("Debug");
		ImGui::Text("FPS: ");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "%.1f FPS", ImGui::GetIO().Framerate);
		std::string drawCallText = "Draw Calls: ";
		drawCallText += std::to_string(Renderer::GetRenderStats().drawCalls);
		ImGui::Text(drawCallText.c_str());
		std::string numVertices = "Vertices: ";
		numVertices += std::to_string(Renderer::GetRenderStats().renderedVertices);
		ImGui::Text(numVertices.c_str());
		ImGui::Checkbox("Grid", &renderGrid);
		ImGui::End();

	}

	void EditorLayer::DisplayAddCustomComponentEntry(const std::string& entryName) {
		auto func = componentRegistry.at(std::hash<std::string>{}(entryName));
		if (!func.HasComponent(selectedEntity)) {
			if (ImGui::MenuItem(entryName.c_str()))
			{
				func.AddComponent(selectedEntity);
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void EditorLayer::DisplayCustomComponent(const std::string& name) {
		auto func = componentRegistry.at(std::hash<std::string>{}(name));
		if (func.HasComponent(selectedEntity)) {
			const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			const char* _name = name.c_str();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = 16;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)_name, treeNodeFlags, _name);
			ImGui::PopStyleVar();

			if (open)
			{
				ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
				if (ImGui::Button("-", ImVec2{ lineHeight, lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
				bool removeComponent = false;
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove component"))
						removeComponent = true;

					ImGui::EndPopup();
				}

				DisplayComponent(ImGui::GetCurrentContext(), selectedEntity, _name);

				ImGui::TreePop();
				if (removeComponent)
					func.RemoveComponent(selectedEntity);
			}
		}
	}

}