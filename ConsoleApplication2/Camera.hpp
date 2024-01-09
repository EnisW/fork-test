#pragma once
#include "Util.hpp"

class Camera
{

public:
	Camera(GLFWwindow*);
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

	void addProgram(GLuint programID);
	void addTextShader(GLuint programID);



private:
	

	glm::mat4 MVP;
	GLFWwindow* window;
	glm::vec3 position;
	
	GLuint textProgramID;
	GLuint textMatrixID;

	float horizontalAngle;
	float verticalAngle;
	float lastTime;

	std::vector<GLuint> programs;
	std::vector<GLuint> matrixIDs;
	std::vector<GLuint> viewPosIDs;

	float speed; 
	float mouseSpeed;


};
