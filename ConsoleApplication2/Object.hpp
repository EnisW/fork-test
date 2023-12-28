#pragma once

#include "Util.hpp"


class Object
{

public:
	Object(std::vector<Vertex> data);
	Object(std::string& path);
	~Object();

	void setData(std::vector<Vertex> data) {
		this->data = data;
	};

	void loadData(std::string& path);

	std::vector<Vertex> getData() {
		return data;
	};

	std::vector<unsigned int> getIndices() {
		return indices;
	};

	void setTexture(std::string path);

	void move(glm::vec3 movement) {
		modelMatrix = glm::translate(modelMatrix, movement);
	};

	void setModelMatrix(glm::mat4 modelMatrix) {
		this->modelMatrix = modelMatrix;
	};

	glm::mat4 getModelMatrix() {
		return modelMatrix;
	};

	

	float modelIndex;
	std::vector<Vertex> data;
	std::vector<unsigned int> indices;
	unsigned int elementBufferBias;
	
	unsigned char* textureData;
	GLuint textureID;

	private:
		void readVT(std::string path);
		glm::mat4 modelMatrix;

};