#pragma once
#include "Cone.h"
class Pyramid :
	public Cone
{
public:
	Pyramid(float height, float radius): Cone(height, radius, 0, 4) {}
	~Pyramid() {}
};

