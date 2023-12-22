#ifndef SCENE_H
#define SCENE_H

#include "Core.h"

#include "entt.hpp"

namespace Cyclope {

	class Entity;

	class CYCLOPE_API Scene {

	public:
		Scene();
		~Scene();

		void Update(float dt);

		Entity CreateEntity();

		template<typename T>
		auto View() {
			auto view = m_Registry.view<T>();
			return view;
		}

	private:
		entt::registry m_Registry;

		friend class Entity;

	};

}

#endif // !SCENE_H