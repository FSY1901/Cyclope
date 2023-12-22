#pragma once

#include "../NativeScripting/Scripting.h"

#include "windows.h"
#include <iostream>

namespace Cyclope {

	typedef void(*FUNC)(ComponentRegistry& components, NativeScriptRegistry& scripts);

	class CYCLOPE_API DLLLoader {
	public:
		void LoadDLL(ComponentRegistry& components, NativeScriptRegistry& scripts);

		void FreeDLL();

	private:
		HINSTANCE hDLL;
	};

}