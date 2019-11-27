#pragma once
#include "Mesh.h"
#include <cmath>

class BoatBody : public Mesh
{
public:
	float width;
	float length;
	float height;
	float bowLength;
	int precision;

	BoatBody(float width = 2, float length = 11, float height = 1.2, float bowLength = 3, int precision = 50): width(width), length(length), height(height), bowLength(bowLength), precision(precision)
	{
		vector<glm::vec3> v = getVertices();
		vector<unsigned> t = getIndices();
		vector<Vertex> verts;

		for (int i = 0; i < v.size(); i++) {
			verts.push_back(Vertex(v[i], glm::vec3(0.35f, 0.16f, 0.14f), glm::vec2(0.0f), glm::vec3(0.1f)));
		}

		setVertices(verts);
		setIndices(t);
		recalculateNormales();
	}


private:
	vector<glm::vec3> getVertices()
	{
		vector<glm::vec3> vertices;
		//pushing vertices of front parabola upper line
		for (int i = 0; i < precision; i++) {
			float x = -width + 2.0 * width * i / (precision - 1);
			float y = x * x*height / (width*width);
			vertices.push_back(glm::vec3(x, height, 0));
		}
		//back parabola
		for (int i = 0; i < precision; i++) {
			float x = -width + 2.0 * width * i / (precision - 1);
			float y = x * x*height / (width*width);
			vertices.push_back(glm::vec3(x, y, 0));
		}
		//front parabola
		for (int i = 0; i < precision; i++) {
			float x = -width + 2.0 * width * i / (precision - 1);
			float y = x * x*height / (width*width);
			vertices.push_back(glm::vec3(x, y, length));
		}

		//bow vertice
		vertices.push_back((glm::vec3(0, height, length + bowLength)));

		return vertices;
	}


	vector<unsigned> getIndices()
	{
		vector<unsigned> triangles;

		//generating back wall triangles
		for (int i = 0; i < precision - 3; ++i)
		{
			triangles.push_back(i + 1);
			triangles.push_back(i + precision + 1);
			triangles.push_back(i + 2);

			triangles.push_back(i + precision + 1);
			triangles.push_back(i + 2);
			triangles.push_back(i + precision + 2);
		}

		//first and last triangle
		triangles.push_back(0);
		triangles.push_back(1);
		triangles.push_back(precision + 1);

		triangles.push_back(precision - 1);
		triangles.push_back(precision - 2);
		triangles.push_back(precision * 2 - 2);

		//generating sides triangles

		for (int i = 0; i < precision - 1; ++i)
		{
			triangles.push_back(i + precision);
			triangles.push_back(i + precision + 1);
			triangles.push_back(i + precision + precision);

			triangles.push_back(i + precision + precision);
			triangles.push_back(i + precision + precision + 1);
			triangles.push_back(i + precision + 1);
		}

		//generating bow
		for (int i = 0; i < precision - 1; ++i)
		{
			triangles.push_back(i + precision + precision);
			triangles.push_back(i + precision + precision + 1);
			triangles.push_back(precision + precision + precision);
		}

		//closing shape
		triangles.push_back(0);
		triangles.push_back(precision+precision);
		triangles.push_back(precision+precision - 1);

		triangles.push_back(precision+precision-1);
		triangles.push_back(precision + precision + precision - 1);
		triangles.push_back(precision + precision);

		triangles.push_back(precision + precision + precision);
		triangles.push_back(precision + precision + precision - 1);
		triangles.push_back(precision + precision);

		return triangles;
	}


};

