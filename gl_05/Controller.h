#pragma once
#include "Mesh.h"

class Controller : public Mesh
{
public:
	Controller()
	{
		vector<Vertex> vertexes;
		vertexes.push_back(Vertex(glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		
		vertexes.push_back(Vertex(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f)));

		vertexes.push_back(Vertex(glm::vec3(0.02f, 0.0, 0.0), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.02f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.02f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		vertexes.push_back(Vertex(glm::vec3(0.02f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		vertexes.push_back(Vertex(glm::vec3(0.02f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

		setVertices(vertexes);
		setIndices(vector<unsigned>
		{ 0, 1, 2,
		  0, 3, 2,
		  4, 2, 3,
		
			5, 6, 7,
			5, 8, 7,
			9, 7, 8});
	}
};

