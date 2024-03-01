#pragma once

#include "Core.h"

#include "../Maths/Maths.h"
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Project/Project.h"

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

	class Model;

	//temporary
	struct CYCLOPE_API ModelManager {
		static std::unordered_map<std::string, Shared<Model>> loadedModels;
	};

	class CYCLOPE_API Model
	{
	public:
		Model() = default;
		static Shared<Model> Create(std::string path)
		{
			if (ModelManager::loadedModels.find(path) != ModelManager::loadedModels.end()) {
				return ModelManager::loadedModels[path];
			}
			else {
				Shared<Model> model = MakeShared<Model>();
				model->m_path = path;
				auto& p = (Project::GetActive()->GetProjectDirectory() /
					Project::GetActive()->GetAssetDirectory()
					/ path).string();
				model->loadModel(p);
				ModelManager::loadedModels[path] = model;
			}

			return ModelManager::loadedModels[path];
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