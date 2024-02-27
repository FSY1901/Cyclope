#include "Cpch.h"
#include "Entity.h"
#include "Components.h"

namespace Cyclope {

	Entity::Entity(entt::entity handle, Scene* scene) : m_Entity(handle), m_Scene(scene) {}

	UUID Entity::GetUUID() {
		return GetComponent<IDComponent>().id;
	}

	std::string& Entity::Tag() {
		return GetComponent<TagComponent>().tag;
	}

	TransformComponent& Entity::Transform() {
		return GetComponent<TransformComponent>();
	}

	void Entity::Destroy() {
		m_Scene->DestroyEntity(*this);
	}

}