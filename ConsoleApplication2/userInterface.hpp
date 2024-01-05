#pragma once
#include "Util.hpp"
#include "Render.hpp"


class userInterface {
public:
	void test(int a, int b);
	void loop(std::vector<Renderer*>* renderQueue, Camera* camera);

private:

	GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
};