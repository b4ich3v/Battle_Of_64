#include "BoardSerializer.h"
#pragma once

class TextBoardSerializer: public BoardSerializer 
{
public:

    explicit TextBoardSerializer(const MyString& fileName);

    void save(const Board& board) const override;

    void load(Board& board) const override;

};
