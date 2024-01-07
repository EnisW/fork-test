#pragma once
// Include standard headers
#pragma comment(lib, "Ws2_32.lib")
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define MAX_OBJECT 32

#define FILE_VN 0
#define FILE_VTN 1
#define FILE_VT 2
#define FILE_FLAT 3

#define CONSOLE_PORT 8090


struct _Vertex{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texCoord;
	GLfloat modelIndex;
	
} ;
typedef struct _Vertex Vertex;

#define WIDTH 1680
#define HEIGHT 960


typedef struct Character {
	int codePoint, x, y, width, height, originX, originY;
} Character;

typedef struct {
	float topLeftX, topLeftY, topRightX, topRightY, bottomLeftX, bottomLeftY, bottomRightX, bottomRightY;
}charUV;

typedef struct Font {
	const char* name;
	int size, bold, italic, width, height, characterCount;
	Character* characters;
} Font;

static Character characters_Arial[] = {
  {' ', 237, 106, 3, 3, 1, 1},
  {'!', 120, 81, 7, 25, -1, 24},
  {'"', 148, 106, 11, 10, 0, 24},
  {'#', 284, 31, 20, 25, 1, 24},
  {'$', 108, 0, 18, 29, 0, 25},
  {'%', 183, 0, 28, 25, 0, 24},
  {'&', 92, 31, 22, 25, 0, 24},
  {'\'', 159, 106, 6, 10, 0, 24},
  {'(', 56, 0, 10, 31, -1, 24},
  {')', 66, 0, 10, 31, -1, 24},
  {'*', 135, 106, 13, 12, 0, 24},
  {'+', 84, 106, 18, 17, 0, 20},
  {',', 165, 106, 6, 10, -1, 4},
  {'-', 199, 106, 11, 5, 0, 11},
  {'.', 210, 106, 5, 5, -2, 4},
  {'/', 76, 81, 11, 25, 1, 24},
  {'0', 59, 56, 18, 25, 0, 24},
  {'1', 87, 81, 11, 25, -2, 24},
  {'2', 239, 56, 17, 25, 0, 24},
  {'3', 77, 56, 18, 25, 0, 24},
  {'4', 40, 56, 19, 25, 1, 24},
  {'5', 95, 56, 18, 25, 0, 24},
  {'6', 113, 56, 18, 25, 0, 24},
  {'7', 131, 56, 18, 25, 0, 24},
  {'8', 149, 56, 18, 25, 0, 24},
  {'9', 167, 56, 18, 25, 0, 24},
  {':', 43, 106, 5, 19, -2, 18},
  {';', 142, 81, 6, 24, -1, 18},
  {'<', 48, 106, 18, 18, 0, 20},
  {'=', 117, 106, 18, 12, 0, 17},
  {'>', 66, 106, 18, 18, 0, 20},
  {'?', 185, 56, 18, 25, 0, 24},
  {'@', 0, 0, 33, 31, 0, 24},
  {'A', 261, 0, 24, 25, 1, 24},
  {'B', 304, 31, 20, 25, -1, 24},
  {'C', 309, 0, 23, 25, 0, 24},
  {'D', 114, 31, 22, 25, -1, 24},
  {'E', 0, 56, 20, 25, -1, 24},
  {'F', 203, 56, 18, 25, -1, 24},
  {'G', 285, 0, 24, 25, 0, 24},
  {'H', 158, 31, 21, 25, -1, 24},
  {'I', 127, 81, 5, 25, -2, 24},
  {'J', 49, 81, 15, 25, 0, 24},
  {'K', 136, 31, 22, 25, -1, 24},
  {'L', 256, 56, 17, 25, -1, 24},
  {'M', 211, 0, 25, 25, -1, 24},
  {'N', 179, 31, 21, 25, -1, 24},
  {'O', 236, 0, 25, 25, 0, 24},
  {'P', 20, 56, 20, 25, -1, 24},
  {'Q', 126, 0, 25, 26, 0, 24},
  {'R', 0, 31, 23, 25, -1, 24},
  {'S', 200, 31, 21, 25, 0, 24},
  {'T', 221, 31, 21, 25, 1, 24},
  {'U', 242, 31, 21, 25, -1, 24},
  {'V', 23, 31, 23, 25, 1, 24},
  {'W', 151, 0, 32, 25, 1, 24},
  {'X', 46, 31, 23, 25, 1, 24},
  {'Y', 69, 31, 23, 25, 1, 24},
  {'Z', 263, 31, 21, 25, 1, 24},
  {'[', 76, 0, 9, 31, -1, 24},
  {'\\', 98, 81, 11, 25, 1, 24},
  {']', 85, 0, 9, 31, 1, 24},
  {'^', 102, 106, 15, 15, 0, 24},
  {'_', 215, 106, 22, 4, 2, -3},
  {'`', 190, 106, 9, 6, 0, 24},
  {'a', 198, 81, 18, 19, 0, 18},
  {'b', 273, 56, 17, 25, -1, 24},
  {'c', 306, 81, 17, 19, 0, 18},
  {'d', 290, 56, 17, 25, 0, 24},
  {'e', 216, 81, 18, 19, 0, 18},
  {'f', 64, 81, 12, 25, 1, 24},
  {'g', 307, 56, 17, 25, 0, 18},
  {'h', 17, 81, 16, 25, -1, 24},
  {'i', 132, 81, 5, 25, -1, 24},
  {'j', 94, 0, 9, 31, 3, 24},
  {'k', 33, 81, 16, 25, -1, 24},
  {'l', 137, 81, 5, 25, -1, 24},
  {'m', 148, 81, 25, 19, -1, 18},
  {'n', 323, 81, 16, 19, -1, 18},
  {'o', 234, 81, 18, 19, 0, 18},
  {'p', 324, 56, 17, 25, -1, 18},
  {'q', 0, 81, 17, 25, 0, 18},
  {'r', 32, 106, 11, 19, -1, 18},
  {'s', 0, 106, 16, 19, 0, 18},
  {'t', 109, 81, 11, 25, 1, 24},
  {'u', 16, 106, 16, 19, -1, 18},
  {'v', 252, 81, 18, 19, 1, 18},
  {'w', 173, 81, 25, 19, 1, 18},
  {'x', 270, 81, 18, 19, 1, 18},
  {'y', 221, 56, 18, 25, 1, 18},
  {'z', 288, 81, 18, 19, 1, 18},
  {'{', 45, 0, 11, 31, 0, 24},
  {'|', 103, 0, 5, 31, -2, 24},
  {'}', 33, 0, 12, 31, 1, 24},
  {'~', 171, 106, 19, 7, 0, 15},
};

typedef struct{
	std::string text;
	int x, y;


}Text;



static Font font_Arial = { "Arial", 32, 0, 0, 341, 125, 95, characters_Arial };
typedef struct {

	unsigned int vertex;
	unsigned int texture;
	unsigned int normal;
} Index;

