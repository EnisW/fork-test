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

	void addGround(GLuint programID);
	
private:
	GLuint programID;
	GLuint programID2;
	GLuint programGround;
	GLuint matrixID;
	GLuint matrixID2;
	GLuint matrixIDGround;

	GLuint viewPosID;
	GLuint viewPosIDGround;

	glm::mat4 MVP;
	GLFWwindow* window;
	glm::vec3 position;

	float horizontalAngle;
	float verticalAngle;
	float lastTime;


	float speed; 
	float mouseSpeed;


};
