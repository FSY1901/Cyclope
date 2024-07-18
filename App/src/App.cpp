#include <iostream>

#include "Cyclope.h"
#include "Cyclope/EntryPoint.h"

#include "Layers/EditorLayer.h"
#include "Layers/ScriptLoaderLayer.h"

using namespace Cyclope;

class App : public Application{

public:

	App(const WindowSpecification& spec) : Application(spec) {
		PushLayer(new CyclopeEditor::EditorLayer());
		PushLayer(new CyclopeEditor::ScriptLoaderLayer());
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	WindowSpecification spec;
	spec.width = 800;
	spec.height = 600;
	spec.title = "Cyclope Editor";
	spec.maximized = true;
	return new App(spec);
}