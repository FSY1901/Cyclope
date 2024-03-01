#include "Cpch.h"
#include "ProjectSerializer.h"

#include <fstream>

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

namespace Cyclope {

	ProjectSerializer::ProjectSerializer(Shared<Project>& project)
	: m_project(project) {}

	void ProjectSerializer::Serialize(const std::filesystem::path& filepath)
	{
		auto& config = m_project->GetConfig();
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Project" << YAML::Value;
		{
			out << YAML::BeginMap;
			out << YAML::Key << "Name";
			out << YAML::Value << config.name;
			out << YAML::Key << "Asset Directory";
			out << YAML::Value << config.assetDirectory.string();
			out << YAML::Key << "Scripts Path";
			out << YAML::Value << config.scriptPath.string();
			out << YAML::Key << "Start Scene";
			out << YAML::Value << config.startScene.string();
			out << YAML::EndMap;
		}
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath.string());

		auto& projectNode = data["Project"];
		if (!projectNode)
			return false;

		auto& config = m_project->GetConfig();

		config.name = projectNode["Name"].as<std::string>();
		config.assetDirectory = projectNode["Asset Directory"].as<std::string>();
		config.scriptPath = projectNode["Scripts Path"].as<std::string>();
		config.startScene = projectNode["Start Scene"].as<std::string>();

		return true;
	}

}