#include <iostream>

#include "Cyclope.h"

using namespace Cyclope;

class App : public Application{

public:
	VAO vao;
	VBO vbo;
	Shader sh;

	App() {
		
	}

	App(int width, int height, const char* title) : Application(width, height, title) {
		
	}

	void Start() override {

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
		ImGui::SetCurrentContext((ImGuiContext*)ctx);
	}

	void Update() override {
		Renderer::ClearColor(0.2f, 0.3f, 0.3f);

		sh.Use();
		vao.Bind();
		vao.Draw(TRIANGLES, 0, 3);

		/*for (auto& o : Scene::GetActiveScene()->m_objects) {
			for (auto& c : o->m_components) {
				c->Update();
			}
		}*/

		if (Input::KeyPressed(Keys::Q)) {
			std::cout << "Pressed";
		}

	}

	void ImGuiUpdate() override {
		// ImGUI window creation
		ImGui::Begin("My name is window");
		// Text that appears in the window
		ImGui::Text("Hello there adventurer!");
		// Ends the window
		ImGui::End();
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	return new App(800, 600, "Cyclope Engine");
}