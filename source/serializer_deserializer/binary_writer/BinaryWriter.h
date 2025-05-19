#include <fstream>
#include "Writer.h"
#pragma once

class BinaryWriter: public Writer 
{
private:

    std::ofstream file;

public:

    bool open(const char* fileName) override;

    bool write(const void* source, size_t bytes) override;

    void close() override;

};
