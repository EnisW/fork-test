#pragma once
// Include standard headers
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
static float initialFoV;

#define GLEW_STATIC
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>



// Include GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


#define MAX_OBJECT 32
#define FILE_VN 0
#define FILE_VTN 1
#define FILE_VT 2


struct _Vertex{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texCoord;
	GLfloat modelIndex;
	
} ;
typedef struct _Vertex Vertex;




typedef struct {

	unsigned int vertex;
	unsigned int texture;
	unsigned int normal;
} Index;

