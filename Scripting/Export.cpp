#include "Export.h"
#include "Components.h"
#include "Scripting.h"
namespace Scripting {

	void Serialize() {

		std::cout << "Hello" << std::endl;

	}

	void Deserialize() {

		std::cout << "DeHello" << std::endl;

	}

	void AddComponent(const char* name, Cyclope::Entity& e) {
		if (name == "PlayerComponent") {
			e.AddComponent<PlayerComponent>();
		}
		else if (name == "") {

		}
	}

	void RemoveComponent(const char* name, Cyclope::Entity& e) {
		if (name == "") {

		}
		else if (name == " ") {

		}
	}

	void Test(Cyclope::ComponentRegistry& registry) {
		//Cyclope::Scene _s;
		//Cyclope::Entity f = s->CreateEntity();
		//for (auto e : s->View<Cyclope::TransformComponent>()) {
			//Cyclope::CameraComponent p;
			//Cyclope::Entity{ e, s }.AddComponent<decltype(p)>();
			//TODO: c++ typelist library
		//}

		//for (auto e : s->View<Cyclope::TransformComponent>()) {
		//	std::cout << "entt" << std::endl;
		//}
		std::cout << "Test" << std::endl;
		registry = Cyclope::reg;
	}

}