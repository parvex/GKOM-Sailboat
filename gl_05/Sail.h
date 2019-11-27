#pragma once
#include "Mesh.h"



class Sail : public Mesh
{
public:
	Sail()
	{
		vector<Vertex> vertexes;
		vertexes.push_back(Vertex(glm::vec3(0.0f), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0f), glm::vec3(-1.0, 0.0, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0f), glm::vec3(-1.0, 0.0, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0f), glm::vec3(-1.0, 0.0, 0.0f)));

		vertexes.push_back(Vertex(glm::vec3(0.02f, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0f), glm::vec3(1.0, 0.0, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.02f, 5.0f, 0.0f), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0f), glm::vec3(1.0, 0.0, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.02f, 0.0f, 3.0f), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0f), glm::vec3(1.0, 0.0, 0.0f)));

		setVertices(vertexes);
		setIndices(vector<unsigned>{ 0, 1, 2, 3, 4, 5 });
	}
};

