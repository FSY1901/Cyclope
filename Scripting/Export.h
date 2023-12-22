#define EXPORT __declspec(dllimport)

#include "Scripting.h"
#include "Game/Entity.h"

namespace Scripting {

	extern "C" EXPORT void Serialize();
	extern "C" EXPORT void Deserialize();
	extern "C" EXPORT void GetRegistries(Cyclope::ComponentRegistry& components, Cyclope::NativeScriptRegistry& scripts);

}