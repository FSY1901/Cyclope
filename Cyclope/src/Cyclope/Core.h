#pragma once

#ifdef CYCLOPE_BUILD_DLL
	#define	CYCLOPE_API __declspec(dllexport)
#else
	#define	CYCLOPE_API __declspec(dllimport)
#endif // CYCLOPE_BUILD_DLL

#ifdef CYCLOPE_ENABLE_ASSERTS
#define CYCLOPE_ASSERT(x, ...) {if(!(x)){CYCLOPE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define CYCLOPE_CORE_ASSERT(x, ...) {if(!(x)){CYCLOPE_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define CYCLOPE_ASSERT(x, ...)
#endif

#include <memory>
namespace Cyclope {

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Unique<T> MakeUnique(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Shared = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Shared<T> MakeShared(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}