#include "MyString.h"
#include <iostream>

int main() 
{

    MyString s1;
    std::cout << s1.empty() << " " << s1.size() << std::endl;

    MyString s2("Hello");
    std::cout << s2 << " " << s2.size() << std::endl;

    s2 += ", world!";
    std::cout << s2 << std::endl;

    MyString s3(" Goodbye");
    s2 += s3;
    std::cout << s2 << std::endl;

    MyString s4 = s2;
    std::cout << s4 << std::endl;

    MyString s5 = std::move(s4);
    std::cout << s5 << std::endl;
    std::cout << s4.empty() << std::endl;

    s5[0] = 'h';
    std::cout << s5 << std::endl;

    MyString part = s5.substr(0, 5);
    std::cout << part << std::endl;

    MyString input;
    std::cin >> input;
    std::cout << input << std::endl;

    return 0;

}

