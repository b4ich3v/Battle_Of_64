#include "Figure.h"
#pragma once

class FigureFactory 
{
public:

    static Figure* create(FigureType type, MyColor color);

};
