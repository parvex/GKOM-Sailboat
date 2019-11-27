#pragma once
#include "Box.h"
class SkyBox :
	public Box
{
public:
	SkyBox(float size = 1000.0f) : Box(size, size, size) {
	
	}
	~SkyBox() {}
	void Draw(ShaderProgram shader) override {
		glBindVertexArray(VAO); //choose vertices
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
};

