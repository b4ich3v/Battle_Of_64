#include <iostream>
#pragma once

class Reader
{
public:

    virtual bool open(const char* fileName) = 0;

    virtual bool read(void* desination, size_t bytes) = 0;

    virtual void close() = 0;

    virtual ~Reader() = default;

};

