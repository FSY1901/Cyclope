#include "EditorLayer.h"

#include "../OBJLoader.h"

#include "Util.h"

#include "Game/SceneSerializer.h"

#include "Platform/Windows/FileDialog.h"

#include "ImGuizmo.h"

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

		sh = Shader::Create("./Resources/shaders/basic.glsl");//shader.glsl
		tex = Texture2D::Create("./Resources/textures/container2.png");
		tex2 = Texture2D::Create("./Resources/textures/specular.png");

		verts.clear();
		ind.clear();

		/*batch = MakeShared<Batch>();

		const int bound = 1250;
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

		LoadOBJFile("./Resources/objs/plane.obj", verts, ind);
		auto f = VertexBuffer::Create(&verts[0], verts.size(), BufferLayout::Standard());
		//v1->SetBufferLayout(BufferLayout::Standard());
		vert2 = VertexArray::Create(f, IndexBuffer::Create(&ind[0], ind.size()));

		sh2 = Shader::Create("./Resources/shaders/s.glsl");
		BillboardTex = Texture2D::Create("./Resources/textures/Billboards/DirectionalLight.png");
		
		FramebufferSpecification fbs;
		fbs.width = 800;
		fbs.height = 600;
		fb2 = Framebuffer::Create(fbs);
		fbs.samples = 4;//Set to 4 for Anti Aliasing
		fb = Framebuffer::Create(fbs);
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
		auto v1 = VertexBuffer::Create(vertices, 20, BufferLayout{{{ShaderDataType::Float3}, {ShaderDataType::Float2}}});
		fbVA = VertexArray::Create(v1, IndexBuffer::Create(indices, 6));
		fbShader = Shader::Create("./Resources/shaders/framebuffer.glsl");

		RenderCommands::Enable(RenderingOperation::DepthTest);
		RenderCommands::Enable(RenderingOperation::Blending);
		RenderCommands::SetClearColor(0.1f, 0.1f, 0.1f);

		grid = Grid();

		activeScene = MakeShared<Scene>();

		loader.LoadDLL(componentRegistry(), componentNamesList(), nativeScriptRegistry(), nativeScriptNamesList());
#if 0
		activeScene->CreateEntity("B");
		Entity e = activeScene->CreateEntity("A");
		e.AddComponent<NativeScriptComponent>();
		e.GetComponent<TransformComponent>().position = Vector3(6.9f, 0.0f, .12f);
#endif
		//auto f = nativeScriptRegistry.at(hash);
		//f(e);
		//auto hash = std::hash<std::string>{}(nativeScriptNamesList[1]);
		DisplayComponent = loader.Load();
		model = Model("./Resources/models/backpack/backpack.obj");
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
		RenderCommands::Enable(RenderingOperation::DepthTest);

		Renderer::BeginScene(activeScene, svc.GetCamera());

		{
			CYCLOPE_PROFILE_SCOPE("Render Scope");
			activeScene->ForEach([&](Entity e) {
				if (e.HasComponent<ModelRendererComponent>()) {
					/*
					Matrix4& transform = e.GetComponent<TransformComponent>().GetTransform();
					sh->Bind();
					sh->SetMat4("transform", transform);
					sh->SetMat3("normalMatrix", Matrix3(glm::transpose(glm::inverse(transform))));
					sh->SetVec3("viewPos", svc.transform.position);
					sh->SetVec3("material.diffuse", Vector3(1.0f, 1.0f, 1.0f));
					sh->SetVec3("material.specular", Vector3(0.5f));
					sh->SetFloat("material.shininess", 32.0f);
					sh->SetInt("material.diffuseMap", 0);
					sh->SetInt("material.specularMap", 1);
					tex->Bind();
					tex2->Bind(1);
					Renderer::Submit(vert, sh);
					tex->Unbind();
					tex2->Unbind();
					*/
					auto& modelComponent = e.GetComponent<ModelRendererComponent>();
					auto& shader = modelComponent.shader;
					Matrix4& transform = e.GetComponent<TransformComponent>().GetTransform();
					if (shader.get() && shader->GetID()) {
						shader->Bind();
						shader->SetMat4("transform", transform);
						//Dependent on the shader
						shader->SetMat3("normalMatrix", Matrix3(glm::transpose(glm::inverse(transform))));
						shader->SetVec3("viewPos", svc.transform.position);
						shader->SetVec3("material.diffuse", modelComponent.diffuse);
						shader->SetVec3("material.specular", modelComponent.specular);
						shader->SetFloat("material.shininess", modelComponent.shininess);
						modelComponent.model.Draw(shader);
					}
					//model.Draw(sh);
				}
				});

			activeScene->ForEach([&](Entity e) {
				if (e.HasComponent<DirectionalLightComponent>()) {
					auto& tc = e.GetComponent<TransformComponent>();
					sh2->Bind();
					sh2->SetVec3("pos", tc.position);
					Vector3 front = glm::normalize(svc.transform.rotation * Vector3(0.0f, 0.0f, -1.0f));
					Vector3 right = glm::normalize(glm::cross(front, Vector3(0.0f, 1.0f, 0.0f)));
					Vector3 up = glm::normalize(glm::cross(right, front));
					sh2->SetVec3("camRight", right);
					sh2->SetVec3("camUp", up);
					sh2->SetVec3("diffuse", e.GetComponent<DirectionalLightComponent>().diffuse);
					//tex->Bind();
					//tex2->Bind(1);
					BillboardTex->Bind();
					Renderer::Submit(vert2, sh2);
					BillboardTex->Unbind();
				}
				});

			activeScene->ForEach([&](Entity e) {
				if (e.HasComponent<PointLightComponent>()) {
					auto& tc = e.GetComponent<TransformComponent>();
					sh2->Bind();
					sh2->SetVec3("pos", tc.position);
					Vector3 front = glm::normalize(svc.transform.rotation * Vector3(0.0f, 0.0f, -1.0f));
					Vector3 right = glm::normalize(glm::cross(front, Vector3(0.0f, 1.0f, 0.0f)));
					Vector3 up = glm::normalize(glm::cross(right, front));
					sh2->SetVec3("camRight", right);
					sh2->SetVec3("camUp", up);
					sh2->SetVec3("diffuse", e.GetComponent<PointLightComponent>().diffuse);
					//tex->Bind();
					//tex2->Bind(1);
					BillboardTex->Bind();
					Renderer::Submit(vert2, sh2);
					BillboardTex->Unbind();
				}
				});

			activeScene->ForEach([&](Entity e) {
				if (e.HasComponent<SpotLightComponent>()) {
					auto& tc = e.GetComponent<TransformComponent>();
					sh2->Bind();
					sh2->SetVec3("pos", tc.position);
					Vector3 front = glm::normalize(svc.transform.rotation * Vector3(0.0f, 0.0f, -1.0f));
					Vector3 right = glm::normalize(glm::cross(front, Vector3(0.0f, 1.0f, 0.0f)));
					Vector3 up = glm::normalize(glm::cross(right, front));
					sh2->SetVec3("camRight", right);
					sh2->SetVec3("camUp", up);
					sh2->SetVec3("diffuse", e.GetComponent<SpotLightComponent>().diffuse);
					//tex->Bind();
					//tex2->Bind(1);
					BillboardTex->Bind();
					Renderer::Submit(vert2, sh2);
					BillboardTex->Unbind();
				}
				});
		}
		
		/*tex->Bind();
		Renderer::Submit(batch, sh);
		tex->Unbind();*/

		{
			CYCLOPE_PROFILE_SCOPE("Grid Scope");
			if (renderGrid)
				grid.Render(svc);
		}

		Renderer::EndScene();

		{
			CYCLOPE_PROFILE_SCOPE("Blit Scope");
			fb->Unbind();
			RenderCommands::Clear();
			fb->BlitTo(fb2);
		}

		//fb2->Bind();
		{
			CYCLOPE_PROFILE_SCOPE("Redraw Scope");
			fbShader->Bind();
			//Draw
			fbVA->Bind();
			RenderCommands::Disable(RenderingOperation::DepthTest);
			//RenderCommands::Disable(RenderingOperation::CullFace);
			fb2->BindTexture(fb2->GetColorAttachment());
			fbShader->Bind();
			//fbShader->SetFloat("iTime", Time::GetTime());
			Renderer::Submit(fbVA, fbShader);
			//Draw End
			fb2->BindTexture(0);
			fb2->Unbind();
		}
		RenderCommands::Clear();

	}

	void EditorLayer::OnImGuiRender() {
		CYCLOPE_PROFILE_FUNCTION();
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
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					activeScene = MakeShared<Scene>();
					selectedEntity = {};
				}
				if (ImGui::MenuItem("Save...", "Ctrl+S")) {
					SerializeScene();
				}
				if (ImGui::MenuItem("Open...", "Ctrl+L")) {
					DeserializeScene();
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

	void EditorLayer::OnEvent(Event& e) {
		activeScene->OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e) {
		if (e.IsRepeat())
			return false;

		bool control = Input::KeyDown(Key::LEFT_CONTROL) || Input::KeyDown (Key::RIGHT_CONTROL);
		bool svcControlled = svc.IsControlling();
		//bool shift = Input::KeyDown(Key::LEFT_SHIFT) || Input::KeyDown(Key::RIGHT_SHIFT);
		switch (e.GetKeyCode()) {
		case Key::S:
			if (control)
				SerializeScene();
			else if(!svcControlled)
				m_GizmoType = ImGuizmo::SCALE;
			break;
		case Key::O:
			if(control)
				DeserializeScene();
			break;
		case Key::N:
			if (control) {
				activeScene = MakeShared<Scene>();
				selectedEntity = {};
			}
			break;
		case Key::G:
			if (!svcControlled)
				m_GizmoType = ImGuizmo::TRANSLATE;
			break;
		case Key::R:
			if (!svcControlled)
				m_GizmoType = ImGuizmo::ROTATE;
			break;
		case Key::TAB:
			if (!svcControlled)
				m_GizmoType = -1;
			break;
		}

		if (e.GetKeyCode() == Key::P) {
			activeScene->m_playing = !activeScene->m_playing;
		}

		return true;
	}

	void EditorLayer::SerializeScene() {
		auto path = FileDialog::SaveFile("Cyclope Scene (*.cyclope)\0*.cyclope\0");
		if (!path.empty()) {
			SceneSerializer serializer(activeScene);
			serializer.Serialize(path);
		}
	}

	void EditorLayer::DeserializeScene() {
		auto path = FileDialog::OpenFile("Cyclope Scene (*.cyclope)\0*.cyclope\0");
		if (!path.empty()) {
			activeScene = MakeShared<Scene>();
			selectedEntity = {};
			SceneSerializer serializer(activeScene);
			serializer.Deserialize(path);
		}
	}

	void EditorLayer::DrawScenePanel() {

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Scene");
		ImGui::PopStyleVar();
		panelSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)fb2->GetColorAttachment(),
			panelSize, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		
		//Gizmos
		if (selectedEntity && m_GizmoType != -1 && !svc.IsControlling()) {
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			auto& tc = selectedEntity.Transform();
			Matrix4 transform = tc.GetTransform();

			//Snap
			bool snap = Input::KeyDown(Key::LEFT_SHIFT);
			float snapAmount = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapAmount = 10.0f;
			float snapValues[3] = { snapAmount, snapAmount, snapAmount };

			ImGuizmo::Manipulate(glm::value_ptr(svc.GetCamera().GetViewMatrix()), glm::value_ptr(svc.GetCamera().GetProjectionMatrix()), 
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);
			
			if (ImGuizmo::IsUsing()) {
				
				Vector3 position, rotation, scale;
				DecomposeTransform(transform, position, rotation, scale);
				tc.position = position;
				Vector3 deltaRotation = glm::degrees(rotation) - ToEulerAngles(tc.rotation);
				Vector3 rot = ToEulerAngles(tc.rotation) + deltaRotation;
				tc.rotation = ToQuaternion(rot);
				tc.scale = scale;

			}

		}

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
			else if (ImGui::BeginMenu("Create Light")) {
				if (ImGui::MenuItem("Directional Light")) {
					auto& e = activeScene->CreateEntity("Directional Light");
					e.AddComponent<DirectionalLightComponent>();
				}
				else if (ImGui::MenuItem("Point Light")) {
					auto& e = activeScene->CreateEntity("Point Light");
					e.AddComponent<PointLightComponent>();
				}
				else if (ImGui::MenuItem("Spot Light")) {
					auto& e = activeScene->CreateEntity("Spot Light");
					e.AddComponent<SpotLightComponent>();
				}
				ImGui::EndMenu();
			}

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
				DisplayAddComponentEntry<ModelRendererComponent>("Model Renderer");
				DisplayAddComponentEntry<DirectionalLightComponent>("Directional Light");
				DisplayAddComponentEntry<PointLightComponent>("Point Light");
				DisplayAddComponentEntry<SpotLightComponent>("Spot Light");
				ImGui::TextColored(ImVec4{0.3f, 0.3f, 0.3f, 1.0f}, "Custom:");
				for (auto& name : componentNamesList()) {
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
				for (auto& s : nativeScriptNamesList()) {
					if (component.scriptName == s)
						break;

					item_current_idx++;
				}
				const char* combo_preview_value = nativeScriptNamesList()[item_current_idx].c_str();
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				if (ImGui::BeginCombo("##", combo_preview_value, flags))
				{
					for (int n = 0; n < nativeScriptNamesList().size(); n++)
					{
						const bool is_selected = (item_current_idx == n);
						if (ImGui::Selectable(nativeScriptNamesList()[n].c_str(), is_selected)) {
							component.scriptName = nativeScriptNamesList()[n];
							if (component.scriptName != "None") {
								auto func = nativeScriptRegistry().at(std::hash<std::string>{}(nativeScriptNamesList()[n]));
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

			DrawComponent<ModelRendererComponent>("Model Renderer", selectedEntity, [](auto& component)
				{
					ImGui::Text("Material Properties:");
					if (ImGui::Button("Change Shader")) {
						auto& path = FileDialog::GetFilePath("Shader File (*.glsl)\0*.glsl\0");
						component.shader = Shader::Create(path);
					}
					float col[3] = { component.diffuse.x, component.diffuse.y, component.diffuse.z };
					ImGui::PushItemWidth(170);
					ImGui::ColorEdit3("Diffuse", col);
					component.diffuse = Vector3(col[0], col[1], col[2]);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(170);
					float* val = &component.specular.x;
					ImGui::DragFloat("Specular", val, 0.005f, 0.0f, 1.0f);
					component.specular = Vector3(*val, *val, *val);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(170);
					ImGui::InputFloat("Shininess", &component.shininess);
					ImGui::PopItemWidth();
					ImGui::Text("Model:");
					if (ImGui::Button("Change Model")) {
						auto& path = FileDialog::GetFilePath("obj File (*.obj)\0*.obj\0");
						component.model = Model(path);
					}

				});

			DrawComponent<CameraComponent>("Camera", selectedEntity, [](auto& component)
				{

				});

			DrawComponent<DirectionalLightComponent>("Directional Light", selectedEntity, [](auto& component)
				{
					float diffuse[3] = { component.diffuse.x, component.diffuse.y, component.diffuse.z };
					ImGui::PushItemWidth(170);
					ImGui::ColorEdit3("Diffuse", diffuse);
					ImGui::PopItemWidth();
					component.diffuse.x = diffuse[0];
					component.diffuse.y = diffuse[1];
					component.diffuse.z = diffuse[2];
					float ambient[3] = { component.ambient.x, component.ambient.y, component.ambient.z };
					ImGui::PushItemWidth(170);
					ImGui::ColorEdit3("Ambient", ambient);
					ImGui::PopItemWidth();
					component.ambient.x = ambient[0];
					component.ambient.y = ambient[1];
					component.ambient.z = ambient[2];
					ImGui::DragFloat("Ambient Value", &component.ambientValue, 0.01f, 0.0f, 1.0f);
					if (ImGui::Button("Set Ambient auto")) {
						component.ambient.x = component.ambientValue * diffuse[0];
						component.ambient.y = component.ambientValue * diffuse[1];
						component.ambient.z = component.ambientValue * diffuse[2];
					}
				});

			DrawComponent<PointLightComponent>("PointLight", selectedEntity, [](auto& component)
				{
					float diffuse[3] = { component.diffuse.x, component.diffuse.y, component.diffuse.z };
					ImGui::PushItemWidth(170);
					ImGui::ColorEdit3("Diffuse", diffuse);
					ImGui::PopItemWidth();
					component.diffuse.x = diffuse[0];
					component.diffuse.y = diffuse[1];
					component.diffuse.z = diffuse[2];
					float ambient[3] = { component.ambient.x, component.ambient.y, component.ambient.z };
					ImGui::PushItemWidth(170);
					ImGui::ColorEdit3("Ambient", ambient);
					ImGui::PopItemWidth();
					component.ambient.x = ambient[0];
					component.ambient.y = ambient[1];
					component.ambient.z = ambient[2];
					ImGui::DragFloat("Ambient Value", &component.ambientValue, 0.01f, 0.0f, 1.0f);
					if (ImGui::Button("Set Ambient auto")) {
						component.ambient.x = component.ambientValue * diffuse[0];
						component.ambient.y = component.ambientValue * diffuse[1];
						component.ambient.z = component.ambientValue * diffuse[2];
					}
					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Radius", &component.radius, 0.1f);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Intensity", &component.intensity, 0.1f);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Cut Off", &component.cutOff, 0.0001f);
					ImGui::PopItemWidth();
				});

			DrawComponent<SpotLightComponent>("Spot Light", selectedEntity, [](auto& component)
				{
					float diffuse[3] = { component.diffuse.x, component.diffuse.y, component.diffuse.z };
					ImGui::PushItemWidth(170);
					ImGui::ColorEdit3("Diffuse", diffuse);
					ImGui::PopItemWidth();
					component.diffuse.x = diffuse[0];
					component.diffuse.y = diffuse[1];
					component.diffuse.z = diffuse[2];
					float ambient[3] = { component.ambient.x, component.ambient.y, component.ambient.z };
					ImGui::PushItemWidth(170);
					ImGui::ColorEdit3("Ambient", ambient);
					ImGui::PopItemWidth();
					component.ambient.x = ambient[0];
					component.ambient.y = ambient[1];
					component.ambient.z = ambient[2];
					ImGui::DragFloat("Ambient Value", &component.ambientValue, 0.01f, 0.0f, 1.0f);
					if (ImGui::Button("Set Ambient auto")) {
						component.ambient.x = component.ambientValue * diffuse[0];
						component.ambient.y = component.ambientValue * diffuse[1];
						component.ambient.z = component.ambientValue * diffuse[2];
					}

					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Intensity", &component.intensity, 0.1f);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Cut Off", &component.cutOff, 0.1f);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Outer Cut Off", &component.outerCutOff, 0.1f);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Linear Constant", &component.linear, 0.01f);
					ImGui::PopItemWidth();
					ImGui::PushItemWidth(60.0f);
					ImGui::DragFloat("Quadratic Constant", &component.quadratic, 0.01f);
					ImGui::PopItemWidth();
				});

			ImGui::Dummy(ImVec2(0.0f, 0.0f));
			ImGui::Text("Custom Components:");

			for (auto& name : componentNamesList()) {
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
		auto func = componentRegistry().at(std::hash<std::string>{}(entryName));
		if (!func.HasComponent(selectedEntity)) {
			if (ImGui::MenuItem(entryName.c_str()))
			{
				func.AddComponent(selectedEntity);
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void EditorLayer::DisplayCustomComponent(const std::string& name) {
		auto func = componentRegistry().at(std::hash<std::string>{}(name));
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