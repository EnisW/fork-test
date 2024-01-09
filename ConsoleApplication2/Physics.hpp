#pragma once
#include "Util.hpp"
#include "Object.hpp"

class Physics
{

public:
	Physics();
	~Physics();

	void updateLoop();
	void update();
	void addObject(Object* object);
	void integrateEuler();
	void applyGravity();
	void handleCollisions();
	void terminate() {
		running = false;
	};
private:
	std::vector<Object*> objects;
	
	glm::vec3 gravity;

	float deltaTime;
	float lastTime;

	bool running;


	bool isColliding(Object* object1, Object* object2);
};