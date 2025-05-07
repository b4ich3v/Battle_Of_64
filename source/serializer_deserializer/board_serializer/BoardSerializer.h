#include <memory>
#include "Board.h"
#include "MyString.h"
#pragma once

enum class SaveType: uint8_t
{

    TXT, 
    BINARY

};

class BoardSerializer 
{
protected:

    MyString fileName;

public:

    explicit BoardSerializer(const MyString& fileName);

    virtual ~BoardSerializer() = default;

    virtual void save(const Board& board) const = 0;
    
    virtual void load(Board& board) const = 0;

};
