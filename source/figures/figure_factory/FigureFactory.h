#include "Figure.h"
#pragma once

class FigureFactory 
{
public:

    // heap-allocates the requested piece (caller must delete)
    static Figure* create(FigureType type, MyColor color);

};
