#include "Cyclope.h"

#include "../SceneView/SceneViewCamera.h"
#include "Scripting.h"

using namespace Cyclope;

//Loading a dll:

#include "windows.h"

typedef void(*FUNC)(ComponentRegistry& registry);

class DLLLoader {
public:
	void LoadDLL(ComponentRegistry& registry);

	void FreeDLL() {
		FreeLibrary(hDLL);
		std::cout << "Freed DLL" << std::endl;
	}

private:
	HINSTANCE hDLL;
};

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

	};

}