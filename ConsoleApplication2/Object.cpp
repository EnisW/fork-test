#include "Object.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object::Object(std::vector<Vertex> data)
{
	modelMatrix = glm::mat4(1.0f);
	this->data = data;
	modelIndex = 0;
}

Object::Object(std::string& path)
{
		modelMatrix = glm::mat4(1.0f);
		loadData(path);
		modelIndex = 0;
}

Object::~Object()
{
}

void Object::setColor(glm::vec3 color)
{
	for (Vertex& v : data) {
		v.color = color;
	}
}

void Object::loadData(std::string& path){

	readVTN(path);
}

void Object::setTexture(std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
								GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
								GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "Texture format not supported" << std::endl;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glGenerateMipmap(GL_TEXTURE_2D);	
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}




void Object::readVT(std::string path) {

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> texCoords;
	std::vector<Index> indices;
	std::vector<Vertex> tempVertexArray;
	std::vector<unsigned int> tempIndexArray;

	std::fstream file(path);


	while (!file.eof()) {
		char line[256];
		glm::vec3 tempVertex;
		glm::vec3 tempUV;


		file.getline(line, 256);

		Index tempI[3];
		unsigned int i1, i2, i3;
		unsigned int t1, t2, t3;

		if (line[0] == 'v' && line[1] == ' ') {
			sscanf_s(line, "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertices.push_back(tempVertex);
		}

		if (line[0] == 'v' && line[1] == 't') {
			sscanf_s(line, "vt %f %f", &tempUV.x, &tempUV.y);
			texCoords.push_back(tempUV);
		}


		if (line[0] == 'f' && line[1] == ' ') {
			sscanf_s(line, "f %u/%u %u/%u %u/%u", &i1, &t1, &i2, &t2, &i3, &t3);
			tempI[0].vertex = i1 - 1;
			tempI[1].vertex = i2 - 1;
			tempI[2].vertex = i3 - 1;
			tempI[0].texture = t1 - 1;
			tempI[1].texture = t2 - 1;
			tempI[2].texture = t3 - 1;
			indices.push_back(tempI[0]);
			indices.push_back(tempI[1]);
			indices.push_back(tempI[2]);

		}


	}

	for (Index x : indices) {
		Vertex v;
		unsigned int vPos = 0;
		v.pos = vertices[x.vertex];
		v.texCoord = texCoords[x.texture];

		bool found = false;
		for (; vPos < tempVertexArray.size(); vPos++) {
			if (v.pos == tempVertexArray[vPos].pos && v.texCoord == tempVertexArray[vPos].texCoord) {
				found = true;
				break;
			}
		}
		if (found) {
			tempIndexArray.push_back(vPos);
		}
		else {
			tempVertexArray.push_back(v);
			tempIndexArray.push_back(tempVertexArray.size() - 1);
		}
	}

	this->data = tempVertexArray;
	this->indices = tempIndexArray;

	file.close();

}

void Object::readVTN(std::string path)
{

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<Index> indices;
	std::vector<Vertex> tempVertexArray;
	std::vector<unsigned int> tempIndexArray;
	std::vector<glm::vec3> tempNormalArray;
	std::fstream file(path);


	while (!file.eof()) {
		char line[256];
		glm::vec3 tempVertex;
		glm::vec3 tempUV;
		glm::vec3 tempNormal;

		file.getline(line, 256);

		Index tempI[3];
		unsigned int i1, i2, i3;
		unsigned int t1, t2, t3;
		unsigned int n1, n2, n3;

		if (line[0] == 'v' && line[1] == ' ') {
			sscanf_s(line, "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertices.push_back(tempVertex);
		}

		if (line[0] == 'v' && line[1] == 't') {
			sscanf_s(line, "vt %f %f", &tempUV.x, &tempUV.y);
			texCoords.push_back(tempUV);
		}

		if (line[0] == 'v' && line[1] == 'n') {
			sscanf_s(line, "vn %f %f %f", &tempNormal.x, &tempNormal.y, &tempNormal.z);
			normals.push_back(tempNormal);
		}


		if (line[0] == 'f' && line[1] == ' ') {
			sscanf_s(line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &i1, &t1, &n1, &i2, &t2, &n2, &i3, &t3, &n3);
			tempI[0].vertex = i1 - 1;
			tempI[1].vertex = i2 - 1;
			tempI[2].vertex = i3 - 1;
			tempI[0].texture = t1 - 1;
			tempI[1].texture = t2 - 1;
			tempI[2].texture = t3 - 1;
			tempI[0].normal = n1 - 1;
			tempI[1].normal = n2 - 1;
			tempI[2].normal = n3 - 1;

			indices.push_back(tempI[0]);
			indices.push_back(tempI[1]);
			indices.push_back(tempI[2]);

		}


	}

	for (Index x : indices) {
		Vertex v;
		unsigned int vPos = 0;
		v.pos = vertices[x.vertex];
		v.texCoord = texCoords[x.texture];
		v.normal = normals[x.normal];
		bool found = false;
		for (; vPos < tempVertexArray.size(); vPos++) {
			if (v.pos == tempVertexArray[vPos].pos && v.texCoord == tempVertexArray[vPos].texCoord && v.normal == tempVertexArray[vPos].normal) {
				found = true;
				break;
			}
		}
		if (found) {
			tempIndexArray.push_back(vPos);
		}
		else {
			tempVertexArray.push_back(v);
			tempIndexArray.push_back(tempVertexArray.size() - 1);
		}
	}


	this->data = tempVertexArray;
	this->indices = tempIndexArray;

	file.close();
}
