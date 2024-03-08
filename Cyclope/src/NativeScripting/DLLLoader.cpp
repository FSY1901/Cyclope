#include "Cpch.h"
#include "DLLLoader.h"
#include "Project/Project.h"

namespace Cyclope {

	void DLLLoader::LoadDLL(ComponentRegistry& components, ComponentNamesList& componentNames, NativeScriptRegistry& scripts, NativeScriptNamesList& scriptNames) {
		auto& project = Project::GetActive();
		std::string s = (project->GetProjectDirectory() / project->GetConfig().scriptPath).string().c_str();
		std::wstring stemp = std::wstring(s.begin(), s.end());
		LPCWSTR sw = stemp.c_str();
		hDLL = LoadLibrary(sw);

		if (hDLL != NULL)
		{

			FUNC func = (FUNC)GetProcAddress(hDLL, "GetRegistries");

			if (!func) {
				std::cout << "could not locate the function \n";
			}
			else {
				func(components, componentNames, scripts, scriptNames);
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