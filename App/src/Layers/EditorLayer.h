#include "Cyclope.h"

#include "../SceneView/SceneViewCamera.h"

#include "../NativeScripting/DLLLoader.h"

namespace CyclopeEditor {

	class EditorLayer : public Layer {
	public:
		void OnAttach() override;

		void OnUpdate(float dt) override;

		void OnImGuiRender() override;

		void OnDetach() override;

	private:
		Shared<VertexArray> vert;
		Shared<Shader> sh;
		Shared<Texture2D> tex;
		SceneViewCamera svc;

		DLLLoader loader;

		Scene s;

	};

}