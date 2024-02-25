#ifndef SCENE_H
#define SCENE_H

#include "Core.h"

#include "Events/Event.h"

#include "entt.hpp"

namespace Cyclope {

	class Entity;

	class CYCLOPE_API Scene {

	public:
		bool m_playing = false; // temporary until scene playing is added
		Scene();
		~Scene();

		void Update(float dt);
		void OnEvent(Event& e);
		void Render();

		Entity CreateEntity(std::string name = "Entity");
		void DestroyEntity(Entity& entity);

		template<typename T>
		auto View() {
			auto view = m_Registry.view<T>();
			return view;
		}

		template<typename Func>
		void ForEach(Func func) {
			m_Registry.each([&](auto entity) {
				func(Entity{ entity, this });
			});
		}

	private:
		entt::registry m_Registry;

		friend class Entity;
		extern friend class SceneSerializer;

	};

}

#endif // !SCENE_H