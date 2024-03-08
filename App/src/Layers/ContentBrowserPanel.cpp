#include "ContentBrowserPanel.h"
#include "Project/Project.h"

using namespace Cyclope;

namespace CyclopeEditor {

	void ContentBrowserPanel::Init()
	{
		currentDirectory = Project::GetActive()->GetProjectDirectory() /
			Project::GetActive()->GetAssetDirectory();

		fileIcon = Texture2D::Create("./Resources/textures/File.png");
		folderIcon = Texture2D::Create("./Resources/textures/Folder.png");

		auto& active = Project::GetActive();
		assetDir = active->GetProjectDirectory() / active->GetAssetDirectory();
		currentDirectory = assetDir;
		LoadDirectory(currentDirectory);
	}

	void ContentBrowserPanel::Draw() {

		ImGui::Begin("Content Browser");

		ImGui::Text(std::filesystem::relative(currentDirectory, assetDir).string().c_str());

		if (currentDirectory != assetDir) {
			if (ImGui::Button("<-")) {
				currentDirectory = currentDirectory.parent_path();
				LoadDirectory(currentDirectory);
			}
		}

		float iconSize = 90.0f;
		float padding = 16.0f;
		float cellSize = iconSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columns = (int)(panelWidth / cellSize);
		if (columns < 1)
			columns = 1;
		ImGui::Columns(columns, 0, false);

		int i = 0;
		for (auto& entry : entries) {
			ImGui::PushID(i++);
			auto& path = entry.path;

			Shared<Texture2D> icon = entry.isDirectory ? folderIcon : fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 1, 1, 0));
			ImGui::ImageButton((void*)(intptr_t)icon->GetTexture(), ImVec2(iconSize, iconSize), { 0, 1 }, { 1, 0 });
			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = path.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (entry.isDirectory) {
					currentDirectory /= entry.filename;
					LoadDirectory(currentDirectory);
				}
			}
			ImGui::TextWrapped(entry.filename.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();

	}

	void ContentBrowserPanel::LoadDirectory(std::filesystem::path dir)
	{
		entries.clear();
		for (auto& p : std::filesystem::directory_iterator(dir)) {
			DirectoryEntry entry;
			entry.isDirectory = p.is_directory();
			auto relative = std::filesystem::relative(p.path(), currentDirectory);
			auto relString = relative.filename().string();
			entry.filename = relString;
			entry.path = p.path();

			entries.push_back(entry);
		}
	}

}