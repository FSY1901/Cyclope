#include <iostream>

#include "Cyclope.h"

using namespace Cyclope;

class comp : public Component {

public:

	int x = 0;

	comp() {

	}

	void Start() override {
		
	}

	void Update() override {
		
	}

	comp* Clone() override {
		return new comp(*this);
	}

	void SetName(std::string _name) {
		object->name = _name;
	}

};

class App : public Application{

public:
	Object o = Object(Vector3f(), Vector3f(), Vector3f(), "name");
	Scene s;
	VAO vao;
	VBO vbo;
	Shader sh;
	App() {
		
	}

	App(int width, int height, const char* title) : Application(width, height, title) {
		
	}

	void Start() override {
		o.AddComponent<comp>();
		auto obj = s.AddObject(&o);
		obj->name = "name name name";
		auto obj1 = s.AddObject(&o);
		obj1->name = "dasdas";
		Scene::SetActiveScene(&s);

		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, // left  
			 0.5f, -0.5f, 0.0f, // right 
			 0.0f,  0.5f, 0.0f  // top   
		};

		sh = Shader("./src/shader.glsl");
		vao.Generate();
		vbo.Generate();
		vao.Bind();
		vbo.Bind();
		vbo.SetData(vertices, sizeof(vertices));
		vao.Link(&vbo);
		vbo.Unbind();
		vao.Unbind();
	}

	void Update() override {
		ClearColor(0.2f, 0.3f, 0.3f);

		sh.Use();
		vao.Bind();

		for (auto& o : Scene::GetActiveScene()->m_objects) {
			for (auto& c : o->m_components) {
				c->Update();
			}
		}

		if (Input::KeyPressed(Keys::Q)) {
			std::cout << "Pressed";
		}

	}

	void ImGuiUpdate() override {
		ImGui::SetCurrentContext((ImGuiContext*)ctx);
		// ImGUI window creation
		ImGui::Begin("My name is window, ImGUI window");
		// Text that appears in the window
		ImGui::Text("Hello there adventurer!");
		// Ends the window
		ImGui::End();
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	return new App(800, 600, "Cyclope Engine");
}