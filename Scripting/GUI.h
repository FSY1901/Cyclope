#pragma once
#include "Cyclope.h"
#include <unordered_map>

namespace Scripting {

	using GUIFunction = void(*)(Cyclope::Entity& e);
	using GUIFunctionRegistry = std::unordered_map<size_t, GUIFunction>;

	static GUIFunctionRegistry guiFunctionRegistry;

	inline uint8_t RegisterGUIFunction(size_t tag, GUIFunction func) {
		return guiFunctionRegistry.insert(GUIFunctionRegistry::value_type(tag, func)).second;
	}

#define REGISTER_GUI_FUNCTION(TYPE, FUNC)				\
	namespace {											\
		const uint8_t reg_gui_function_##TYPE			\
			{ RegisterGUIFunction(						\
				std::hash<std::string>()(#TYPE),		\
				[](Cyclope::Entity& e) {FUNC }			\
		)};												\
	}													

}