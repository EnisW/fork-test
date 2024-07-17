#include "Object.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Object::Object(std::vector<Vertex> data)
{
	physicsEnabled = true;
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	thrustForce = glm::vec3(0, 0, 0);
	size = glm::vec3(0, 0, 0);
	modelMatrix = glm::mat4(1.0f);
	modelIndex = 0;
	objScale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->data = data;

}

Object::Object(std::string& path)
{
	physicsEnabled = true;
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	thrustForce = glm::vec3(0, 0, 0);
	size = glm::vec3(0, 0, 0);
	modelMatrix = glm::mat4(1.0f);
	modelIndex = 0;
	objScale = glm::vec3(1.0f, 1.0f, 1.0f);

	loadData(path);
}
Object::Object(std::string& path, int a)
{

	name = "";
	physicsEnabled = true;
	position = glm::vec3(0, 0, 0);
	velocity = glm::vec3(0, 0, 0);
	acceleration = glm::vec3(0, 0, 0);
	thrustForce = glm::vec3(0, 0, 0);
	size = glm::vec3(0, 0, 0);
	modelMatrix = glm::mat4(1.0f);
	modelIndex = 0;
	objScale = glm::vec3(1.0f, 1.0f, 1.0f);

	if(a == FILE_VT)
		readVT(path);
	else if(a == FILE_VTN)
		readVTN(path);
	else if(a == FILE_VN)
		readVN(path);
	else if(a == FILE_FLAT)
		readFlat(path);
	else
		loadData(path);
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

void Object::loadDataVN(std::string& path) {

	readVN(path);
}

void Object::setTexture(std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glActiveTexture(GL_TEXTURE0 + modelIndex);
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
			sscanf(line, "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertices.push_back(tempVertex);
		}

		if (line[0] == 'v' && line[1] == 't') {
			sscanf(line, "vt %f %f", &tempUV.x, &tempUV.y);
			texCoords.push_back(tempUV);
		}


		if (line[0] == 'f' && line[1] == ' ') {
			sscanf(line, "f %u/%u %u/%u %u/%u", &i1, &t1, &i2, &t2, &i3, &t3);
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

	glm::vec3 sizeL = glm::vec3(0, 0, 0);
	glm::vec3 sizeB = glm::vec3(0, 0, 0);

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
			sscanf(line, "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertices.push_back(tempVertex);
			if (tempVertex.x > sizeB.x)
				sizeB.x = tempVertex.x;
			if (tempVertex.y > sizeB.y)
				sizeB.y = tempVertex.y;
			if (tempVertex.z > sizeB.z)
				sizeB.z = tempVertex.z;


			if (tempVertex.x < sizeL.x)
				sizeL.x = tempVertex.x;
			if (tempVertex.y < sizeL.y)
				sizeL.y = tempVertex.y;
			if (tempVertex.z < sizeL.z)
				sizeL.z = tempVertex.z;
		}

		if (line[0] == 'v' && line[1] == 't') {
			sscanf(line, "vt %f %f", &tempUV.x, &tempUV.y);
			texCoords.push_back(tempUV);
		}

		if (line[0] == 'v' && line[1] == 'n') {
			sscanf(line, "vn %f %f %f", &tempNormal.x, &tempNormal.y, &tempNormal.z);
			normals.push_back(tempNormal);
		}


		if (line[0] == 'f' && line[1] == ' ') {
			sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &i1, &t1, &n1, &i2, &t2, &n2, &i3, &t3, &n3);
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
	int a = 0;
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
			a++;
			tempIndexArray.push_back(vPos);
			if(a % 100 == 0)
				std::cout << indices.size() << "/" << a << std::endl;
		}
		else {
			a++;

			tempVertexArray.push_back(v);
			tempIndexArray.push_back(tempVertexArray.size() - 1);
			if (a % 100 == 0)
				std::cout << indices.size() << "/" << a << std::endl;
		}
	}

	size.x = (sizeB.x - sizeL.x) * objScale.x;
	size.y = (sizeB.y - sizeL.y) * objScale.y;
	size.z = (sizeB.z - sizeL.z) * objScale.z;
	this->data = tempVertexArray;
	this->indices = tempIndexArray;

	file.close();
}

void Object::readVN(std::string path)
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
		
		unsigned int n1, n2, n3;

		if (line[0] == 'v' && line[1] == ' ') {
			sscanf(line, "v %f %f %f", &tempVertex.x, &tempVertex.y, &tempVertex.z);
			vertices.push_back(tempVertex);
			if (abs(tempVertex.x) > size.x)
				size.x = abs(tempVertex.x);
			if (abs(tempVertex.y) > size.y)
				size.y = abs(tempVertex.y);
			if (abs(tempVertex.z) > size.z)
				size.z = abs(tempVertex.z);
		}

	
		if (line[0] == 'v' && line[1] == 'n') {
			sscanf(line, "vn %f %f %f", &tempNormal.x, &tempNormal.y, &tempNormal.z);
			normals.push_back(tempNormal);
		}


		if (line[0] == 'f' && line[1] == ' ') {
			sscanf(line, "f %u//%u %u//%u %u//%u", &i1, &n1, &i2, &n2, &i3, &n3);
			tempI[0].vertex = i1 - 1;
			tempI[1].vertex = i2 - 1;
			tempI[2].vertex = i3 - 1;
			tempI[0].texture = 0;
			tempI[1].texture = 0;
			tempI[2].texture = 0;
			tempI[0].normal = n1 - 1;
			tempI[1].normal = n2 - 1;
			tempI[2].normal = n3 - 1;

			indices.push_back(tempI[0]);
			indices.push_back(tempI[1]);
			indices.push_back(tempI[2]);

		}
	

	}
	int a = 0;
	for (Index x : indices) {
		Vertex v;
		unsigned int vPos = 0;
		v.pos = vertices[x.vertex];
		v.normal = normals[x.normal];
		bool found = false;
		for (; vPos < tempVertexArray.size(); vPos++) {
			if (v.pos == tempVertexArray[vPos].pos && v.normal == tempVertexArray[vPos].normal) {
				found = true;
				break;
			}
		}
		if (found) {
			a++;
			tempIndexArray.push_back(vPos);
			if (a % 100 == 0)
				std::cout << indices.size() << "/" << a << std::endl;
		}
		else {
			a++;
			tempVertexArray.push_back(v);
			tempIndexArray.push_back(tempVertexArray.size() - 1);
			if (a % 100 == 0)
				std::cout << indices.size() << "/" << a << std::endl;

		}
	}


	this->data = tempVertexArray;
	this->indices = tempIndexArray;

	file.close();
}

void Object::readFlat(std::string path)
{
	std::fstream file(path);
	std::vector<Vertex> tempVertexArray;
	std::vector<unsigned int> tempIndexArray;
	glm::vec3 sizeL = glm::vec3(0, 0, 0);
	glm::vec3 sizeB = glm::vec3(0, 0, 0);
	while (!file.eof()) {
		char line[256];
		Vertex tempVertex;

		file.getline(line, 256);

		if (line[0] == 'v') {
			sscanf(line, "v %f %f %f %f %f %f %f %f", &tempVertex.pos.x, &tempVertex.pos.y, &tempVertex.pos.z, &tempVertex.texCoord.x, &tempVertex.texCoord.y, &tempVertex.normal.x, &tempVertex.normal.y, &tempVertex.normal.z);
			tempVertexArray.push_back(tempVertex);
			
			if(tempVertex.pos.x > sizeB.x)
				sizeB.x = tempVertex.pos.x;
			if(tempVertex.pos.y > sizeB.y)
				sizeB.y = tempVertex.pos.y;
			if(tempVertex.pos.z > sizeB.z)
				sizeB.z = tempVertex.pos.z;


			if (tempVertex.pos.x < sizeL.x)
				sizeL.x = tempVertex.pos.x;
			if (tempVertex.pos.y < sizeL.y)
				sizeL.y = tempVertex.pos.y;
			if (tempVertex.pos.z < sizeL.z)
				sizeL.z = tempVertex.pos.z;



		}
		else if (line[0] == 'i') {
			unsigned int i;
			sscanf(line, "i %u", &i);
			tempIndexArray.push_back(i);
		}
	}

	this->data = tempVertexArray;
	this->indices = tempIndexArray;
	size.x = (sizeB.x - sizeL.x) * objScale.x;
	size.y = (sizeB.y - sizeL.y) * objScale.y;
	size.z = (sizeB.z - sizeL.z) * objScale.z;
}
