#include "Cyclope.h"

#include "../SceneView/SceneViewCamera.h"
#include "../SceneView/Grid.h"

#include "../NativeScripting/DLLLoader.h"

namespace CyclopeEditor {

	class EditorLayer : public Layer {
	public:
		void OnAttach() override;

		void OnUpdate(float dt) override;

		void OnImGuiRender() override;

		void OnDetach() override;

		static EditorLayer* GetEditorLayer() { return s_EditorLayer; }
		ImVec2 GetPanelSize() { return panelSize; }

	private:

		Shared<VertexArray> vert;
		Shared<VertexArray> vert2;
		Shared<Shader> sh;
		Shared<Shader> sh2;
		Shared<Texture2D> tex;
		Shared<Texture2D> tex2;
		SceneViewCamera svc;

		Mesh m;
		Shared<Batch> batch;

		Shared<Framebuffer> fb;
		Shared<Framebuffer> fb2; //used for post processing effects in the renderer
		Shared<VertexArray> fbVA;
		Shared<Shader> fbShader;
		ImVec2 panelSize;

		Grid grid;
		bool renderGrid = true;

		DLLLoader loader;

		Shared<Scene> activeScene;
		Entity selectedEntity;

		void DrawScenePanel();
		void DrawSceneHierarchyPanel();
		void DrawInspectorPanel();
		void DrawDebugPanel();

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

		static EditorLayer* s_EditorLayer;

	};

}