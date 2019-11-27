#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum Direction {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};


class Camera
{
public:
	float AngleX;
	float AngleY;

	float MovementSpeed;
	float MouseSensitivity;

	glm::vec3 Position;
	//vectors defining camera controlling directions
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Top;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float AngleX = -70.0f, float AngleY = 0.0f) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.05f)
	{
		this->Position = position;
		this->AngleX = AngleX;
		this->AngleY = AngleY;
		Top = glm::vec3(0.0f, 1.0f, 0.0f);
		calculateNewCameraVectors();
	}

	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, glm::vec3(0.0f,1.0f,0.0f));
	}

	void UpdateFacingDirection(float xdifference, float ydifference)
	{
		xdifference *= MouseSensitivity;
		ydifference *= MouseSensitivity;
		AngleX += xdifference;
		AngleY += ydifference;

		if (AngleY > 89.0f) AngleY = 89.0f;
		if (AngleY < -89.0f) AngleY = -89.0f;

		calculateNewCameraVectors();
	}

	void Move(Direction direction, float deltaTime)
	{
		float normalizedMovementSpeed = MovementSpeed * deltaTime;
		if (direction == FORWARD) Position += Front * normalizedMovementSpeed;
		if (direction == BACKWARD) Position -= Front * normalizedMovementSpeed;
		if (direction == LEFT)  Position -= Right * normalizedMovementSpeed;
		if (direction == RIGHT) Position += Right * normalizedMovementSpeed;
		if (direction == TOP) Position += Top * normalizedMovementSpeed;
		if (direction == BOTTOM) Position -= Top * normalizedMovementSpeed;
	}


private:
	void calculateNewCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(AngleX)) * cos(glm::radians(AngleY));
		front.y = sin(glm::radians(AngleY));
		front.z = sin(glm::radians(AngleX)) * cos(glm::radians(AngleY));
		Front = glm::normalize(front);
		Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
	}
};
#endif