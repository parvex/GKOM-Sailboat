#pragma once
#include "Cone.h"
class Cylinder :
	public Cone
{
public:
	Cylinder(float height, float radius, int nbSides = 32): Cone(height, radius, radius, nbSides) {}
	~Cylinder() {}
};

