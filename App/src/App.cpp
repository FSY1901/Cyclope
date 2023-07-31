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
		/*TextureSpecification t;
		t.Format = ImageFormat::RGBA8;
		t.Width = 64;
		t.Height = 64;
		GLubyte checkImage[64][64][4];
		int i, j, c;

		for (i = 0; i < 64; i++) {
			for (j = 0; j < 64; j++) {
				c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
				checkImage[i][j][0] = (GLubyte)c;
				checkImage[i][j][1] = (GLubyte)c;
				checkImage[i][j][2] = (GLubyte)c;
				checkImage[i][j][3] = (GLubyte)255;
			}
		}
		tex = Texture2D::Create(t, checkImage);
		tex->SetData(checkImage);*/
		auto v = VertexBuffer::Create(vertices, sizeof(vertices));
		v->SetBufferLayout(BufferLayout::Standard());
		vert = VertexArray::Create(v, IndexBuffer::Create(indices, sizeof(indices)));
		RenderCommands::SetClearColor(0.2f, 0.3f, 0.3f);
	}

	void OnUpdate() override {
		RenderCommands::Clear();
		tex->Bind();
		Matrix4 mat = Matrix4(1.0f);
		mat = glm::rotate(mat, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mat = glm::rotate(mat, (float)Time::GetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		sh->SetMat4("transform", mat);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		sh->SetMat4("view", view);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)Application::GetInstance()->GetWindow()->GetWidth() / (float)Application::GetInstance()->GetWindow()->GetHeight(), 0.1f, 100.0f);
		//glm::ortho(-800.0f / 600.0f, 800.0f / 600.0f, -1.0f, 1.0f, 0.1f, 100.0f);
		sh->SetMat4("proj", projection);
		Renderer::Submit(vert, sh);

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
};

class EditorLayer : public Layer {
private:
	Scene* m_ActiveScene = new Scene();
public:
	void OnAttach() override {
		Entity entity = m_ActiveScene->CreateEntity();
		//entity.AddComponent<TransformComponent>(); -- All entities have transforms
		/*if (entity.HasComponent<TransformComponent>()) {
			std::cout << entity.GetComponent<TransformComponent>().position.x;
			entity.RemoveComponent<TransformComponent>();
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