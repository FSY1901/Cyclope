#include "ScriptLoaderLayer.h"

namespace CyclopeEditor {

	GUIFunction ScriptLoaderLayer::s_DisplayComponent = GUIFunction();

	void ScriptLoaderLayer::OnAttach() {

		dllLoader.LoadDLL(componentRegistry(), componentNamesList(),
			nativeScriptRegistry(), nativeScriptNamesList(), layerList());

		//load all layers
		for (auto addLayer : layerList()) {
			addLayer(Application::GetInstance());
		}

		s_DisplayComponent = dllLoader.LoadComponentGUIFunction();

	}

}