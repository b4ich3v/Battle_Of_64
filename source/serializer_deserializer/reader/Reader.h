#include <iostream>
#pragma once

class Reader
{
public:

    virtual bool open(const char* fileName) = 0; // open file for reading; return true on success

    virtual bool read(void* desination, size_t bytes) = 0; // read "bytes" into "destination", return true if all bytes read

    virtual void close() = 0; // close underlying handle (safe-to-call twice)

    virtual ~Reader() = default; // polymorphic destruction

};
