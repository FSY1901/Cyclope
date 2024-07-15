#include "Export.h"
#include "Components.h"
#include "Scripting.h"
#include "GUI.h"
#include <iostream>
namespace Scripting {

	void GetRegistries(Cyclope::ComponentRegistry& components, Cyclope::ComponentNamesList& names, 
		Cyclope::NativeScriptRegistry& scripts, Cyclope::NativeScriptNamesList& scriptNames, Cyclope::LayerList& layers) {
		components = Cyclope::componentRegistry();
		names = Cyclope::componentNamesList();
		scripts = Cyclope::nativeScriptRegistry();
		scriptNames = Cyclope::nativeScriptNamesList();
		layers = Cyclope::layerList();
	}

	void PrintComponents(ImGuiContext* ctx, Cyclope::Entity& e, const std::string& componentName) {

		ImGui::SetCurrentContext(ctx);
		size_t hash = std::hash<std::string>()(componentName);
		if (guiFunctionRegistry.find(hash) != guiFunctionRegistry.end()) {
			auto func = guiFunctionRegistry.at(hash);
			func(e);
		}

	}

}