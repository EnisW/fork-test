#include "Render.hpp"




Renderer::Renderer()
{
	physics = nullptr;
}

Renderer::Renderer(GLuint programID)
{
	screenHeight = HEIGHT;
	screenWidth = WIDTH;
	priv = false;
	physics = nullptr;

	textureEnabled = false;

	modelMatrices.resize(MAX_OBJECT);
	modelMatrixUsed.resize(MAX_OBJECT);
	for (int i = 0; i < MAX_OBJECT; i++) {
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
		sizeof(Vertex),
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, color))
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, normal))
	);


	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, texCoord))
	);


	glEnableVertexAttribArray(4);
	glVertexAttribPointer(
		4,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(offsetof(Vertex, modelIndex))
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	this->programID = programID;
	glUseProgram(programID);
	int samplers[32];
	for (int i = 0; i < 32; i++) {
		samplers[i] = i;
	}
	glUniform1iv(glGetUniformLocation(programID, "sampler"), 32, samplers);
	modelUniformID = glGetUniformLocation(programID, "M");
	lightUniformID = glGetUniformLocation(programID, "lightPos");
	lightPos = glm::vec3(0.0f, 26.0f, 0.0f);
	glUniform3fv(lightUniformID, 1, &lightPos[0]);
}

Renderer::~Renderer()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
}

bool Renderer::addObject(Object* object)
{
		//physics->addObject(object);


		if(objects.size() >= MAX_OBJECT)
			return false;


		objects.push_back(object);

		

		for (int i = 0; i < MAX_OBJECT; i++) {
			if (modelMatrixUsed[i] == false) {
				object->modelIndex = i;
				modelMatrices[i] = object->getModelMatrix();
				modelMatrixUsed[i] = true;
				break;
			}
		}


		for (int i = 0; i < object->data.size(); i++) {
			object->data[i].modelIndex = object->modelIndex;
		}
		unsigned int bias = this->vertices.size();

		std::vector<Vertex> data = object->getData();
		for (int i = 0; i < data.size(); i++) {
			vertices.push_back(data[i]);
		}
		std::vector<unsigned int> indicies_t = object->getIndices();
		object->elementBufferBias = bias;
		object->elementBufferOffset = indices.size();
		for (int i = 0; i < indicies_t.size(); i++) {
			this->indices.push_back(indicies_t[i] + bias);
		}


		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * this->indices.size(), &this->indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		
		std::cout << "added object" << std::endl;

		return true;

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
		this->indices.erase(std::remove(this->indices.begin(), this->indices.end(), indicies_t[i]), this->indices.end());
	}


	modelMatrices.erase(std::remove(modelMatrices.begin(), modelMatrices.end(), object->getModelMatrix()), modelMatrices.end());
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
	bool found = false;

	for(int i = 0; i < objects.size(); i++){
		if(objects[i]->name == object->name){
			objects.erase(objects.begin() + i);
			found = true;
			break;
		}
	}
	if (!found)
		return;

	vertices.erase(vertices.begin() + object->elementBufferBias, vertices.begin() + object->elementBufferBias + object->data.size());

	indices.erase(indices.begin() + object->elementBufferOffset, indices.begin() + object->elementBufferOffset + object->indices.size());

	for (int i = 0; i < objects.size(); i++) {

		if (object->elementBufferBias < objects[i]->elementBufferBias) {
			objects[i]->elementBufferBias -= object->data.size();
		}

		if (object->elementBufferOffset < objects[i]->elementBufferOffset) {
			objects[i]->elementBufferOffset -= object->indices.size();
		}

	}


	for(int i = object->elementBufferOffset; i < indices.size(); i++){
		indices[i] -= object->data.size();
	}

	modelMatrixUsed[object->modelIndex] = false;

	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Renderer::addObjectToQueue(Object* object)
{
	objectQueue.push_back(object);
}

void Renderer::setPhysics(Physics* physics)
{
		this->physics = physics;
		for(Object* o: objects)
			physics->addObject(o);
}

void Renderer::render()
{

	for (Object* o : objectQueue) {
		addObject(o);
	}
	for (Object* o : objects)
		modelMatrices[o->modelIndex] = o->getModelMatrix();


	objectQueue.clear();

	glUseProgram(programID);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	glUniformMatrix4fv(modelUniformID, objects.size(), GL_FALSE, &modelMatrices[0][0][0]);


	if (textureEnabled) {
		for(Object* o: objects)
			glBindTextureUnit(o->modelIndex, o->textureID);
	}



	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

GLuint Renderer::LoadShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

