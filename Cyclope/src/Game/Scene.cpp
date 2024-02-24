#include "Cpch.h"
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

	void Scene::OnEvent(Event& e) {
		m_Registry.view<NativeScriptComponent>().each([&](auto entity, NativeScriptComponent& script) {
			if (script.InstantiateScript == nullptr)
				return;
			//TODO: Revisit this; just a test for now!
			script.instance->OnEvent(e);
			if (e.Handled)
				return;

			});
	}

	void Scene::Render() {

		//TODO: Implement this
		//m_Registry.view<MeshRendererComponent>().each([=](auto entity, MeshRendererComponent& mesh) {});

	}

	Entity Scene::CreateEntity(std::string name) {
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	void Scene::DestroyEntity(Entity& entity) {
		m_Registry.destroy(entity);
	}

}