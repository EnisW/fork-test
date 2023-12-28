#pragma once
#include "Util.hpp"

class Camera
{

public:
	Camera(GLuint, GLuint,GLFWwindow*);
	~Camera();


	void update();
	void setMVP(glm::mat4 MVP) {
		this->MVP = MVP;
	};
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		double temp = -(5 * yoffset);
		if(initialFoV + temp > 10.0f && initialFoV + temp < 45.0f)
			initialFoV += temp;

	}
	
private:
	GLuint programID;
	GLuint programID2;
	GLuint matrixID;
	GLuint matrixID2;

	glm::mat4 MVP;
	GLFWwindow* window;
	glm::vec3 position;

	float horizontalAngle;
	float verticalAngle;
	float lastTime;


	float speed; 
	float mouseSpeed;


};
