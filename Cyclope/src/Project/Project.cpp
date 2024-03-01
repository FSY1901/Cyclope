#include "Cpch.h"
#include "Project.h"
#include "ProjectSerializer.h"

namespace Cyclope {

	Shared<Project> Project::s_activeProject = nullptr;

	Shared<Project> Project::New()
	{
		s_activeProject = MakeShared<Project>();
		return s_activeProject;
	}
	Shared<Project> Project::Load(const std::filesystem::path& path) {

		Shared<Project> project = MakeShared<Project>();
		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path)) {
			project->m_projectDirectory = path.parent_path();
			s_activeProject = project;
			return s_activeProject;
		}

		return nullptr;

	}

	void Project::SaveActive(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_activeProject);
		s_activeProject->m_projectDirectory = path.parent_path();
		serializer.Serialize(path);
	}

}