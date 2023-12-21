#pragma once

#include <unordered_map>
#include "Game/Entity.h"

namespace Cyclope {

	using AddComponentFunction = void(*)(Entity& e);
	using ComponentRegistry = std::unordered_map<size_t, AddComponentFunction>;
	static ComponentRegistry reg;

	inline uint8_t RegisterComponent(size_t tag, AddComponentFunction func) {
		return reg.insert(ComponentRegistry::value_type({ tag, func })).second;
	}

#define REGISTER_COMPONENT(TYPE)				\
	struct TYPE;									\
	namespace {									\
		const uint8_t reg_##TYPE				\
		{ Cyclope::RegisterComponent(					\
			std::hash<std::string>()(#TYPE),	\
			[](Cyclope::Entity& e){e.AddComponent<TYPE>();})\
		}; \
	}

}
