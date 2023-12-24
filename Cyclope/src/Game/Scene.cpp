#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Cyclope {

	Scene::Scene() {

	}

	Scene::~Scene() {

	}

	void Scene::Update(float dt) {
		
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& script) {
				//TODO: Revisit this: Should it be allowed to have an unbound NSC?
				if (script.InstantiateScript == nullptr)
					return;

				if (!script.instance) {
					script.instance = script.InstantiateScript();
					script.instance->m_entity = Entity{ entity, this };
					script.instance->OnCreate();
				}
				
				script.instance->OnUpdate(dt);

			});
	
	}

	Entity Scene::CreateEntity() {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::ForEach(void(*EntityIteratorFunction)(Entity)) {
		m_Registry.each([&](auto entity) {
				EntityIteratorFunction(Entity{ entity, this });
			});
	}

}