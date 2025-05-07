#include "FactoryIO.h"
#include "TextBoardIO.h"
#include "BinaryBoardIO.h"

MySharedPtr<BoardIO> FactoryIO::create(SaveType type, const MyString& fileName)
{

    switch (type)
    {

    case SaveType::TXT: return MySharedPtr<BoardIO>(new TextBoardIO(fileName));
    case SaveType::BINARY: return MySharedPtr<BoardIO>(new BinaryBoardIO(fileName));
    default: break;

    }

    return MySharedPtr<BoardIO>();

}
