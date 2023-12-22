#include "DLLLoader.h"

namespace Cyclope {

	void DLLLoader::LoadDLL(ComponentRegistry& components, NativeScriptRegistry& scripts) {
		//TODO: Make relative to project directory
		hDLL = LoadLibrary(L"D:\\VS_Projects\\Cyclope\\bin\\Release-windows-x86_64\\Scripting\\Scripting.dll");

		if (hDLL != NULL)
		{

			std::cout << "Loaded DLL \n";

			FUNC func = (FUNC)GetProcAddress(hDLL, "GetRegistries");

			if (!func) {
				std::cout << "could not locate the function \n";
			}
			else {
				func(components, scripts);
			}
		}
		else {
			std::cout << "Could not load DLL! \n";
		}

	}

	void DLLLoader::FreeDLL() {
		FreeLibrary(hDLL);
		std::cout << "Freed DLL" << std::endl;
	}

}