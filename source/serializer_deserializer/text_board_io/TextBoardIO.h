#include "BoardIO.h"
#pragma once

class TextBoardIO: public BoardIO
{
public:

    explicit TextBoardIO(const MyString& fileName);

    void save(const Board& board) const override;

    void load(Board& board) const override;

};
