#include "rttr/registration.h"

using namespace rttr;

struct A
{
    int data;
};
struct MyStruct { MyStruct() {}; void func(double) {}; A data; int x; };

RTTR_REGISTRATION
{
    registration::class_<MyStruct>("MyStruct")
         .constructor<>()
         .property("data", &MyStruct::data)
         .property("x", &MyStruct::x)
         .method("func", &MyStruct::func);
} 

/*
Loading a dll:

#include "windows.h"

typedef void(*FUNC)();

void LoadDLL() {
	HINSTANCE hDLL;

	hDLL = LoadLibrary(L"D:\\VS_Projects\\Cyclope\\bin\\Debug-windows-x86_64\\Scripting\\Scripting.dll");

	if (hDLL != NULL)
	{

		std::cout << "It worked! \n";

		FUNC func = (FUNC)GetProcAddress(hDLL, "PrintSomething");

		if (!func) {
			std::cout << "could not locate the function \n";
		}
		else {
			func();
		}

		FreeLibrary(hDLL);
	}
	else {
		std::cout << "It didn't work! \n";
	}
}

*/