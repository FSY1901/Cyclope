#include "Cpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include "NativeScripting/Scripting.h"

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static YAML::Node encode(const glm::quat& rhs)
		{
			Node node;
			node.push_back(rhs.w);
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.w = node[0].as<float>();
			rhs.x = node[1].as<float>();
			rhs.y = node[2].as<float>();
			rhs.z = node[3].as<float>();
			return true;
		}
	};

}

namespace Cyclope {

	YAML::Emitter& operator<<(YAML::Emitter& out, const Vector3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Quaternion& q) {
		out << YAML::Flow;
		out << YAML::BeginSeq << q.w << q.x << q.y << q.z << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(Shared<Scene>& scene)
	: m_scene(scene) {}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		out << YAML::BeginMap;
		out << YAML::Key << "Entity";
		out << YAML::Value << "12321312313";

		if (entity.HasComponent<TagComponent>()) {
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>().tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap; 
		}

		if (entity.HasComponent<TransformComponent>()) {
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << transform.position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshRendererComponent>()) {
			out << YAML::Key << "MeshRendererComponent";
			out << YAML::BeginMap;
			//TODO: Implement this
			out << YAML::EndMap;
		}

		//custom components
		for (auto& name : componentNamesList()) {
			auto func = componentRegistry().at(std::hash<std::string>{}(name));
			if (func.HasComponent(entity)) {
				out << YAML::Key << name;
				out << YAML::BeginMap;
				//TODO: Implement this
				out << YAML::EndMap;
			}
		}

		if (entity.HasComponent<NativeScriptComponent>()) {
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "ScriptName" << YAML::Value << entity.GetComponent<NativeScriptComponent>().scriptName;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Name";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		m_scene->m_Registry.each([&](auto id) {
			Entity e = { id, m_scene.get() };
			if (!e)
				return;

			SerializeEntity(out, e);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as < std::string>();

		auto entities = data["Entities"];
		if (entities) {
			std::vector<YAML::Node> entitiesRev(entities.begin(),
				entities.end());
			for (auto entity = entitiesRev.rbegin(); entity != entitiesRev.rend(); ++entity) {
				std::string name;
				auto tagComponent = (*entity)["TagComponent"];
				if (tagComponent) {
					name = tagComponent["Tag"].as<std::string>();
				}

				Entity deserializedEntity = m_scene->CreateEntity(name);

				auto transformComponent = (*entity)["TransformComponent"];
				if (transformComponent) {
					auto& transform = deserializedEntity.GetComponent<TransformComponent>();
					transform.position = transformComponent["Position"].as<Vector3>();
					transform.rotation = transformComponent["Rotation"].as<Quaternion>();
					transform.scale = transformComponent["Scale"].as<Vector3>();
				}

				auto meshRendererComponent = (*entity)["MeshRendererComponent"];
				if (meshRendererComponent) {
					deserializedEntity.AddComponent<MeshRendererComponent>();
					//implement
				}

				auto nativeScriptComponent = (*entity)["NativeScriptComponent"];
				if (nativeScriptComponent) {
					deserializedEntity.AddComponent<NativeScriptComponent>();
					auto scriptName = nativeScriptComponent["ScriptName"].as<std::string>();
					if (scriptName != "None") {
						auto func = nativeScriptRegistry().at(std::hash<std::string>{}(scriptName));
						func(deserializedEntity);
					}
					deserializedEntity.GetComponent<NativeScriptComponent>().instance = nullptr;
				}

				for (auto& _name : componentNamesList()) {
					auto customComponent = (*entity)[_name];
					if (customComponent) {
						auto func = componentRegistry().at(std::hash<std::string>{}(_name));
						func.AddComponent(deserializedEntity);
					}
				}
			}
		}

		return true;
	}

}