#include <iostream>

#include "Cyclope.h"
#include "Cyclope/EntryPoint.h"

#include "Layers/EditorLayer.h"

using namespace Cyclope;

class App : public Application{

public:

	App(int width, int height, const char* title) : Application(width, height, title) {
		PushLayer(new CyclopeEditor::EditorLayer());
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	return new App(800, 600, "Cyclope Engine");
}