#include "Cyclope.h"

#include "../SceneView/SceneViewCamera.h"
#include "../SceneView/Grid.h"

#include "../NativeScripting/DLLLoader.h"

#include "ContentBrowserPanel.h"

namespace CyclopeEditor {

	class EditorLayer : public Layer {
	public:
		void OnAttach() override;

		void OnUpdate(float dt) override;

		void OnImGuiRender() override;

		void OnDetach() override;

		void OnEvent(Event& e) override;
		bool OnKeyPressedEvent(KeyPressedEvent& e);

		static EditorLayer* GetEditorLayer() { return s_EditorLayer; }
		ImVec2 GetPanelSize() { return panelSize; }

	private:
#pragma region Toolbar
		Shared<Texture2D> playButtonTexture;
		Shared<Texture2D> stopButtonTexture;
#pragma endregion

#pragma region Billboards
		Shared<VertexArray> planeVA;//used for billboards, framebuffers, etc
		Shared<Shader> billboardShader;
		Shared<Texture2D> BillboardTex;
#pragma endregion

#pragma region Skybox
		Shared<CubeMapTexture> skybox;
		Shared<Shader> skyboxShader;
		Shared<VertexArray> skyboxVA;
#pragma endregion
		
#pragma region Cameras
		Camera* activeCamera;//TODO: should this be in Scene?

		SceneViewCamera svc;
		Entity cameraEntity;
#pragma endregion

#pragma region Scenes
		Shared<Scene> activeScene;
		Shared<Scene> editorScene;

		enum class SceneState {
			Edit,
			Play
		};

		SceneState sceneState = SceneState::Edit;

		void SerializeScene();
		void DeserializeScene();

		void OpenScene(const std::filesystem::path& path);
#pragma endregion

#pragma region Framebuffer
		Shared<Framebuffer> framebuffer;
		Shared<Framebuffer> framebuffer2; //used for post processing effects in the renderer
		Shared<VertexArray> fbVA;
		Shared<Shader> fbShader;
#pragma endregion

#pragma region Grid
		Grid grid;
		bool renderGrid = true;
#pragma endregion

#pragma region ProjectFunctions
		void NewProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();
#pragma endregion

#pragma region GUI
		Entity selectedEntity;

		int gizmoType = -1;

		ImVec2 panelSize;
		Vector2 viewportBounds[2];

		Shared<Shader> selectedObjectShader;

		void DrawViewportPanel();
		void DrawSceneHierarchyPanel();
		void DrawInspectorPanel();
		void DrawDebugPanel();
		void DrawToolbar();
		ContentBrowserPanel contentBrowser;


		GUIFunction DisplayComponent;

		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName) {
			if (!selectedEntity.HasComponent<T>())
			{
				if (ImGui::MenuItem(entryName.c_str()))
				{
					selectedEntity.AddComponent<T>();
					ImGui::CloseCurrentPopup();
				}
			}
		}

		void DisplayAddCustomComponentEntry(const std::string& entryName);

		void DisplayCustomComponent(const std::string& name);
#pragma endregion

#pragma region DLLLoader
		DLLLoader loader;
#pragma endregion

		static EditorLayer* s_EditorLayer;

	};

}