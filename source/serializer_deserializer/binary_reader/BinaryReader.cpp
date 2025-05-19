#include "BinaryReader.h"

bool BinaryReader::open(const char* fileName) 
{

    file.open(fileName, std::ios::binary); 
    return file.is_open();

}

bool BinaryReader::read(void* destination, size_t bytes) 
{

    return bool(file.read((char*)(destination), bytes));

}

void BinaryReader::close() 
{ 

    if (file.is_open()) file.close(); 

}
