#pragma once

#include <unordered_map>
#include "../Game/Entity.h"

namespace Cyclope {
	//TODO: Add RemoveComponentFunction
	using AddComponentFunction = void(*)(Entity& e);
	using AddNativeScriptFunction = void(*)(Entity& e);
	using ComponentRegistry = std::unordered_map<size_t, AddComponentFunction>;
	using NativeScriptRegistry = std::unordered_map<size_t, AddNativeScriptFunction>;

	static ComponentRegistry componentRegistry;
	static NativeScriptRegistry nativeScriptRegistry;

	inline uint8_t RegisterComponent(size_t tag, AddComponentFunction func) {
		return componentRegistry.insert(ComponentRegistry::value_type({ tag, func })).second;
	}

	inline uint8_t RegisterNativeScript(size_t tag, AddNativeScriptFunction func) {
		return nativeScriptRegistry.insert(ComponentRegistry::value_type({ tag, func })).second;
	}

#define REGISTER_COMPONENT(TYPE)								\
	struct TYPE;												\
	namespace {													\
		const uint8_t reg_##TYPE								\
		{ Cyclope::RegisterComponent(							\
			std::hash<std::string>()(#TYPE),					\
			[](Cyclope::Entity& e){e.AddComponent<TYPE>();})	\
		};														\
	}

#define REGISTER_SCRIPT(TYPE)									\
	class TYPE;													\
	namespace {													\
		const uint8_t reg_##TYPE								\
		{ Cyclope::RegisterNativeScript(							\
			std::hash<std::string>()(#TYPE),					\
			[](Cyclope::Entity& e){e.GetComponent<Cyclope::NativeScriptComponent>().Bind<TYPE>();})	\
		};														\
	}

}
