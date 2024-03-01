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
	}

	void ContentBrowserPanel::Draw() {
		auto& active = Project::GetActive();
		auto& assetDir = active->GetProjectDirectory() / active->GetAssetDirectory();

		ImGui::Begin("Content Browser");

		ImGui::Text(std::filesystem::relative(currentDirectory, assetDir).string().c_str());

		if (currentDirectory != assetDir) {
			if (ImGui::Button("<-")) {
				currentDirectory = currentDirectory.parent_path();
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
		for (auto& p : std::filesystem::directory_iterator(currentDirectory)) {
			ImGui::PushID(i++);
			auto& path = p.path();
			auto relative = std::filesystem::relative(path, currentDirectory);
			auto relString = relative.filename().string();

			Shared<Texture2D> icon = p.is_directory() ? folderIcon : fileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1, 1, 1, 0));
			ImGui::ImageButton((void*)(intptr_t)icon->GetTexture(), ImVec2(iconSize, iconSize), { 0, 1 }, { 1, 0 });
			
			if (ImGui::BeginDragDropSource()) {
				const wchar_t* itemPath = p.path().c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (p.is_directory()) {
					currentDirectory /= path.filename();
				}
			}
			ImGui::TextWrapped(relString.c_str());

			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::End();
	}

}