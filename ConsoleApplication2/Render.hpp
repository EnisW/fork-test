#pragma once

#include "Util.hpp"
#include "Object.hpp"
#include "Camera.hpp"

class Renderer {


public:
	Renderer(GLuint programID);
	~Renderer();

	bool addObject(Object* object);
	void removeObject(Object* object);
	void addObjectToQueue(Object* object);

	void render();

	bool textureEnabled;

private:

	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
	GLuint screenHeight, screenWidth;
	glm::mat4 lightProjection, lightView;

	
	
	GLuint shadowMapFBO;
	GLuint shadowMap;
	GLuint shadowShaderProgram;

	std::vector<Object*> objects;
	std::vector<Object*> objectQueue;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;

	std::vector<glm::mat4> modelMatrices;
	glm::vec3 lightPos;

	GLuint vertexbuffer;
	GLuint elementBuffer;
	GLuint VertexArrayID;

	GLuint modelUniformID;
	GLuint lightUniformID;
	GLuint programID;

	std::vector<bool> modelMatrixUsed;
};