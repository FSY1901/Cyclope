#include "Cyclope.h"

#include "../NativeScripting/DLLLoader.h"

namespace CyclopeEditor {

	using namespace Cyclope;

	class ScriptLoaderLayer : public Layer {
	
	public:
		void OnAttach();

		static GUIFunction GetComponentDisplayFunction() { return s_DisplayComponent; }

	private:
		static GUIFunction s_DisplayComponent;

		DLLLoader dllLoader;
	};
}