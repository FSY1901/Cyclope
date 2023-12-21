#include <memory>

namespace Cyclope {

	namespace Pointer {

		template <typename T> struct Pointer {};

		template <typename T> struct Pointer<T*>
		{
			typedef T Type;
		};

		template <typename T> struct Pointer<std::shared_ptr<T>>
		{
			typedef T Type;
		};

		template <typename T> struct Pointer<std::unique_ptr<T>>
		{
			typedef T Type;
		};

		/*
		Example Usage:
		std::unique_ptr<T> t;
		Pointer::Pointer<decltype(t)>::Type <-- give you the type
		*/

	}

}