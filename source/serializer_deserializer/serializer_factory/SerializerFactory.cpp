#include "SerializerFactory.h"
#include "TextBoardSerializer.h"
#include "BinaryBoardSerializer.h"

MySharedPtr<BoardSerializer> SerializerFactory::create(SaveType type, const MyString& fileName)
{

    switch (type)
    {

    case SaveType::TXT: return MySharedPtr<BoardSerializer>(new TextBoardSerializer(fileName));
    case SaveType::BINARY: return MySharedPtr<BoardSerializer>(new BinaryBoardSerializer(fileName));
    default: break;

    }
    
    return MySharedPtr<BoardSerializer>();

}
