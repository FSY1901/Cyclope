#define EXPORT __declspec(dllimport)

#include "Scripting.h"
#include "Game/Entity.h"
#include "imgui.h"

namespace Scripting {

	extern "C" EXPORT void GetRegistries(Cyclope::ComponentRegistry& components, Cyclope::ComponentNamesList& componentNames, 
		Cyclope::NativeScriptRegistry& scripts, Cyclope::NativeScriptNamesList& scriptNames, Cyclope::LayerList& layers);
	extern "C" EXPORT void ComponentGUIFunction(ImGuiContext* ctx, Cyclope::Entity& e, const std::string & componentName);

}