#pragma once

#include "Util.hpp"


class Object
{

public:
	Object(std::vector<Vertex> data);
	Object(std::string& path);
	Object(std::string& path, int a);
	~Object();

	void setData(std::vector<Vertex> data) {
		this->data = data;
	};

	void setColor(glm::vec3 color);

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

	void loadDataVN(std::string& path);

	float modelIndex;
	std::vector<Vertex> data;
	std::vector<unsigned int> indices;
	unsigned int elementBufferBias;
	
	unsigned char* textureData;
	GLuint textureID;
	GLuint textureSlot;

	private:
		void readVT(std::string path);
		void readVTN(std::string path);
		void readVN(std::string path);
		void readFlat(std::string path);

		glm::mat4 modelMatrix;

};