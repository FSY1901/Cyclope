#ifndef SCENE_H
#define SCENE_H

#include "../Cyclope/Core.h"
#include "Object.h"
#include <memory>

namespace Cyclope {

	class API Scene {
	
	public:

		Scene();
		~Scene();
		Object* AddObject(Object* obj);

		static Scene* GetActiveScene();
		static void SetActiveScene(Scene* scene);

		std::vector<Object*> m_objects;
	private:

		static Scene* m_activeScene;

		friend class Application;

	};

}

#endif // !SCENE_H