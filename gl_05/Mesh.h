#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shprogram.h"
#include "Object3D.h"

using namespace std;

struct Vertex {
	Vertex(glm::vec3 Position, glm::vec3 Color, glm::vec2 TexCoords, glm::vec3 Normal) : Position(Position), Color(Color), TexCoords(TexCoords), Normal(Normal) {}

	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoords;
	glm::vec3 Normal;
};

struct Texture {
	unsigned int id;
	string path;

	Texture() {}
	Texture(string path) : path(path) {}
};

class Mesh : public Object3D {
protected:
	unsigned int VAO;
	vector<Vertex> vertices;
	Texture texture;
	vector<unsigned int> indices;
	
	
public:

	~Mesh()
	{
		//not sure if it is right
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Draw(ShaderProgram shader)
	{
		shader.setMat4Uniform("model", model); //move object to desired place
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.id); //choose texture
		glBindVertexArray(VAO); //choose vertices
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}


protected:
	Mesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	void setVertices(vector<Vertex> v) {
		vertices = v;

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glBindVertexArray(0);
	}

	void setIndices(vector<unsigned int> i) {
		indices = i;

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	void setTexture(Texture texture) {
		this->texture = texture;

		LoadMipmapTexture(texture.path.c_str());
	}

	void recalculateNormales() {
		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].Normal = glm::vec3(0.0f);
		}

		for(int i = 0; i < indices.size() / 3; i++) {
			int a = indices[i * 3];
			int b = indices[i * 3 + 1];
			int c = indices[i * 3 + 2];

			glm::vec3 faceVector = glm::cross(vertices[c].Position - vertices[a].Position, vertices[b].Position - vertices[a].Position);

			vertices[a].Normal += faceVector;
			vertices[b].Normal += faceVector;
			vertices[c].Normal += faceVector;
		}

		for (int i = 0; i < vertices.size(); i++) {
			vertices[i].Normal = glm::normalize(vertices[i].Normal);
		}

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

private:
	unsigned int VBO, EBO;

	GLuint LoadMipmapTexture(const char* fname)
	{
		int width, height;
		unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
		if (image == nullptr)
			throw exception("Failed to load texture file");

		GLuint textureId;
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		texture.id = textureId;
		return textureId;
	}

};
#endif
