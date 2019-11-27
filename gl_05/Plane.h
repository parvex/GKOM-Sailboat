#pragma once
#include "Mesh.h"
class Plane :
	public Mesh
{
public:
	Plane(float width, float length) {
		vector<glm::vec3> v = vertices(width, length);
		vector<unsigned>  t = triangles();
		vector<Vertex> vertices;

		for (int i = 0; i < v.size(); i++) {
			vertices.push_back(Vertex(v[i], glm::vec3(0.5f), glm::vec2(0.0f), glm::vec3(0.0f)));
		}

		setVertices(vertices);
		setIndices(t);
		recalculateNormales();
	}

private:
	vector<glm::vec3> vertices(float width, float length) {
		glm::vec3 p0 = glm::vec3(-width * .5f, 0.0f, -length * .5f);
		glm::vec3 p1 = glm::vec3(width * .5f, 0.0f, -length * .5f);
		glm::vec3 p2 = glm::vec3(width * .5f, 0.0f, length * .5f);
		glm::vec3 p3 = glm::vec3(-width * .5f, 0.0f, length * .5f);

		vector<glm::vec3> vertices = {
			p0, p1, p2, p3,
		};

		return vertices;
	}

	vector<unsigned> triangles() {
		vector<unsigned> triangles = {
			0, 1, 2, 2, 3, 0
		};

		return triangles;
	}
};

