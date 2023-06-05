#ifndef SCENE_H
#define SCENE_H

#include "Core.h"
#include "Components.h"

#include "entt.hpp"

namespace Cyclope {

	class CYCLOPE_API Scene {

	public:
		Scene();
		~Scene();

		void Update();

		entt::entity CreateEntity();

	private:
		entt::registry m_Registry;

	};

}

#endif // !SCENE_H