#include "Cpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "NativeScripting/Scripting.h"

namespace Cyclope {

	Scene::Scene() {

	}

	Scene::~Scene() {

	}

	template<typename Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap) {
		auto view = src.view<Component>();
		for (auto e : view) {
			UUID uuid = src.get<IDComponent>(e).id;
			entt::entity id = enttMap.at(uuid);

			auto& component = src.get<Component>(e);
			dst.emplace_or_replace<Component>(id, component);
		}
	}

	Shared<Scene> Scene::Copy(Shared<Scene> other)
	{
		Shared<Scene> newScene = MakeShared<Scene>();

		auto& src = other->m_Registry;
		auto& dest = newScene->m_Registry;

		std::unordered_map<UUID, entt::entity> enttMap;

		auto idView = src.view<IDComponent>();
		//TODO: Fix reverse order loading
		for (auto e : idView) {
			UUID uuid = src.get<IDComponent>(e).id;
			const auto& name = src.get<TagComponent>(e).tag;
			Entity entity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = entity;
		}

		CopyComponent<TransformComponent>(dest, src, enttMap);
		CopyComponent<CameraComponent>(dest, src, enttMap);
		CopyComponent<DirectionalLightComponent>(dest, src, enttMap);
		CopyComponent<PointLightComponent>(dest, src, enttMap);
		CopyComponent<SpotLightComponent>(dest, src, enttMap);
		CopyComponent<ModelRendererComponent>(dest, src, enttMap);
		CopyComponent<NativeScriptComponent>(dest, src, enttMap);

		for (auto& name : componentNamesList()) {
			auto func = componentRegistry().at(std::hash<std::string>{}(name));
			func.CopyComponent(dest, src, enttMap);
		}
		
		return newScene;
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
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, std::string name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity& entity) {
		m_Registry.destroy(entity);
	}

}