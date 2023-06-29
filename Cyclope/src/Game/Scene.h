#ifndef SCENE_H
#define SCENE_H

#include "Core.h"
#include "Components.h"

#include "entt.hpp"

namespace Cyclope {

	class Entity;

	class CYCLOPE_API Scene {

	public:
		Scene();
		~Scene();

		void Update();

		Entity CreateEntity();

	private:
		entt::registry m_Registry;

		friend class Entity;

	};

}

#endif // !SCENE_H