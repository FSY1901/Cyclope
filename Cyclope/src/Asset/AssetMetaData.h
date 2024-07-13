#pragma once

#include "Asset.h"

#include <filesystem>

namespace Cyclope {

	struct AssetMetadata
	{
		AssetType Type = AssetType::None;
		std::filesystem::path filePath;

		operator bool() const { return Type != AssetType::None; }
	};

}