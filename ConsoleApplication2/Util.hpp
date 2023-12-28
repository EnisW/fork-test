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
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


struct _Vertex{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	GLfloat modelIndex;
	
} ;
typedef struct _Vertex Vertex;


typedef struct {

	unsigned int vertex;
	unsigned int texture;
} Index;