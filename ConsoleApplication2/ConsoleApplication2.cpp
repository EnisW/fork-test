#include "Util.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Render.hpp"
#include "userInterface.hpp"
#include "TextRenderer.hpp"

#define ENABLE_INPUT false

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
	window = glfwCreateWindow(WIDTH, HEIGHT, "Deneme", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible..\n");
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

	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

	


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
	Text t;
	t.text = "drone1: x-10 y-10 z-10";
	t.x = 100;
	t.y = 100;
	std::cout<< t.text << std::endl;

	//GLuint programID = LoadShaders("shader.vert", "shader.frag");
	GLuint programID2 = LoadShaders("shaderLine.vert", "shaderLine.frag");
	GLuint programID = LoadShaders("shaderColored.vert", "shaderColored.frag");
	GLuint programIDGround = LoadShaders("shaderGround.vert", "shaderGround.frag");

	TextRenderer textRenderer = TextRenderer();
	textRenderer.setBitmapFont("font.png");
	textRenderer.setScale(3.0f);


	Camera camera(window);
	camera.addProgram(programID);
	camera.addProgram(programID2);
	camera.addProgram(programIDGround);
	std::string path = "out.obj";
	std::string path1 = "ground.obj";

	Object squareObject1(path, FILE_FLAT);
	Object squareObject2(path, FILE_FLAT);
	Object squareObject3(path, FILE_FLAT);
	Object squareObject4(path, FILE_FLAT);


	Object groundObject(path1);
	groundObject.physicsEnabled = false;
	squareObject1.move(vec3(0.0f, 4.0f, 0.0f));
	squareObject2.move(vec3(45.0f, 23.0f,-20.0f));
	squareObject3.move(vec3(30.0f, 23.0f, -20.0f));
	squareObject4.move(vec3(15.0f, 23.0f, -20.0f));

	
	groundObject.move(vec3(0.0f, -5.0f, 0.0f));
	std::cout << "ground: " << groundObject.position.x << " " << groundObject.position.y << " " << groundObject.position.z << std::endl;
	std::cout << "ground size:" << groundObject.size.x << " " << groundObject.size.y << " " << groundObject.size.z << std::endl;
	std::cout << "square: " << squareObject2.position.x << " " << squareObject2.position.y << " " << squareObject2.position.z << std::endl;
	std::cout << "square size:" << squareObject2.size.x << " " << squareObject2.size.y << " " << squareObject2.size.z << std::endl;

	squareObject1.setColor(vec3(0.0f, 1.0f, 1.0f));
	squareObject2.setColor(vec3(0.0f, 1.0f, 1.0f));
	squareObject3.setColor(vec3(0.0f, 1.0f, 1.0f));
	squareObject4.setColor(vec3(0.0f, 1.0f, 1.0f));

	Physics* physics = new Physics();

	Renderer renderer = Renderer(programID);
	renderer.setPhysics(physics);
	renderer.addObject(&squareObject2);
	renderer.addObject(&squareObject3);
	renderer.addObject(&squareObject4);


	renderer.textureEnabled = false;
	
	Renderer rendererGround = Renderer(programIDGround);
	rendererGround.setPhysics(physics);
	rendererGround.addObject(&groundObject);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

	std::vector<Renderer*> renderQueue;
	renderQueue.push_back(&renderer);
	renderQueue.push_back(&rendererGround);
	renderQueue.push_back(&textRenderer);
	int command = 0;
	camera.addTextShader(textRenderer.getProgramID());
	

	if (ENABLE_INPUT) {
		userInterface ui;
		ui.textRenderer = &textRenderer;
		std::thread loopThread(&userInterface::loop, &ui, &renderQueue, &camera);
	}
	




	physics->applyGravity();
	std::thread physicsThread(&Physics::updateLoop, physics);

	
	do {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glUseProgram(programID2);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBindVertexArray(VertexArrayID);

		glDrawArrays(GL_LINES, 0, 8);
		for(Renderer* r: renderQueue)
			r->render();
		camera.update();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	physics->terminate();
	physicsThread.join();
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

