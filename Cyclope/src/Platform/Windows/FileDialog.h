#include "Core.h"

namespace Cyclope {

	class CYCLOPE_API FileDialog {

	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		static std::string GetRelativeFilePath(const char* filter);

	};

}