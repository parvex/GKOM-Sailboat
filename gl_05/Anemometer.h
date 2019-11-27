#pragma once
#include "Model.h"
#include "Cylinder.h"

class Anemometer : public Model
{
private: 
	Cylinder anemometerHolder = Cylinder(1.2, 0.1);
	Cylinder handleArray[8] = { Cylinder(1.0, 0.05), Cylinder(1.0, 0.05), Cylinder(1.0, 0.05), Cylinder(1.0, 0.05), Cylinder(1.0, 0.05), Cylinder(1.0, 0.05),Cylinder(1.0, 0.05), Cylinder(1.0, 0.05) };
public:
	Anemometer() {

		anemometerHolder.translate(glm::vec3(0, 1, 0));
		AddMesh(&anemometerHolder);
		float currentDegrees = -180.0f;
		float degrees = 360.0f / 8;
		for (int i = 0; i < 8; i++) {

			handleArray[i].rotate(glm::vec3(0, 1.0, 0), glm::radians(currentDegrees));
			handleArray[i].rotate(glm::vec3(0, 0, 1.0), glm::radians(90.0f));

			currentDegrees += degrees;

			handleArray[i].translate(glm::vec3(1.5, 0.5, 0));

			AddMesh(&handleArray[i]);
		}

		//move Anemometer
		glm::mat4 transformMoveAnemometer = glm::mat4();
		transformMoveAnemometer = glm::translate(transformMoveAnemometer, glm::vec3(3.0f, 0.7f, 12.0f));
		transform(transformMoveAnemometer);
	}

};

