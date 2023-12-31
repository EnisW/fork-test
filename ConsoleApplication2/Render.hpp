#pragma once

#include "Util.hpp"
#include "Object.hpp"
#include "Camera.hpp"

class Renderer {


public:
	Renderer(GLuint programID);
	~Renderer();

	void addObject(Object* object);
	void removeObject(Object* object);

	void render();

private:
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	GLuint screenHeight, screenWidth;
	glm::mat4 lightProjection, lightView;

	void updateShadowMap();
	void createShadowMap();
	
	GLuint shadowMapFBO;
	GLuint shadowMap;
	GLuint shadowShaderProgram;

	std::vector<Object*> objects;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;

	std::vector<glm::mat4> modelMatrices;

	GLuint vertexbuffer;
	GLuint elementBuffer;
	GLuint VertexArrayID;

	GLuint modelUniformID;
	GLuint programID;

	std::vector<bool> modelMatrixUsed;
};