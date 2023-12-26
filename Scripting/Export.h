#define EXPORT __declspec(dllimport)

#include "Scripting.h"
#include "Game/Entity.h"
#include "imgui.h"

namespace Scripting {

	extern "C" EXPORT void Serialize();
	extern "C" EXPORT void Deserialize();
	extern "C" EXPORT void GetRegistries(Cyclope::ComponentRegistry& components, Cyclope::ComponentNamesList& componentNames, 
		Cyclope::NativeScriptRegistry& scripts, Cyclope::NativeScriptNamesList& scriptNames);
	extern "C" EXPORT void PrintComponents(ImGuiContext* ctx, Cyclope::Entity& e, const std::string & componentName);

}