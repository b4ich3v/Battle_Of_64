#include <memory>
#include "Board.h"
#include "MyString.h"
#pragma once

enum class SaveType: uint8_t
{

    TXT, 
    BINARY

};

class BoardIO 
{
protected:

    MyString fileName;

public:

    explicit BoardIO(const MyString& fileName);

    virtual ~BoardIO() = default;

    virtual void save(const Board& board) const = 0;
    
    virtual void load(Board& board) const = 0;

};
