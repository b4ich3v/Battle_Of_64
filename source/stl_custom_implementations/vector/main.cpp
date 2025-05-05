#include <iostream>
#include "MyVector.hpp"

int main()
{

    MyVector<int> v;
    std::cout  << v.empty() << std::endl;

    for (int i = 1; i <= 5; i++)
    {

        v.push_back(i * 10);

    }

    std::cout << v << std::endl;
    std::cout << v.size() << std::endl << std::endl;

    MyVector<int> copy = v;
    std::cout << copy << std::endl;

    MyVector<int> moved = std::move(v);
    std::cout << moved << std::endl;
    std::cout << v.empty() << std::endl << std::endl;

    try
    {

        std::cout << moved[2] << std::endl;
        moved.pop_back();
        std::cout << moved << std::endl;

    }
    catch (const std::out_of_range& e)
    {

        std::cout << e.what() << std::endl;

    }
    std::cout << std::endl;

    MyVector<int> inputVec;
    std::cin >> inputVec;
    std::cout << inputVec << std::endl;

    return 0;

}
