#include "BoardIO.h"
#pragma once

class BinaryBoardIO: public BoardIO 
{
public:

    explicit BinaryBoardIO(const MyString& fileName);

    void save(const Board& board) const override;

    void load(Board& board) const override;

};
