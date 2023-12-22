#include "Scene.h"
#include "Components.h"

#include "entt.hpp"

#ifndef CYCLOPE_ENTITY_H
#define CYCLOPE_ENTITY_H

namespace Cyclope {

	class CYCLOPE_API Entity {

	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.all_of<T>(m_Entity);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Scene->m_Registry.emplace<T>(m_Entity, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			return m_Scene->m_Registry.get<T>(m_Entity);
		}

		template<typename T>
		void RemoveComponent() {
			m_Scene->m_Registry.remove<T>(m_Entity);
		}

		TransformComponent& Transform();

	private:
		entt::entity m_Entity;
		Scene* m_Scene;

	};

}

#endif // !CYCLOPE_ENTITY_H
