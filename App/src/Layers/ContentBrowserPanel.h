#include "Cyclope.h"
#include "Rendering/Texture.h"
#include <filesystem>

using namespace Cyclope;

namespace CyclopeEditor {

	class ContentBrowserPanel {

	public:
		ContentBrowserPanel() = default;
		void Init();
		void Draw();
	private:
		std::filesystem::path currentDirectory;
		Shared<Texture2D> folderIcon;
		Shared<Texture2D> fileIcon;

	};

}