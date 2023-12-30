/*
This file is used for various rendering commands that can be called by the user if he needs a custom renderer
*/

#include "Core.h"

#include "glad.h"
#include "glfw3.h"

#include "Buffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Camera.h"

#include "../Game/Components.h"

#ifndef RENDERING_H
#define RENDERING_H

namespace Cyclope {

	//TODO: Make this API unspecific

	enum class RendererAPI {
		OpenGL
	};
	
	enum class CYCLOPE_API DrawMode {
		Point = GL_POINT,
		Line = GL_LINE,
		Fill = GL_FILL
	};

	enum class CYCLOPE_API RenderingOperation{
		DepthTest = GL_DEPTH_TEST,
		Blending = GL_BLEND
	};

	struct CYCLOPE_API RenderCommands
	{
		static void SetClearColor(float r, float g, float b);
		static void Clear();
		static void SetDrawMode(DrawMode mode);
		static void SetViewport(int width, int height);

		static void Enable(RenderingOperation op);
		static void Disable(RenderingOperation op);
	};

	struct CYCLOPE_API Batch {

	public:
		void AddToBatch(const Mesh& mesh, const Matrix4& transform);
		void GenerateBatch();

		GLuint batchVertexArray;
		GLuint batchVertexBuffer;
		GLuint batchIndexBuffer;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		unsigned int lastTotalVertices = 0;

	};

	struct RendererData
	{
		glm::mat4 viewProjectionMatrix;
	};

	struct RendererStats {
		uint32_t drawCalls = 0;
		uint32_t renderedVertices = 0;
	};

	class CYCLOPE_API Renderer {
	public:
		static void BeginScene(const Camera& camera);
		static void EndScene();
		static void Submit(const Shared<VertexArray>& vertexArray, const Shared<Shader>& shader);
		static void Submit(const Shared<Batch>& batch, const Shared<Shader>& shader);

		static RendererAPI GetAPI();
		static RendererStats GetRenderStats();
	private:
		static RendererData s_data;
		static RendererAPI s_RendererAPI;
		static RendererStats s_stats;
	};

}

#endif // !RENDERING_H