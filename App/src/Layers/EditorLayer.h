#include "Cyclope.h"

#include "../SceneView/SceneViewCamera.h"

using namespace Cyclope;

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

	};

}