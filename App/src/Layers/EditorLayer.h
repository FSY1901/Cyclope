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

		Shared<Framebuffer> fb;
		ImVec2 panelSize;

		Grid grid;

		DLLLoader loader;

		Scene s;

		static EditorLayer* s_EditorLayer;

	};

}