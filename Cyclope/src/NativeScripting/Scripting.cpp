#include "Cpch.h"
#include "Scripting.h"

namespace Cyclope {

	ComponentRegistry& componentRegistry() {
		static ComponentRegistry cr;
		return cr;
	}

	NativeScriptRegistry& nativeScriptRegistry() {
		static NativeScriptRegistry nsr;
		return nsr;
	}
	
	ComponentNamesList& componentNamesList() {
		static ComponentNamesList cnl;
		return cnl;
	};
	
	NativeScriptNamesList& nativeScriptNamesList() {
		static NativeScriptNamesList nsnl = std::vector<std::string>{ "None" };
		return nsnl;
	}
}