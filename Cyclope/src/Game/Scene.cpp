#include "Scene.h"

namespace Cyclope {

	Scene::Scene() {

	}

	Scene::~Scene() {

	}

	void Scene::Update() {

	}

	entt::entity Scene::CreateEntity() { return m_Registry.create(); }

}