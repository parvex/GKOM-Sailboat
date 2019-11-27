#pragma once
#include "Model.h"
#include "Cylinder.h"

class SteeringWheel : public Model {
private:
	Cylinder wheel[6] = { Cylinder(1.5, 0.05),Cylinder(1.5, 0.05),Cylinder(1.5, 0.05),Cylinder(1.5, 0.05),Cylinder(1.5, 0.05),Cylinder(1.5, 0.05) };
	Cylinder handle[6] = { Cylinder(2, 0.05),Cylinder(2, 0.05),Cylinder(2, 0.05),Cylinder(2, 0.05),Cylinder(2, 0.05),Cylinder(2, 0.05) };
	Cylinder connector = Cylinder(0.3, 0.1);
public:
	SteeringWheel() {
		float currentDegrees = -180.0f;
		
		connector.rotate(glm::vec3(0, 0, 1), glm::radians(90.0f));
		AddMesh(&connector);

		for (int i = 0; i < 6; i++) {
			handle[i].rotate(glm::vec3(1.0, 0, 0), glm::radians(currentDegrees));
			handle[i].translate(glm::vec3(0, 0.375, 0));

			wheel[i].rotate(glm::vec3(1.0, 0, 0), glm::radians(currentDegrees));
			wheel[i].translate(glm::vec3(0, 0, -0.675));

			AddMesh(&handle[i]);
			AddMesh(&wheel[i]);

			currentDegrees += 60.0f;
		}

		

		glm::mat4 transformMoveSteeringWheel = glm::mat4();
		glm::mat4 transformRotateSteeringWheel = glm::mat4();

		transformRotateSteeringWheel = glm::rotate(transformRotateSteeringWheel, glm::radians(90.0f), glm::vec3(0, 1, 0));
		transform(transformRotateSteeringWheel);



		transformMoveSteeringWheel = glm::translate(transformMoveSteeringWheel, glm::vec3(3.0, 2.5, 6.1));
		transform(transformMoveSteeringWheel);


		scale(glm::vec3(0.5f));
	}
	
};

