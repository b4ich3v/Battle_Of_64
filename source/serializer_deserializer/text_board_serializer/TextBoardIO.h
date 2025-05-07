#include "BoardSerializer.h"
#pragma once

class TextBoardIO: public BoardSerializer
{
public:

    explicit TextBoardIO(const MyString& fileName);

    void save(const Board& board) const override;

    void load(Board& board) const override;

};
