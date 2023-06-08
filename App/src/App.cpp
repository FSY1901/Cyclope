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
			 0.5f,  0.5f, 0.0f,  // top right
			 0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left 
		};
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		sh = Shader("./src/shader.glsl");
		vao.Generate();
		vbo.Generate();
		ebo.Generate();
		vao.Bind();
		vbo.Bind();
		vbo.SetData(vertices, sizeof(vertices));
		ebo.Bind();
		ebo.SetData(indices, sizeof(indices));
		vao.LinkEBO(&vbo, &ebo);
	}

	void OnUpdate() override {
		Renderer::ClearColor(0.2f, 0.3f, 0.3f);

		sh.Use();
		vao.DrawEBO(TRIANGLES, 6);

		if (Input::KeyPressed(Keys::Q)) {
			std::cout << "Pressed";
		}
	}
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
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