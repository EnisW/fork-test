#include "Util.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Render.hpp"

using namespace glm;

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

GLFWwindow* window;


static const GLfloat g_vertex_buffer_data2[] = {
   -10.0f, 0.0f,0.0f, 1.0f, 1.0f, 1.0f,
   0.0f, 0.0f, 0.0f,1.0f,1.0f,1.0f,
   10.0f, 0.0f, 0.0f,1.0f, 0.0f, 0.0f,
   0.0f, 0.0f, 0.0f,1.0f,0.0f,0.0f,
   0.0f, 0.0f, 0.0f,0.0f, 1.0f, 0.0f,
   0.0f, 10.0f, 0.0f,0.0f,1.0f,0.0f,
   0.0f, 0.0f, 0.0f,1.0f, 1.0f, 0.0f,
   0.0f, -10.0f, 0.0f,1.0f,1.0f,0.0f
};








int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Deneme", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);


	std::vector<float> square0 = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 0
		0.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 1
		3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// 2
		3.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// 3
		0.0f, 3.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,// 4
		3.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f// 5
	};

	std::vector<float> square1 = {
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,0.0f, // 0
		0.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f,0.0f, // 1
		-3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,0.0f, // 2
		-3.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f,0.0f, // 3
		0.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f,0.0f, // 4
		-3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f // 5
	};









	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data2), g_vertex_buffer_data2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 6,
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(float) * 6,
		(void*)(sizeof(float) * 3)
	);



	GLuint programID = LoadShaders("shader.vert", "shader.frag");


	glUseProgram(programID);

	Camera camera(programID, window);

	std::string path = "untitled.obj";

	Object squareObject0(path);
	Object squareObject1(path);
	squareObject0.move(vec3(0.0f, 0.0f, 0.0f));
	squareObject1.move(vec3(0.0f, 4.0f, 0.0f));



	Renderer renderer = Renderer(programID);
	renderer.addObject(&squareObject0);
	renderer.addObject(&squareObject1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);



	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindVertexArray(VertexArrayID);

		glDrawArrays(GL_LINES, 0, 8);

		renderer.render();
		camera.update();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}




GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

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