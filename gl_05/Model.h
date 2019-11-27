#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "shprogram.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class Model
{
public:
	vector<Mesh*> meshes;
	string directory;



	Model()
	{

	}

	Model(vector<Mesh*> meshes)
	{
		this->meshes = meshes;
	}

	void AddMesh(Mesh* mesh)
	{
		meshes.push_back(mesh);
	}

	void AddModel(Model* model) {
		for (int i = 0; i < model->meshes.size(); i++) {
			meshes.push_back(model->meshes[i]);
		}
	}

	void Draw(ShaderProgram shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i]->Draw(shader);
	}	


	void translate(const glm::vec3& v) {
		for(int i = 0; i < meshes.size(); ++i)
			meshes[i]->translate(v);
	}

	void rotate(const glm::vec3& axis, float angle) {
		for (int i = 0; i < meshes.size(); ++i)
			meshes[i]->rotate(axis, angle);
	}

	void scale(const glm::vec3& scale) {
		for (int i = 0; i < meshes.size(); ++i)
			meshes[i]->scale(scale);
	}


	void transform(const glm::mat4& transformation)
	{
		for (int i = 0; i < meshes.size(); ++i)
			meshes[i]->transform(transformation);
	}

};

#endif
