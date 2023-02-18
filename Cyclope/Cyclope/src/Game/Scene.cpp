#include "Scene.h"

namespace Cyclope {

	Scene* Scene::m_activeScene = nullptr;

	Scene::Scene() {
		if (!m_activeScene) {
			m_activeScene = this;
		}
	}

	Scene::~Scene() {
		for (auto o : m_objects) {
			delete o;
		}
	}

	Object* Scene::AddObject(Object* obj) {
		Object* o = new Object(*obj);
		o->m_components.clear();
		m_objects.push_back(o);
		auto& g = m_objects[m_objects.size() - 1];
		for (auto& c : obj->m_components) {
			Component* comp = c->Clone();
			comp->object = g;
			g->m_components.push_back(comp);
		}
		return g;
	}

	Scene* Scene::GetActiveScene() { return m_activeScene; }

	void Scene::SetActiveScene(Scene* scene) { m_activeScene = scene; }

}