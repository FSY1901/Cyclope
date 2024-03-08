#include "Cyclope.h"
#include "Rendering/Texture.h"
#include <filesystem>

using namespace Cyclope;

namespace CyclopeEditor {

	struct DirectoryEntry {
		bool isDirectory;
		std::string filename;
		std::filesystem::path path;
	};

	class ContentBrowserPanel {

	public:
		ContentBrowserPanel() = default;
		void Init();
		void Draw();
	private:
		std::filesystem::path assetDir;
		std::filesystem::path currentDirectory;
		std::vector<DirectoryEntry> entries;

		void LoadDirectory(std::filesystem::path dir);

		Shared<Texture2D> folderIcon;
		Shared<Texture2D> fileIcon;

	};

}