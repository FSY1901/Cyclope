#pragma once

#include "Entity.h"

namespace Cyclope {

	class CYCLOPE_API ScriptableEntity : public Entity {

	public:
		template<typename T>
		bool HasComponent() {
			return m_entity.HasComponent<T>();
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_entity.AddComponent<T>(std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent() {
			return m_entity.GetComponent<T>();
		}

		template<typename T>
		void RemoveComponent() {
			m_entity.RemoveComponent<T>();
		}

	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(float dt) {}

	private:
		Entity m_entity;
		friend class Scene;

	};

}