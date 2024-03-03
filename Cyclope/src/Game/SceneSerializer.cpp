#include "Cpch.h"
#include "SceneSerializer.h"

#include "Entity.h"
#include "Components.h"

#include "NativeScripting/Scripting.h"

#include "Project/Project.h"

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
		out << YAML::Value << entity.GetUUID();

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

		if (entity.HasComponent<ModelRendererComponent>()) {
			out << YAML::Key << "ModelRendererComponent";
			out << YAML::BeginMap;
			auto& model = entity.GetComponent<ModelRendererComponent>();
			out << YAML::Key << "Path" << YAML::Value << model.model->GetPath();
			out << YAML::Key << "Shader" << YAML::Value << model.shader->GetPath();
			out << YAML::Key << "Diffuse" << YAML::Value << model.diffuse;
			out << YAML::Key << "Specular" << YAML::Value << model.specular;
			out << YAML::Key << "Shininess" << YAML::Value << model.shininess;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<DirectionalLightComponent>()) {
			out << YAML::Key << "DirectionalLightComponent";
			out << YAML::BeginMap;
			auto& light = entity.GetComponent<DirectionalLightComponent>();
			out << YAML::Key << "ambient" << YAML::Value << light.ambient;
			out << YAML::Key << "ambientValue" << YAML::Value << light.ambientValue;
			out << YAML::Key << "diffuse" << YAML::Value << light.diffuse;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<PointLightComponent>()) {
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap;
			auto& light = entity.GetComponent<PointLightComponent>();
			out << YAML::Key << "ambient" << YAML::Value << light.ambient;
			out << YAML::Key << "ambientValue" << YAML::Value << light.ambientValue;
			out << YAML::Key << "diffuse" << YAML::Value << light.diffuse;
			out << YAML::Key << "radius" << YAML::Value << light.radius;
			out << YAML::Key << "intensity" << YAML::Value << light.intensity;
			out << YAML::Key << "cutOff" << YAML::Value << light.cutOff;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpotLightComponent>()) {
			out << YAML::Key << "SpotLightComponent";
			out << YAML::BeginMap;
			auto& light = entity.GetComponent<SpotLightComponent>();
			out << YAML::Key << "ambient" << YAML::Value << light.ambient;
			out << YAML::Key << "ambientValue" << YAML::Value << light.ambientValue;
			out << YAML::Key << "diffuse" << YAML::Value << light.diffuse;
			out << YAML::Key << "intensity" << YAML::Value << light.intensity;
			out << YAML::Key << "cutOff" << YAML::Value << light.cutOff;
			out << YAML::Key << "outerCutOff" << YAML::Value << light.outerCutOff;
			out << YAML::Key << "linear" << YAML::Value << light.linear;
			out << YAML::Key << "quadratic" << YAML::Value << light.quadratic;
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
				uint64_t uuid = (*entity)["Entity"].as<uint64_t>();

				std::string name;
				auto tagComponent = (*entity)["TagComponent"];
				if (tagComponent) {
					name = tagComponent["Tag"].as<std::string>();
				}

				Entity deserializedEntity = m_scene->CreateEntityWithUUID(uuid, name);

				auto transformComponent = (*entity)["TransformComponent"];
				if (transformComponent) {
					auto& transform = deserializedEntity.GetComponent<TransformComponent>();
					transform.position = transformComponent["Position"].as<Vector3>();
					transform.rotation = transformComponent["Rotation"].as<Quaternion>();
					transform.scale = transformComponent["Scale"].as<Vector3>();
				}

				auto meshRendererComponent = (*entity)["ModelRendererComponent"];
				if (meshRendererComponent) {
					auto& model = deserializedEntity.AddComponent<ModelRendererComponent>();
					model.model = Model::Create(meshRendererComponent["Path"].as<std::string>());
					model.shader = Shader::CreateRealtiveToProject(meshRendererComponent["Shader"].as<std::string>());
					model.diffuse = meshRendererComponent["Diffuse"].as<Vector3>();
					model.specular = meshRendererComponent["Specular"].as<Vector3>();
					model.shininess = meshRendererComponent["Shininess"].as<float>();
				}

				auto directionalLightComponent = (*entity)["DirectionalLightComponent"];
				if (directionalLightComponent) {
					auto& light = deserializedEntity.AddComponent<DirectionalLightComponent>();
					light.ambient = directionalLightComponent["ambient"].as<Vector3>();
					light.ambientValue = directionalLightComponent["ambientValue"].as<float>();
					light.diffuse = directionalLightComponent["diffuse"].as<Vector3>();
				}

				auto pointLightComponent = (*entity)["PointLightComponent"];
				if (pointLightComponent) {
					auto& light = deserializedEntity.AddComponent<PointLightComponent>();
					light.ambient = pointLightComponent["ambient"].as<Vector3>();
					light.ambientValue = pointLightComponent["ambientValue"].as<float>();
					light.diffuse = pointLightComponent["diffuse"].as<Vector3>();
					light.cutOff = pointLightComponent["cutOff"].as<float>();
					light.radius = pointLightComponent["radius"].as<float>();
					light.intensity = pointLightComponent["intensity"].as<float>();
				}

				auto spotLightComponent = (*entity)["SpotLightComponent"];
				if (spotLightComponent) {
					auto& light = deserializedEntity.AddComponent<SpotLightComponent>();
					light.ambient = spotLightComponent["ambient"].as<Vector3>();
					light.ambientValue = spotLightComponent["ambientValue"].as<float>();
					light.diffuse = spotLightComponent["diffuse"].as<Vector3>();
					light.cutOff = spotLightComponent["cutOff"].as<float>();
					light.outerCutOff = spotLightComponent["outerCutOff"].as<float>();
					light.intensity = spotLightComponent["intensity"].as<float>();
					light.linear = spotLightComponent["linear"].as<float>();
					light.quadratic = spotLightComponent["quadratic"].as<float>();
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

				//Custom Components
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