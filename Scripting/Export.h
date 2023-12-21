#define EXPORT __declspec(dllimport)

#include "Scripting.h"
#include "Game/Entity.h"

namespace Scripting {

	static int i = 69;
	extern "C" EXPORT void Serialize();
	extern "C" EXPORT void Deserialize();
	extern "C" EXPORT void AddComponent(const char* name, Cyclope::Entity& e);
	extern "C" EXPORT void RemoveComponent(const char* name, Cyclope::Entity& e);
	extern "C" EXPORT void Test(Cyclope::ComponentRegistry& registry);

}