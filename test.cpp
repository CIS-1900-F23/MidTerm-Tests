

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <type_traits>
// TODO: write your includes here



// TODO: replace the aliases with your own implementations 
// and remove the include for <memory> 
#include <memory>
template <typename T>
using __UniquePtr = std::unique_ptr<T>;

template <typename T>
using __SharedPtr = std::shared_ptr<T>;

template <typename T>
using __WeakPtr = std::weak_ptr<T>;


struct Foo 
{
    int a;
    int b;

    Foo(): a{5}, b{5} {}
};


// Needs basic constructors, dereference and arrow operator
TEST_CASE("Pointers work")
{
    int* int_ptr1 = new int{10};
    __SharedPtr<int>  sp1 {int_ptr1};

    int* int_ptr2 = new int{10};
    __UniquePtr<int>  up1 {int_ptr2};
    
    SECTION("Dereferences works")
    {
        REQUIRE(*sp1 == 10);  
        REQUIRE(*up1 == 10);  
    }

    *sp1 = 20;
    *up1 = 20;

    SECTION("Can change values")
    {
        REQUIRE(*sp1 == 20);  
        REQUIRE(*up1 == 20);  
    }

    Foo* foo_ptr1 = new Foo{};
    __SharedPtr<Foo> sp2 {foo_ptr1};

    Foo* foo_ptr2 = new Foo{};
    __UniquePtr<Foo> up2 {foo_ptr2};

    SECTION("Arrow operator works")
    {
        REQUIRE(sp2->a == 5); 
        REQUIRE(sp2->b == 5);  

        REQUIRE(up2->a == 5); 
        REQUIRE(up2->b == 5);  
    }

    sp2->a = 20;
    sp2->b = 20;
    
    up2->a = 20;
    up2->b = 20;

    SECTION("Can change values")
    {
        REQUIRE(sp2->a == 20); 
        REQUIRE(sp2->b == 20);   

        REQUIRE(up2->a == 20); 
        REQUIRE(up2->b == 20); 
    }
}


// Needs get, release, and reset
TEST_CASE("Special Functions work")
{
    int* int_ptr1 = new int{10};
    __SharedPtr<int>  sp1 {int_ptr1};
    int* int_ptr2 = new int{10};
    __UniquePtr<int>  up1 {int_ptr2};
    
    SECTION("get works")
    {
        REQUIRE(*(sp1.get()) == 10);  
        REQUIRE(*(up1.get()) == 10);  
    }

    SECTION("relase works")
    {
        delete up1.release();
        REQUIRE(up1.get() == nullptr);  
    }



    SECTION("reset works")
    {
        int* int_ptr3 = new int{20};
        sp1.reset(int_ptr3);
        REQUIRE(*sp1 == 20);

        int* int_ptr4 = new int{20};
        up1.reset(int_ptr4);
        REQUIRE(*up1 == 20);
        
    }
}


// Needs special constructors
TEST_CASE("Special constructors")
{   
    SECTION("Deleted Constructors")
    {
        REQUIRE(std::is_copy_constructible<__SharedPtr<int>>::value == true);
        REQUIRE(std::is_move_constructible<__SharedPtr<int>>::value == true);  
        REQUIRE(std::is_copy_assignable<__SharedPtr<int>>::value == true);
        REQUIRE(std::is_move_assignable<__SharedPtr<int>>::value == true);  
        REQUIRE(std::is_default_constructible<__SharedPtr<int>>::value == true);  

        REQUIRE(std::is_copy_constructible<__UniquePtr<int>>::value == false);
        REQUIRE(std::is_move_constructible<__UniquePtr<int>>::value == true);  
        REQUIRE(std::is_copy_assignable<__UniquePtr<int>>::value == false);
        REQUIRE(std::is_move_assignable<__UniquePtr<int>>::value == true);  
        REQUIRE(std::is_default_constructible<__UniquePtr<int>>::value == true);  
    }

    SECTION("Copy Constructor Shared")
    {
        int* int_ptr1 = new int{10};
        auto *sp1 = new  __SharedPtr<int>{int_ptr1};
        __SharedPtr<int>  sp2 {*sp1};

        delete sp1;
        // sp2 should still be valid
        REQUIRE(*sp2 == 10);  
    }

    SECTION("Copy Assignment Shared")
    {
        int* int_ptr1 = new int{10};
        auto *sp1 = new  __SharedPtr<int>{int_ptr1};
        int* int_ptr2 = new int{20};
        __SharedPtr<int>  sp2 {int_ptr2};
        sp2 = *sp1;

        delete sp1;
        // sp2 should still be valid
        REQUIRE(*sp2 == 10);  
    }

    SECTION("Move Constructor")
    {
        int* int_ptr1 = new int{10};
        __SharedPtr<int>  sp1 {int_ptr1};
        __SharedPtr<int>  sp2 {std::move(sp1)};

        int* int_ptr2 = new int{10};
        __UniquePtr<int>  up1 {int_ptr2};
        __UniquePtr<int>  up2 {std::move(up1)};

        REQUIRE(*sp2 == 10);  
        REQUIRE(sp1.get() == nullptr);  

        REQUIRE(*up2 == 10);  
        REQUIRE(up1.get() == nullptr);  
    }

    SECTION("Move Assingment")
    {
        int* int_ptr1 = new int{10};
        __SharedPtr<int>  sp1 {int_ptr1};
        int* int_ptr2 = new int{20};
        __SharedPtr<int>  sp2 {int_ptr2};
        sp2 = std::move(sp1);

        int* int_ptr3 = new int{10};
        __UniquePtr<int>  up1 {int_ptr3};
        int* int_ptr4 = new int{20};
        __UniquePtr<int>  up2 {int_ptr4};
        up2 = std::move(up1);

        REQUIRE(*sp2 == 10);  
        REQUIRE(sp1.get() == nullptr);  

        REQUIRE(*up2 == 10);  
        REQUIRE(up1.get() == nullptr);  
    }
}


TEST_CASE("Weak Ptr")
{
    int* int_ptr1 = new int{10};
    auto *sp1 = new  __SharedPtr<int>{int_ptr1};
    auto *sp2 = new  __SharedPtr<int>{*sp1};
    __WeakPtr<int> wp1 {*sp1};
    
    SECTION("expired works")
    {
        
        REQUIRE(wp1.expired() == false);  

        delete sp1;
        REQUIRE(wp1.expired() == false); 

        delete sp2;
        REQUIRE(wp1.expired() == true); 
    }

    SECTION("lock works")
    {
        auto sp3 = wp1.lock();
        
        REQUIRE(*sp3 == 10); 

        delete sp1;
        delete sp2;

        // still valid
        REQUIRE(*sp3 == 10); 
        REQUIRE(wp1.expired() == false); 
    }
}
