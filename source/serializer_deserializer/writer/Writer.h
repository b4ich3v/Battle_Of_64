#include <iostream>
#pragma once

class Writer
{
public:

    virtual bool open(const char* fileName) = 0;

    virtual bool write(const void* source, size_t bytes) = 0;

    virtual void close() = 0;

    virtual ~Writer() = default;

};
