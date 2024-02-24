#pragma once
#include "Scene.h"

namespace Cyclope {

	class CYCLOPE_API SceneSerializer {

	public:
		SceneSerializer(Shared<Scene>& scene);

		void Serialize(const std::string& filepath);
		bool Deserialize(const std::string& filepath);

	private:
		Shared<Scene> m_scene;

	};

}