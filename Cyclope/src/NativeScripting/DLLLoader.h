#pragma once

#include "../NativeScripting/Scripting.h"
#include "imgui.h"

#define NOMINMAX
#include "windows.h"
#include <iostream>

namespace Cyclope {

	typedef void(*FUNC)(ComponentRegistry& components, ComponentNamesList& componentNames, 
		NativeScriptRegistry& scripts, NativeScriptNamesList& scriptNames, LayerList& layers);
	typedef void(*GUIFunction)(ImGuiContext* ctx, Entity& e, const std::string& componentName);

	class CYCLOPE_API DLLLoader {
	public:
		void LoadDLL(ComponentRegistry& components, ComponentNamesList& names, 
			NativeScriptRegistry& scripts, NativeScriptNamesList& scriptNames, LayerList& layers);
		
		GUIFunction LoadComponentGUIFunction();
		
		void FreeDLL();

	private:
		HINSTANCE hDLL;
	};

}