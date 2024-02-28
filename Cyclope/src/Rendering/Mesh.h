#pragma once

#include "Core.h"

#include "../Maths/Maths.h"
#include <vector>

#include "Shader.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Cyclope {

	struct CYCLOPE_API Vertex {

		Vector3 position;
		Vector3 normal;
		Vector2 uv;

	};

	struct CYCLOPE_API Texture { //maybe rename
		Shared<Texture2D> texture;
		std::string type;
		std::string path;
	};

	extern class VertexBuffer;
	extern class VertexArray;

	struct CYCLOPE_API Mesh {

		Mesh() = default;
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

		void Draw(Shared<Shader> shader);
		Shared<VertexArray> GetVA() { return va; }

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;
	private:
		Shared<VertexBuffer> vb;
		Shared<VertexArray> va;

		void SetupMesh();

	};

	class CYCLOPE_API Model
	{
	public:
		Model(){}
		Model(std::string path)
		{
			m_path = path;
			loadModel(path);
		}
		void Draw(Shared<Shader> shader);
		std::string GetPath() { return m_path; }
	private:
		// model data
		std::vector<Mesh> m_meshes;
		std::vector<Texture> m_textures_loaded;
		std::string m_directory;
		std::string m_path;

		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
			std::string typeName);
	};

}