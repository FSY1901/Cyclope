#include "Cyclope.h"
#include "Cyclope/EntryPoint.h"

using namespace Cyclope;

class App : public Application {

public:

	App(const WindowSpecification& spec) : Cyclope::Application(spec) {
		
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	WindowSpecification spec;
	spec.width = 0;
	spec.height = 0;
	spec.title = "";
	return new App(spec);
}