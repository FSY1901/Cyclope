extern Cyclope::Application* Cyclope::CreateApplication();

int main(int argc, char** argv) {

	auto app = Cyclope::CreateApplication();
	app->Run();
	delete app;

}