#include "BoardSerializer.h"
#pragma once

class BinaryBoardSerializer: public BoardSerializer 
{
public:

    explicit BinaryBoardSerializer(const MyString& fileName);

    void save(const Board& board) const override;

    void load(Board& board) const override;

};
