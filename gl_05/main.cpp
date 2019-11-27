#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;
#include "Camera.h"
#include "shprogram.h"
#include "Mesh.h"
#include "CustomMesh.h"
#include "Model.h"
#include "Box.h"
#include "Plane.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "Ocean.h"
#include "Skybox.h"
#include "BoatBody.h"
#include "Sail.h"
#include "Controller.h"
#include "Anemometer.h"
#include "SteeringWheel.h"


const GLuint WIDTH = 1024, HEIGHT = 576;

Camera camera(glm::vec3(-1.0f, 3.0f, -1.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;


// ocean
#define WIND_DIRECTION		{ -0.4f, -0.9f }

float oceancolors[][3] = {
	{ 0.0056f, 0.0194f, 0.0331f },	// deep blue
	{ 0.1812f, 0.4678f, 0.5520f },	// carribbean
	{ 0.0000f, 0.2307f, 0.3613f },	// light blue
	{ 0.2122f, 0.6105f, 1.0000f },
	{ 0.0123f, 0.3613f, 0.6867f },
	{ 0.0000f, 0.0999f, 0.4508f },
	{ 0.0000f, 0.0331f, 0.1329f },
	{ 0.0000f, 0.0103f, 0.0331f }
};
int currentcolor = 0;


void processKeyboard(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.Move(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.Move(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.Move(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.Move(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.Move(TOP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.Move(BOTTOM, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.UpdateFacingDirection(xoffset, yoffset);
}


GLuint LoadMipmapTexture(GLuint texId, const char* fname)
{
	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw exception("Failed to load texture file");

	GLuint texture;
	glGenTextures(1, &texture);

	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

ostream& operator<<(ostream& os, const glm::mat4& mx)
{
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
			cout << mx[row][col] << ' ';
		cout << endl;
	}
	return os;
}

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

GLuint			VAO;
GLuint			VBO;
GLuint			EBO;

int main()
{
	if (glfwInit() != GL_TRUE)
	{
		cout << "GLFW initialization failed" << endl;
		return -1;
	}
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);*/
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try
	{
		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GKOM - OpenGL 05", nullptr, nullptr);
		if (window == nullptr)
			throw exception("GLFW window not created");
		glfwMakeContextCurrent(window);

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);

		unsigned int depthMapFBO;
		glGenFramebuffers(1, &depthMapFBO);

		const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

		unsigned int depthMap;
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);



		// Build, compile and link shader program
		ShaderProgram theProgram("vertshader.vert", "fragshader.frag");
		ShaderProgram oceanProgram("ocean.vert", "ocean.frag");
		ShaderProgram skyProgram("sky.vert", "sky.frag");

		Ocean ocean;
		SkyBox skybox;
		//boat meshes
		BoatBody boatBody;
		Box wheelHolder(0.5, 2, 0.5);
		Cylinder boatMast(7, 0.1);
		Sail boatSail;
		Anemometer anemometer;
		SteeringWheel steeringWheel;
		Controller boatController;

		boatMast.translate(glm::vec3(0,4.5,4));
		boatSail.translate(glm::vec3(0, 3, 4));   
		wheelHolder.translate(glm::vec3(0, 1.6, 3.4));
		

		Model boat;
		boat.AddMesh(&boatMast);
		boat.AddMesh(&boatBody);
		boat.AddMesh(&boatSail);
		boat.AddMesh(&wheelHolder);
		boat.AddMesh(&boatController);


		boat.translate(glm::vec3(3, 0, 3));


		// During init, enable debug output
		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);


		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLuint LightID = glGetUniformLocation(theProgram.get_programID(), "LightPosition_worldspace");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_2D, depthMap);


		GLfloat rot_angle = 0.01f;
		glm::vec3 lightPos = glm::vec3(0.603, 0.240, -0.761);
		lightPos *= 300;
		//main event loop
		while (!glfwWindowShouldClose(window))
		{
			double currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)WIDTH / (float)HEIGHT, 1.0f, 2000.0f);
			glm::mat4 view = camera.GetViewMatrix();

			// render ocean
			float			flipYZ[16] = { 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 };
		
			glm::vec2		perlinoffset = { 0, 0};
			float			w[2] = WIND_DIRECTION;
			int				pattern[4];
			GLuint			subset = 0;


			perlinoffset[0] = -w[0] * currentFrame * 0.06f;
			perlinoffset[1] = -w[1] * currentFrame * 0.06f;

			glm::vec3 colors = {oceancolors[currentcolor][0], 
								oceancolors[currentcolor][1],
								oceancolors[currentcolor][2] };
			
			processKeyboard(window);

			// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
			glfwPollEvents();

			// Clear the colorbuffer
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			oceanProgram.Use();

			oceanProgram.setMat4Uniform("matViewProj", projection * view);
			oceanProgram.setMat4Uniform("matWorld", glm::mat4(1.0));
			oceanProgram.setVec2Uniform("perlinOffset", perlinoffset);
			oceanProgram.setVec3Uniform("eyePos", camera.Position);
			oceanProgram.setVec3Uniform("oceanColor", colors);


		
			ocean.Draw(oceanProgram);
				
			skyProgram.Use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, ocean.skytex);
			skyProgram.setMat4Uniform("matViewProj", projection * view);
			skyProgram.setMat4Uniform("matWorld", glm::translate(glm::mat4(1.0), camera.Position));
			skyProgram.setVec3Uniform("eyePos", camera.Position);
			skybox.Draw(skyProgram);


			theProgram.Use();

			// view/projection transformations
			theProgram.setMat4Uniform("projection", projection);
			theProgram.setMat4Uniform("view", view);

			//boat swimming comment if you don't want to have it
			glm::mat4 transformBoat = glm::mat4();
			rot_angle = 10 * deltaTime;
			transformBoat = glm::translate(transformBoat, glm::vec3(14.0f, 0.0f, 7.0f));
			transformBoat = glm::rotate(transformBoat, glm::radians(rot_angle), glm::vec3(0.0, 1.0, 0.0));

			transformBoat = glm::translate(transformBoat, glm::vec3(-14.0f, 0.0f, -7.0f));
			boat.transform(transformBoat);
			//boat swimming

			//anemometer swimming
			glm::mat4 transformMoveAnemometer = glm::mat4();

			transformMoveAnemometer = glm::translate(transformMoveAnemometer, glm::vec3(14.0f, 0.0f, 7.0f));
			transformMoveAnemometer = glm::rotate(transformMoveAnemometer, glm::radians(rot_angle), glm::vec3(0.0, 1.0, 0.0));
			transformMoveAnemometer = glm::translate(transformMoveAnemometer, glm::vec3(-14.0f, 0.0f, -7.0f));


			anemometer.transform(transformMoveAnemometer);
			//anemometer swimming

			//steering wheel swimming
			glm::mat4 transformMoveSteeringWheel = glm::mat4();
			transformMoveSteeringWheel = glm::translate(transformMoveSteeringWheel, glm::vec3(14.0f, 0.0f, 7.0f));
			transformMoveSteeringWheel = glm::rotate(transformMoveSteeringWheel, glm::radians(rot_angle), glm::vec3(0.0, 1.0, 0.0));
			transformMoveSteeringWheel = glm::translate(transformMoveSteeringWheel, glm::vec3(-14.0f, 0.0f, -7.0f));

			steeringWheel.transform(transformMoveSteeringWheel);
			//steering wheel swimming

			glm::mediump_float rad = glm::radians(sin(currentFrame / 34) * 360.0f);
			glm::vec4 lightPosModified(lightPos, 1);
			//lightPosModified = glm::rotate(glm::mat4(1), rad, glm::vec3(0, 1, 0)) * lightPosModified;
			glUniform3f(LightID, lightPosModified.x, lightPosModified.y, lightPosModified.z);
			

			glm::mediump_float boatRad = glm::radians(sin(currentFrame / 2) * 360.0f);

			boat.Draw(theProgram);
			anemometer.Draw(theProgram);
			steeringWheel.Draw(theProgram);

			glfwSwapBuffers(window);
		}
	}
	catch (exception ex)
	{
		cout << ex.what() << endl;
	}
	glfwTerminate();

	return 0;
}

