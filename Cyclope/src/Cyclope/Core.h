#ifdef CYCLOPE_BUILD_DLL
	#define	CYCLOPE_API __declspec(dllexport)
#else
	#define	CYCLOPE_API __declspec(dllimport)
#endif // CYCLOPE_BUILD_DLL

#include <memory>
namespace Cyclope {

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T>
	using Shared = std::shared_ptr<T>;

}