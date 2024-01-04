extern Cyclope::Application* Cyclope::CreateApplication();

int main(int argc, char** argv) {

	CYCLOPE_BEGIN_PROFILE_SESSION("Startup", "Startup-Profile.json");
	auto app = Cyclope::CreateApplication();
	CYCLOPE_END_PROFILE_SESSION();
	CYCLOPE_BEGIN_PROFILE_SESSION("Runtime", "Runtime-Profile.json");
	app->Run();
	CYCLOPE_END_PROFILE_SESSION();
	CYCLOPE_BEGIN_PROFILE_SESSION("Shutdown", "Shutdown-Profile.json");
	delete app;
	CYCLOPE_END_PROFILE_SESSION();

}