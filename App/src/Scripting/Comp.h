#include "rttr/registration.h"
#include "Game/Entity.h"

//using namespace rttr;

struct A
{
    int data;
};
struct MyStruct { MyStruct() {}; void func(double) {}; A data; int x; };

/*
RTTR_REGISTRATION
{
    rttr::registration::class_<MyStruct>("MyStruct")
         .constructor<>()
         .property("data", &MyStruct::data)
         .property("x", &MyStruct::x)
         .method("func", &MyStruct::func);
}

        rttr::type t = rttr::type::get_by_name("MyStruct");//type::get<MyStruct>();
        MyStruct m;
        for (auto& prop : t.get_properties()) {
            std::cout << prop.get_type().get_name() << ": " << prop.get_name() << std::endl;
        }
*/
