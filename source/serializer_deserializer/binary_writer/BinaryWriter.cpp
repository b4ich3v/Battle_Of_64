#include "BinaryWriter.h"

bool BinaryWriter::open(const char* fileName)
{

    file.open(fileName, std::ios::binary | std::ios::trunc); // create, overwrite
    return file.is_open();

}

bool BinaryWriter::write(const void* source, size_t bytes) 
{

    return bool(file.write((const char*)(source), bytes)); // true on full write

}

void BinaryWriter::close() 
{

    if (file.is_open()) file.close(); // flush and close

}
