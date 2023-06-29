#include "Entity.h"

namespace Cyclope {

	Entity::Entity(entt::entity handle, Scene* scene) : m_Entity(handle), m_Scene(scene) {}

	TransformComponent& Entity::Transform() {
		return GetComponent<TransformComponent>();
	}

}