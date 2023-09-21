#include <iostream>

#include "Cyclope.h"
#include "Comp.h"

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
		float vertices[] = {
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f // top left 
		};
		unsigned int indices[] = {
			0, 1, 3,   // first triangle
			1, 2, 3    // second triangle
		};
		sh = Shader::Create("./src/shader.glsl");
		tex = Texture2D::Create("./src/Obama.png");
		auto v = VertexBuffer::Create(vertices, sizeof(vertices));
		v->SetBufferLayout(BufferLayout::Standard());
		vert = VertexArray::Create(v, IndexBuffer::Create(indices, sizeof(indices)));
		RenderCommands::SetClearColor(0.2f, 0.3f, 0.3f);
		t.position = Vector3(0.0f, 0.0f, 3.0f);

		type t = type::get_by_name("MyStruct");//type::get<MyStruct>();
		for (auto& prop : t.get_properties())
			std::cout << prop.get_type().get_name() << ": " << prop.get_name() << std::endl;
	}

	void OnUpdate() override {
		RenderCommands::Clear();
		tex->Bind();
		Matrix4 mat = Matrix4(1.0f);
		mat = glm::rotate(mat, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, (float)Time::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		sh->SetMat4("transform", mat);
		cam.SetPerspectiveMatrix((float)Application::GetInstance()->GetWindow()->GetWidth() / (float)Application::GetInstance()->GetWindow()->GetHeight());
		/*cam.SetOrthographicMatrix(-(float)Application::GetInstance()->GetWindow()->GetWidth() / (float)Application::GetInstance()->GetWindow()->GetHeight(),
			(float)Application::GetInstance()->GetWindow()->GetWidth() / (float)Application::GetInstance()->GetWindow()->GetHeight(),
			-1.0f, 1.0f);*/
		cam.RecalculateViewMatrix(t);
		Renderer::BeginScene(cam);
		Renderer::Submit(vert, sh);
		Renderer::EndScene();

		if (Input::KeyPressed(Key::Q)) {
			std::cout << "Pressed";
		}
	}

	void OnDetach() {
		
	}

private:
	Shared<VertexArray> vert;
	Shared<Shader> sh;
	Shared<Texture2D> tex;
	Camera cam;
	TransformComponent t;
	Vector3 pos;
};

class EditorLayer : public Layer {
private:
	Scene* m_ActiveScene = new Scene();
public:
	bool pressed = false;
	void OnAttach() override {
		/*Entity entity = m_ActiveScene->CreateEntity();
		entity.AddComponent<CameraComponent>();// -- All entities have transforms
		if (entity.HasComponent<CameraComponent>()) {
			std::cout << entity.GetComponent<CameraComponent>().camera.fov;
		}*/
		
	}

	void OnUpdate() override {
		//m_ActiveScene->Update();
	}

	void OnDetach() override {
		delete m_ActiveScene;
	}
};

class App : public Application{

public:

	App(int width, int height, const char* title) : Application(width, height, title) {
		PushLayer(new UI());
		PushLayer(new RenderLayer());
		PushLayer(new EditorLayer());
	}

};

Cyclope::Application* Cyclope::CreateApplication() {
	return new App(800, 600, "Cyclope Engine");
}