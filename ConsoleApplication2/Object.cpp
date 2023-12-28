#include "Object.hpp"

Object::Object(std::vector<float> data)
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

void Object::loadData(std::string& path){

	char line[256];
	unsigned int i1, i2, i3, t1, t2, t3;
	bool hasTexture = false;



	std::fstream file(path);
	if (file.fail()) {
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	while (file.is_open() && !file.eof()) {

		file.getline(line, 256);
		Vertex vertex;

		if (line[0] == 'v' && line[1] == ' ') {
			sscanf(line, "v %f %f %f", &vertex.pos.x, &vertex.pos.y, &vertex.pos.z);
		}
		if (line[0] == 'v' && line[1] == 't') {
			sscanf(line, "v %f %f", &vertex.texCoord.x, &vertex.texCoord.y);
			hasTexture = true;
		}


		if (line[0] == 'f' && line[1] == ' ' || hasTexture) {
			sscanf(line, "f %u/%u %u/%u %u/%u", &i1, &t1, &i2, &t2, &i3, &t3);
			indices.push_back(i1-1);
			indices.push_back(i2-1);
			indices.push_back(i3-1);
		}

		if (line[0] == 'f' && line[1] == ' ' || !hasTexture) {
			sscanf(line, "f %u %u %u", &i1, &i2, &i3);
			indices.push_back(i1 - 1);
			indices.push_back(i2 - 1);
			indices.push_back(i3 - 1);
		}




	}

	

	
	file.close();
}

void Object::setTexture(std::string path)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
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

void Object::convertData(std::string path)
{


	char line[256];
	unsigned int i1, i2, i3, t1, t2, t3;
	bool hasTexture = false;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;


	std::vector<std::string> temp;
	std::vector<Vertex> vertices2;


	std::fstream file(path);
	if (file.fail()) {
		std::cout << "Failed to open file: " << path << std::endl;
		return;
	}

	while (file.is_open() && !file.eof()) {


		file.getline(line, 256);
		
		Vertex tempVertex[3];

		glm::vec3 vertex;
		glm::vec2 texCoord;

		if (line[0] == 'v' && line[1] == ' ') {
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		if (line[0] == 'v' && line[1] == 't') {
			sscanf(line, "vt %f %f", &texCoord.x, &texCoord.y);
			hasTexture = true;
			texCoords.push_back(texCoord);
		}


		if (line[0] == 'f' && line[1] == ' ' || hasTexture) {
			sscanf(line, "f %u/%u %u/%u %u/%u", &i1, &t1, &i2, &t2, &i3, &t3);
			tempVertex[0].pos = vertices[i1 - 1];
			tempVertex[0].texCoord = texCoords[t1 - 1];
			tempVertex[1].pos = vertices[i2 - 1];
			tempVertex[1].texCoord = texCoords[t2 - 1];
			tempVertex[2].pos = vertices[i3 - 1];
			tempVertex[2].texCoord = texCoords[t3 - 1];
			indices.push_back(getVertexPos(vertices2, tempVertex[0]));
			indices.push_back(getVertexPos(vertices2, tempVertex[1]));
			indices.push_back(getVertexPos(vertices2, tempVertex[2]));
		}

		if (line[0] == 'f' && line[1] == ' ' || !hasTexture) {
			sscanf(line, "f %u %u %u", &i1, &i2, &i3);
			indices.push_back(i1 - 1);
			indices.push_back(i2 - 1);
			indices.push_back(i3 - 1);
		}




	}




	file.close();
}

unsigned int Object::getVertexPos(std::vector<Vertex>& vertices, Vertex vertex)
{

	for (int i = 0; i < vertices.size(); i++) {
		if (vertices[i].pos == vertex.pos && vertices[i].texCoord == vertex.texCoord) {
			return i;
		}
	}

	vertices.push_back(vertex);
	return vertices.size() - 1;



	return 0;
}
