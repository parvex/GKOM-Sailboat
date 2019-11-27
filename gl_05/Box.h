#pragma once
#include "Mesh.h"

class Box : public Mesh {
public:
	Box(float width, float height, float length) {
		vector<glm::vec3> v = vertices(width, height, length);
		vector<unsigned>  t = triangles();
		vector<Vertex> vertices;

		for (int i = 0; i < v.size(); i++) {
			vertices.push_back(Vertex(v[i], glm::vec3(0.2f, 0.31f, 0.27f), glm::vec2(0.0f), glm::vec3(0.0f)));
		}

		setVertices(vertices);
		setIndices(t);
		recalculateNormales();
	}

private:
	vector<glm::vec3> vertices(float width, float height, float length) {
		glm::vec3 p0 = glm::vec3(-width * .5f, -height * .5f, -length * .5f);
		glm::vec3 p1 = glm::vec3( width * .5f, -height * .5f, -length * .5f);
		glm::vec3 p2 = glm::vec3( width * .5f, -height * .5f,  length * .5f);
		glm::vec3 p3 = glm::vec3(-width * .5f, -height * .5f,  length * .5f);

		glm::vec3 p4 = glm::vec3(-width * .5f, height * .5f, -length * .5f);
		glm::vec3 p5 = glm::vec3( width * .5f, height * .5f, -length * .5f);
		glm::vec3 p6 = glm::vec3( width * .5f, height * .5f,  length * .5f);
		glm::vec3 p7 = glm::vec3(-width * .5f, height * .5f,  length * .5f);

		vector<glm::vec3> vertices = {
			// Bottom
			p0, p1, p2, p3,
			// Top
			p4, p5, p6, p7
		};

		return vertices;
	}

	vector<unsigned> triangles() {
		vector<unsigned> triangles = {
			// Bottom
			0, 3, 2, 2, 1, 0,
			// Top
			4, 5, 6, 6, 7, 4,
			// Left
			0, 1, 5, 5, 4, 0,
			// Right
			2, 3, 7, 7, 6, 2,
			// Back
			1, 2, 6, 6, 5, 1,
			// Front
			0, 4, 7, 7, 3, 0, 
		};

		return triangles;
	}

};

