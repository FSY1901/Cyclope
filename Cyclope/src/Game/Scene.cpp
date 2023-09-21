#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Cyclope {

	Scene::Scene() {

	}

	Scene::~Scene() {

	}

	void Scene::Update() {
		
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view) {
			TransformComponent transform = view.get<TransformComponent>(entity);
		}
	
	}

	Entity Scene::CreateEntity() {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		//TODO: Maybe add other components and maybe transform properties like position, scale etc. as parameters
		return entity;
	}

}