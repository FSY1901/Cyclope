#include "Cyclope.h"
#include "Cyclope/EntryPoint.h"

using namespace Cyclope;

class App : public Application {

public:

	App(int width, int height, const char* title) : Cyclope::Application(width, height, title) {
		
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	return new App(0, 0, "");
}