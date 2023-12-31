#include "Render.hpp"




Renderer::Renderer(GLuint programID)
{
	screenHeight = 768;
	screenWidth = 1024;


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
		sizeof(float) * 9,
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 9,
		(void*)(sizeof(float) * 3)
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 9,
		(void*)(sizeof(float) * 6)
	);


	glEnableVertexAttribArray(3);
	glVertexAttribPointer(
		3,
		1,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 9,
		(void*)(sizeof(float) * 8)
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	this->programID = programID;
	modelUniformID = glGetUniformLocation(programID, "M");
	shadowShaderProgram = LoadShaders("shadowMap.vert", "shadowMap.frag");

	createShadowMap();
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
		for (int i = 0; i < indicies_t.size(); i++) {
			this->indicies.push_back(indicies_t[i] + bias);
		}


		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

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
	updateShadowMap();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(programID, "shadowMap"), 1);

	glUseProgram(programID);
	glBindVertexArray(VertexArrayID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	glUniformMatrix4fv(modelUniformID, objects.size(), GL_FALSE, &modelMatrices[0][0][0]);

	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, (void*)0);

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

void Renderer::updateShadowMap()
{

	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
	glm::mat4 lightView = glm::lookAt(
		glm::vec3(0.0f, 10.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0, 0.0, 0.0)
	);
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	// Use the shadow map framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Use depth shader
	glUseProgram(shadowShaderProgram);

	// Set lightSpaceMatrix uniform in shader
	// This matrix transforms from world space to light space
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

	// Render the scene using the depth shader
	
		glBindVertexArray(VertexArrayID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

		glUniformMatrix4fv(modelUniformID, objects.size(), GL_FALSE, &modelMatrices[0][0][0]);

		glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, (void*)0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		

		glBindVertexArray(0);
	

	// Reset framebuffer and viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screenWidth, screenHeight);
	glUseProgram(0);


}

void Renderer::createShadowMap()
{


	// Create framebuffer
	glGenFramebuffers(1, &shadowMapFBO);

	// Create depth texture
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Attach depth texture to framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
