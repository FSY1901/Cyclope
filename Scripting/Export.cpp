#include "Export.h"
#include "Components.h"
#include "Scripting.h"
#include <iostream>
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

	void GetRegistries(Cyclope::ComponentRegistry& components, Cyclope::NativeScriptRegistry& scripts) {
		components = Cyclope::componentRegistry;
		scripts = Cyclope::nativeScriptRegistry;
	}

}