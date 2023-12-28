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