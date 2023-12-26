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
	unsigned int i1, i2, i3;

	std::fstream file(path);

	while (file.is_open() && !file.eof()) {

		file.getline(line, 256);

		if (line[0] == 'v' && line[1] == ' ') {
			glm::vec3 vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			data.push_back(vertex.x);
			data.push_back(vertex.y);
			data.push_back(vertex.z);
			data.push_back(1.0f);
			data.push_back(0.0f);
			data.push_back(1.0f);
			data.push_back(0.0f);
		}

		if (line[0] == 'f' && line[1] == ' ') {
			sscanf(line, "f %u %u %u", &i1, &i2, &i3);
			indices.push_back(i1-1);
			indices.push_back(i2-1);
			indices.push_back(i3-1);
		}

	}

	

	
	file.close();
}
