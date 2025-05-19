#include <fstream>
#include "Reader.h"
#pragma once

class BinaryReader: public Reader 
{

private:

    std::ifstream file;

public:

    bool open(const char* fileName) override;

    bool read(void* destination, size_t bytes)  override;

    void close() override;

};
