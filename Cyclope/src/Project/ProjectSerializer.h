#include "Core.h"
#include "Project.h"

namespace Cyclope {

	class CYCLOPE_API ProjectSerializer {

	public:
		ProjectSerializer() = default;
		ProjectSerializer(Shared<Project>& project);

		void Serialize(const std::filesystem::path& filepath);
		bool Deserialize(const std::filesystem::path& filepath);

	private:
		Shared<Project> m_project;

	};

}