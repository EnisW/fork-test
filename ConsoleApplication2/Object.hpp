#pragma once

#include "Util.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
class Object
{

public:
	Object(std::vector<float> data);
	Object(std::string& path);
	~Object();

	void setData(std::vector<float> data) {
		this->data = data;
	};

	void loadData(std::string& path);

	std::vector<float> getData() {
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

	

	unsigned int modelIndex;
	std::vector<float> data;
	std::vector<unsigned int> indices;
	unsigned int elementBufferBias;
	
	unsigned char* textureData;
	GLuint textureID;

	private:
		glm::mat4 modelMatrix;
		void convertData(std::string path);
		unsigned int getVertexPos(std::vector<Vertex>& vertices, Vertex vertex);

};