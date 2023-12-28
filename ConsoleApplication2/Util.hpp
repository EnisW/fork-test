#pragma once
// Include standard headers
#include <stdio.h>
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



typedef struct {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;
	GLuint modelIndex;
} Vertex;
