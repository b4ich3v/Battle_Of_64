#pragma once
#include "MySharedPtr.hpp"
#include "BoardIO.h"

class FactoryIO
{
public:

    static MySharedPtr<BoardIO> create(SaveType type, const MyString& fileName);

};
