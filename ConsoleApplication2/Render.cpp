#include "Render.hpp"
#define VERTEX_SIZE 7
Renderer::Renderer(GLuint programID)
{

	modelMatrices.resize(100);
	modelMatrixUsed.resize(100);
	for (int i = 0; i < 100; i++) {
		modelMatrixUsed[i] = false;
	}


	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 7,
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 7,
		(void*)(sizeof(float) * 3)
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 7,
		(void*)(sizeof(float) * 6)
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	this->programID = programID;
	modelUniformID = glGetUniformLocation(programID, "M");
}

Renderer::~Renderer()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}

void Renderer::addObject(Object* object)
{
		objects.push_back(object);

		

		for (int i = 0; i < 100; i++) {
			if (modelMatrixUsed[i] == false) {
				object->modelIndex = i;
				modelMatrices[i] = object->getModelMatrix();
				modelMatrixUsed[i] = true;
				break;
			}
		}

		for (int i = 6; i < object->data.size(); i += 7) {
			object->data[i].modelIndex += object->modelIndex;
		}
		unsigned int bias = this->vertices.size() / VERTEX_SIZE;

		std::vector<Vertex> data = object->getData();
		for (int i = 0; i < data.size(); i++) {
			vertices.push_back(data[i]);
		}
		std::vector<unsigned int> indicies_t = object->getIndices();
		object->elementBufferBias = bias;
		for (int i = 0; i < indicies_t.size(); i++) {
			this->indicies.push_back(indicies_t[i]+ bias);
		}


		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indicies.size(), &this->indicies[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);




}

void Renderer::removeObject(Object* object)
{
	//TODO: decrease indices of objects after the removed one

	/*
	objects.erase(std::remove(objects.begin(), objects.end(), object), objects.end());

	std::vector<Vertex> data = object->getData(); // DONT USE ITERATORS, DO IT YOURSELF
	for (int i = 0; i < data.size(); i++) {
		this->vertices.erase(std::remove(this->vertices.begin(), this->vertices.end(), data[i]), this->vertices.end());
	}

	std::vector<unsigned int> indicies_t = object->getIndices();
	for (int i = 0; i < indicies_t.size(); i++) {
		this->indicies.erase(std::remove(this->indicies.begin(), this->indicies.end(), indicies_t[i]), this->indicies.end());
	}


	modelMatrices.erase(std::remove(modelMatrices.begin(), modelMatrices.end(), object->getModelMatrix()), modelMatrices.end());
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicies.size(), &indicies[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
}

void Renderer::render()
{
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	glUniformMatrix4fv(modelUniformID, objects.size(), GL_FALSE, &modelMatrices[0][0][0]);

	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}
