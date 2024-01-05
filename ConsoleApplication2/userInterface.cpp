#include "userInterface.hpp"




void userInterface::test(int a, int b)
{
		std::cout << a << " " << b << std::endl;
	
}

void userInterface::loop(std::vector<Renderer*>* renderQueue, Camera* camera)
{
	char buffer[1024];
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cout << "WSAStartup failed" << std::endl;
		return;
	}
	int opt = 1;
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));

	server.sin_family = AF_INET;
	server.sin_port = htons(CONSOLE_PORT);
	server.sin_addr.s_addr = htons(INADDR_ANY);

	if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		std::cout << "bind failed" << std::endl;
		return;
	}

	if (listen(sock,5) < 0) {
		std::cout << "listen failed" << std::endl;
		return;
	}

	struct sockaddr_in client;
	int c = sizeof(struct sockaddr_in);
	int client_sock = accept(sock, (struct sockaddr*)&client, &c);
	if (client_sock < 0) {
		std::cout << "accept failed" << std::endl;
		return;
	}

	std::cout << "connection accepted" << std::endl;
	while (recv(client_sock, buffer, 1024, 0) != 0) {
		//split buffer into tokens and store in vector of strings 
		std::vector<std::string> tokens;
		std::string token;
		std::istringstream tokenStream(buffer);
		while (std::getline(tokenStream, token, ' '))
		{
			tokens.push_back(token);
		}
		if (tokens[0] == "exit") {
			break;
		}
		else if (tokens[0] == "load") {
			std::string path = tokens[1];

			float x = std::stof(tokens[2]);
			float y = std::stof(tokens[3]);
			float z = std::stof(tokens[4]);
			float r = std::stof(tokens[5]);
			float g = std::stof(tokens[6]);
			float b = std::stof(tokens[7]);
			float scalex = std::stof(tokens[8]);
			float scaley = std::stof(tokens[9]);
			float scalez = std::stof(tokens[10]);

			if(tokens[11] == "true")
				renderQueue->at(0)->textureEnabled = true;
			else
				renderQueue->at(0)->textureEnabled = false;

			std::cout << path << " " << x << " " << y << " " << z << std::endl;

			Object* obj = new Object(path, FILE_FLAT);
			obj->move(glm::vec3(x, y, z));
			obj->setColor(glm::vec3(r, g, b));
			obj->scale(glm::vec3(scalex, scaley, scalez));
			renderQueue->at(0)->addObjectToQueue(obj);
			

		
		}




	}
	


}



GLuint userInterface::LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

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