#include "Figure.h"

Figure::Figure(Color color, FigureType type): 
	color(color), type(type) {}

Color Figure::getColor() const 
{

	return color; 

}

FigureType Figure::getType() const 
{ 

	return type; 

}
