#include <iostream>

#include "Cyclope.h"

using namespace Cyclope;

class UI : public Layer {

public:
	void OnImGuiRender() override {
		ImGui::Begin("ImGui Window");
		ImGui::Text("Test Text");
		ImGui::End();
	}
};

class RenderLayer : public Layer {
public:
	void OnAttach() override {
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

	void OnUpdate() override {
		Renderer::ClearColor(0.2f, 0.3f, 0.3f);

		sh.Use();
		vao.Bind();
		vao.Draw(TRIANGLES, 0, 3);

		if (Input::KeyPressed(Keys::Q)) {
			std::cout << "Pressed";
		}
	}
private:
	VAO vao;
	VBO vbo;
	Shader sh;
};

class App : public Application{

public:

	App(int width, int height, const char* title) : Application(width, height, title) {
		PushLayer(new UI());
		PushLayer(new RenderLayer());
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	return new App(800, 600, "Cyclope Engine");
}