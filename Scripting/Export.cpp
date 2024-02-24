#include "Export.h"
#include "Components.h"
#include "Scripting.h"
#include "GUI.h"
#include <iostream>
namespace Scripting {

	void Serialize() {

		std::cout << "Hello" << std::endl;

	}

	void Deserialize() {

		std::cout << "DeHello" << std::endl;

	}

	void GetRegistries(Cyclope::ComponentRegistry& components, Cyclope::ComponentNamesList& names, 
		Cyclope::NativeScriptRegistry& scripts, Cyclope::NativeScriptNamesList& scriptNames) {
		components = Cyclope::componentRegistry();
		names = Cyclope::componentNamesList();
		scripts = Cyclope::nativeScriptRegistry();
		scriptNames = Cyclope::nativeScriptNamesList();
	}

	void PrintComponents(ImGuiContext* ctx, Entity& e, const std::string& componentName) {

		ImGui::SetCurrentContext(ctx);
		size_t hash = std::hash<std::string>()(componentName);
		if (guiFunctionRegistry.find(hash) != guiFunctionRegistry.end()) {
			auto func = guiFunctionRegistry.at(hash);
			func(e);
		}

	}

}