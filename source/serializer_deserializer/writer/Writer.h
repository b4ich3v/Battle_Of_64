#include <iostream>
#pragma once

class Writer
{
public:

    virtual bool open(const char* fileName) = 0; // open file for writing; truncate if it exists

    virtual bool write(const void* source, size_t bytes) = 0; // write "bytes" from "source", return true if all bytes written

    virtual void close() = 0; // flush and close handle

    virtual ~Writer() = default;

};
