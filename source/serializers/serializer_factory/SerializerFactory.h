#pragma once
#include "MySharedPtr.hpp"
#include "BoardSerializer.h"

class SerializerFactory 
{
public:
    
    static MySharedPtr<BoardSerializer> create(SaveType type, const MyString& fileName);

};
