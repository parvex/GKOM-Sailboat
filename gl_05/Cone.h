#pragma once
#include "Mesh.h"
#include <cmath>
#define M_PI 3.14159265358979323846


class Cone :
	public Mesh
{
private:
	int nbSides;
	float height;
	float bottomRadius;
	float topRadius;

public:
	Cone(float height, float bottomRadius, float topRadius = 0, int nbSides = 32) {
		this->height = height;
		this->bottomRadius = bottomRadius;
		this->topRadius = topRadius;
		this->nbSides = nbSides;

		vector<glm::vec3> v = vertices();
		vector<unsigned>  t = triangles();
		vector<Vertex> vertices;

		for (int i = 0; i < v.size(); i++) {
			vertices.push_back(Vertex(v[i], glm::vec3(0.52f, 0.37f, 0.26f), glm::vec2(0.0f), glm::vec3(0.0f)));
		}

		setVertices(vertices);
		setIndices(t);
		recalculateNormales();




		//   Vector2[] uvs = new Vector2[vertices.Length];

		   //// Bottom cap
		   //int u = 0;
		   //uvs[u++] = new Vector2(0.5f, 0.5f);
		   //while (u <= nbSides)
		   //{
		   //	float rad = (float)u / nbSides * _2pi;
		   //	uvs[u] = new Vector2(Mathf.Cos(rad) * .5f + .5f, Mathf.Sin(rad) * .5f + .5f);
		   //	u++;
		   //}

		   //// Top cap
		   //uvs[u++] = new Vector2(0.5f, 0.5f);
		   //while (u <= nbSides * 2 + 1)
		   //{
		   //	float rad = (float)u / nbSides * _2pi;
		   //	uvs[u] = new Vector2(Mathf.Cos(rad) * .5f + .5f, Mathf.Sin(rad) * .5f + .5f);
		   //	u++;
		   //}

		   //// Sides
		   //int u_sides = 0;
		   //while (u <= uvs.Length - 4)
		   //{
		   //	float t = (float)u_sides / nbSides;
		   //	uvs[u] = new glm::vec3(t, 1f);
		   //	uvs[u + 1] = new glm::vec3(t, 0f);
		   //	u += 2;
		   //	u_sides++;
		   //}
		   //uvs[u] = new Vector2(1f, 1f);
		   //uvs[u + 1] = new Vector2(1f, 0f);
		   //#endregion



	}
	~Cone() {}

private:
	vector<glm::vec3> vertices() {
		vector<glm::vec3> vertices;
		int vert = 0;

		float _2pi = M_PI * 2.0f;

		// Bottom cap
		vertices.push_back(glm::vec3(0.0, -height / 2.0f, 0.0));
		if (bottomRadius > 0) {
			for (vert = 0; vert < nbSides; vert++) {
				float rad = vert / (float)nbSides * _2pi;
				vertices.push_back(glm::vec3(cos(rad) * bottomRadius, -height / 2.0f, sin(rad) * bottomRadius));
			}
		}
		

		// Top cap
		vertices.push_back(glm::vec3(0.0, height / 2.0f, 0.0));
		if (topRadius > 0) {
			for (vert = 0; vert < nbSides; vert++) {
				float rad = vert / (float)nbSides * _2pi;
				vertices.push_back(glm::vec3(cos(rad) * topRadius, height / 2.0f, sin(rad) * topRadius));
			}
		}

		return vertices;
	}

	vector<unsigned> triangles() {
		vector<unsigned> triangles;

		// Bottom cap
		if (bottomRadius > 0) {
			for (int i = 0; i < nbSides; i++) {
				triangles.push_back(0);
				triangles.push_back(((i + 1) % nbSides) + 1);
				triangles.push_back((i % nbSides) + 1);
			}
		}

		// Top cap
		if (topRadius > 0) {
			int firstIdx = bottomRadius > 0 ? nbSides + 1 : 1;
			for (int i = 0; i < nbSides; i++) {
				triangles.push_back(firstIdx);
				triangles.push_back((i % nbSides) + firstIdx + 1);
				triangles.push_back(((i + 1) % nbSides) + firstIdx + 1);
			}
		}

		// Sides 
		if (topRadius > 0 && bottomRadius > 0) {
			for (int i = 0; i < nbSides; i++) {
				vector<unsigned> tri = quadToTri(
					(i % nbSides) + 1,
					((i + 1) % nbSides) + 1,
					((i + 1) % nbSides) + 1 + nbSides + 1,
					(i % nbSides) + 1 + nbSides + 1
				);
				triangles.insert(triangles.end(), tri.begin(), tri.end());
			}
		}
		else if (topRadius > 0) {
			int firstIdx = 1;
			for (int i = 0; i < nbSides; i++) {
				triangles.push_back(0);
				triangles.push_back(((i + 1) % nbSides) + firstIdx + 1);
				triangles.push_back((i % nbSides) + firstIdx + 1);
			}
		}
		else if (bottomRadius > 0) {
			int firstIdx = 0;
			for (int i = 0; i < nbSides; i++) {
				triangles.push_back((i % nbSides) + firstIdx + 1);
				triangles.push_back(((i + 1) % nbSides) + firstIdx + 1);
				triangles.push_back(nbSides + 1);
			}
		}

		return triangles;
	}

	vector<unsigned> quadToTri(unsigned a, unsigned b, unsigned c, unsigned d) {
		return { a, b, c, c, d, a };
	}
};

