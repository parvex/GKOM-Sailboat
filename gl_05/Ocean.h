#pragma once
#include "Object3D.h"
#include <vector>
#include <SOIL.h>


class Ocean :
	public Object3D
{
private:
	static constexpr unsigned FOURCC_DXT1 = 0x31545844;
	static constexpr unsigned FOURCC_DXT3 = 0x33545844;
	static constexpr unsigned FOURCC_DXT5 = 0x35545844;

	unsigned int VAO;
	unsigned int VBO, EBO;
	unsigned int perlintex;
	

	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

public:
	unsigned int skytex;

	Ocean() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		//vertices.push_back(glm::vec3(-25.0, 0.0f, -25.0));
		//vertices.push_back(glm::vec3(-25.0, 0.0f, 25.0));
		//vertices.push_back(glm::vec3(25.0, 0.0f, 25.0));
		//vertices.push_back(glm::vec3(25.0, 0.0f, -25.0));
		float length = 20;
		float width = 20;
		int resX = 4;
		int resZ = 4;
		for (int z = 0; z < resZ; z++)
		{
			// [ -length / 2, length / 2 ]
			float zPos = ((float)z / (resZ - 1) - .5f) * length;
			for (int x = 0; x < resX; x++)
			{
				// [ -width / 2, width / 2 ]
				float xPos = ((float)x / (resX - 1) - .5f) * width;
				vertices.push_back(glm::vec3(xPos, 0.0f, zPos));
			}
		}

		int nbFaces = (resX - 1) * (resZ - 1);
		int t = 0;
		for (int face = 0; face < nbFaces; face++)
		{
			// Retrieve lower left corner from face ind
			int i = face % (resX - 1) + (face / (resZ - 1) * resX);

			indices.push_back(i + resX);
			indices.push_back(i + 1);
			indices.push_back(i);

			indices.push_back(i + resX);
			indices.push_back(i + resX + 1);
			indices.push_back(i + 1);
		}

		/*	vertices = {
					glm::vec3(0.25f, 0.5f, 0.0f),
					glm::vec3(-0.75f, 0.5f, 0.0f),
					glm::vec3(-0.25f, -0.5f, 0.0f),
					glm::vec3(0.75f, -0.5f, 0.0f)
			};*/

	/*	indices = {
			0, 1, 2,
			2, 3, 0,
		};*/

		/*indices = {
			0, 1, 2,
			0, 2, 3,
		};*/

		setVertices();
		setIndices();

		perlintex = LoadMipmapTexture("res/perlin_noise.png");
		skytex = loadDDS("res/ocean_env.dds");

	}

	void Draw(ShaderProgram shader) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, perlintex);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skytex);

		float levelsize = (float)(512 >> 0);
		float scale = 5120.0 / levelsize;

		glm::mat4 localtraf = glm::scale(glm::mat4(1), glm::vec3(200.0f, 0, 200.0f));

		glm::vec4 uvparams = { 1.0 / 20, 1.0 / 512.0, -128.0, -128.0 };

		shader.setMat4Uniform("matLocal", localtraf);
		shader.setVec4Uniform("uvParams", uvparams);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	~Ocean() {

	}

	void setVertices() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

		glBindVertexArray(0);
	}

	void setIndices() {

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		glBindVertexArray(0);
	}

	GLuint loadDDS(const char * imagepath) {
		int width, height;
		//unsigned int image = SOIL_load_OGL_texture(imagepath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 64);
		unsigned int image = SOIL_load_OGL_single_cubemap(imagepath, SOIL_DDS_CUBEMAP_FACE_ORDER,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_POWER_OF_TWO
			| SOIL_FLAG_MIPMAPS
			//| SOIL_FLAG_COMPRESS_TO_DXT
			//| SOIL_FLAG_TEXTURE_REPEATS
			//| SOIL_FLAG_INVERT_Y
			| SOIL_FLAG_DDS_LOAD_DIRECT);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if (image == 0)
			throw std::exception("Failed to load texture.");

		return image;
	}

	GLuint LoadMipmapTexture(const char* fname)
	{
		int width, height;
		unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
		if (image == nullptr)
			throw std::exception("Failed to load texture file");

		GLuint textureId;
		glGenTextures(1, &textureId);

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 3);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		return textureId;
	}


};

