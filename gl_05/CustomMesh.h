#pragma once
#include "Mesh.h"
class CustomMesh :
	public Mesh
{
public:
	CustomMesh(vector<Vertex> vertices, vector<unsigned int> indices, Texture texture)
	{
		setVertices(vertices);
		setIndices(indices);
		setTexture(texture);
	}
};

