#pragma once

#include "Core.h"

#include "AssetManagerBase.h"

#include "../Project/Project.h"

namespace Cyclope {
	
	class AssetManager
	{
	public:
		template<typename T>
		static Shared<T> GetAsset(AssetHandle handle)
		{
			Shared<Asset> asset = Project::GetActive()->GetAssetManager()->GetAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}

		static bool IsAssetHandleValid(AssetHandle handle)
		{
			return Project::GetActive()->GetAssetManager()->IsAssetHandleValid(handle);
		}

		static bool IsAssetLoaded(AssetHandle handle)
		{
			return Project::GetActive()->GetAssetManager()->IsAssetLoaded(handle);
		}

		static AssetType GetAssetType(AssetHandle handle)
		{
			return Project::GetActive()->GetAssetManager()->GetAssetType(handle);
		}
	};
}