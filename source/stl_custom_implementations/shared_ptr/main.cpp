#include <iostream>
#include "MySharedPtr.hpp"

struct Foo 
{
public:

    int x;

    Foo(int x) : x(x) 
    {

        std::cout << "Foo(" << x << ") constructed "<< std::endl;

    }

    ~Foo() 
    {

        std::cout << "Foo(" << x << ") destroyed "<< std::endl;

    }

    void say() const
    {

        std::cout << "Foo says " << x << std::endl;

    }

};

int main() 
{
    
    MySharedPtr<Foo> ptr1(new Foo(42));
    std::cout << ptr1.use_count() << std::endl;

    {
        
        MySharedPtr<Foo> ptr2 = ptr1;
        std::cout << ptr2.use_count() << std::endl;
        ptr2->say();

    }
    
    std::cout << ptr1.use_count() << std::endl;
    ptr1.reset(new Foo(7));
    std::cout << ptr1.use_count() << std::endl;

    MySharedPtr<Foo> ptr3;
    ptr3 = ptr1;
    std::cout << ptr3.use_count() << std::endl;

    ptr1.swap(ptr3);
    std::cout << ptr1.use_count() << " " << ptr3.use_count() << std::endl;

    ptr1.reset();
    ptr3.reset();
    std::cout << (bool)ptr1 << " " << (bool)ptr3 << std::endl;

    return 0;

}
