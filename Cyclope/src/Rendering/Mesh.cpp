#include "Cpch.h"
#include "Mesh.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "Rendering.h"

#include <filesystem>

namespace Cyclope {

    static Shared<Texture2D> TextureFromFile(const char* path, const std::string& directory)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        Shared<Texture2D> texture = Texture2D::Create(filename.c_str());

        return texture;
    }

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        SetupMesh();
    }

    void Mesh::SetupMesh() {

        vb = VertexBuffer::Create(vertices);
        va = VertexArray::Create(vb, IndexBuffer::Create(&indices[0], indices.size()));

    }

    void Mesh::Draw(Shared<Shader> shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            std::string number;
            std::string name = textures[i].type;
            if (name == "texture_diffuse") {
                number = std::to_string(diffuseNr++);
            }
            else if (name == "texture_specular") {
                number = std::to_string(specularNr++);
            }

            shader->SetInt(("material." + name + number).c_str(), i);
            textures[i].texture->Bind(i);
        }
        glActiveTexture(GL_TEXTURE0);

        Renderer::Submit(va, shader);

    }

    void Model::Draw(Shared<Shader> shader)
    {
        for (unsigned int i = 0; i < m_meshes.size(); i++)
            m_meshes[i].Draw(shader);
    }

    void Model::loadModel(std::string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeMeshes);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            CYCLOPE_CORE_ERROR("Could not load model from path: {0}", path);
            return;
        }
        m_directory = std::filesystem::path(path).remove_filename().string();//path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            Vector3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                Vector2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.uv = vec;
            }
            else
                vertex.uv = Vector2(0.0f, 0.0f);
            vertices.push_back(vertex);
        }
        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process material
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            std::vector<Texture> specularMaps = loadMaterialTextures(material,
                aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < m_textures_loaded.size(); j++)
            {
                if (std::strcmp(m_textures_loaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(m_textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                texture.texture = TextureFromFile(str.C_Str(), m_directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                m_textures_loaded.push_back(texture); // add to loaded textures
            }
        }
        return textures;
    }

}