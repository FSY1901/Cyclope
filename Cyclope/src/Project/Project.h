#pragma once
#include "Core.h"

#include <filesystem>
#include <string>

namespace Cyclope {

	struct ProjectConfig {
		std::string name = "New Project";
		std::filesystem::path startScene = "";

		std::filesystem::path assetDirectory = "";
		std::filesystem::path scriptPath = "";
	};

	class CYCLOPE_API Project {

	public:
		ProjectConfig& GetConfig() { return m_config; }
		const std::filesystem::path& GetProjectDirectory() { return m_projectDirectory; }
		const std::filesystem::path& GetAssetDirectory() {
			return m_config.assetDirectory; }

		static Shared<Project> New();
		static Shared<Project> Load(const std::filesystem::path& path);
		static void SaveActive(const std::filesystem::path& path);

		static Shared<Project> GetActive() { return s_activeProject; }

	private:
		ProjectConfig m_config;
		std::filesystem::path m_projectDirectory;

		static Shared<Project> s_activeProject;

	};

}