#include "Physics.hpp"

Physics::Physics()
{
	running = true;
	objects = std::vector<Object*>();
	deltaTime = 0.0f;
	lastTime = glfwGetTime();
	gravity = glm::vec3(0, -9.8, 0);
}

Physics::~Physics()
{
}

void Physics::updateLoop()
{
	while (running) {
		update();
	}
}

void Physics::update()
{
	deltaTime = glfwGetTime() - lastTime;
	
	lastTime = glfwGetTime();
	integrateEuler();
	handleCollisions();

}



void Physics::addObject(Object* object)
{
	objects.push_back(object);
	object->acceleration += gravity;
}

void Physics::integrateEuler()
{
	for (int i = 0; i < objects.size(); i++) {
		if(!objects[i]->physicsEnabled)
			continue;
		objects[i]->velocity += objects[i]->acceleration * deltaTime;
		objects[i]->move(objects[i]->velocity * deltaTime);
	}
}

void Physics::applyGravity()
{
	for (int i = 0; i < objects.size(); i++) {
		if (!objects[i]->physicsEnabled)
			continue;
		objects[i]->acceleration += gravity;
	}
}

void Physics::handleCollisions()
{
	/*
	for (int i = 0; i < objects.size(); i++) {
		if(!objects[i]->physicsEnabled)
			continue;
	
		if (objects[i]->position.y < 0) {
			objects[i]->position.y = 0;
			objects[i]->velocity.y = -objects[i]->velocity.y;
			objects[i]->move();
		}
	}
	*/

	for (size_t i = 0; i < objects.size(); ++i) {
		for (size_t j = i + 1; j < objects.size(); ++j) {

			if (isColliding(objects[i], objects[j])) {
				if (objects[i]->physicsEnabled) {
					objects[i]->velocity = -objects[i]->velocity * 0.5f;
					
					if (abs(objects[i]->velocity.y) < 0.01f) {
						objects[i]->acceleration.y = 0.0f;
						
						objects[i]->velocity.y = 0.0f;
					}
					
				}
				if (objects[j]->physicsEnabled) {
					objects[j]->velocity = -objects[j]->velocity * 0.5f;
					if (abs(objects[j]->velocity.y) < 0.01f) {
						objects[j]->acceleration.y = 0.0f;

						objects[j]->velocity.y = 0.0f;
					}
				}
			}
		}
	}



}

bool Physics::isColliding(Object* object1, Object* object2)
{
	

	if (object1->position.x < object2->position.x + object2->size.x &&
		object1->position.x + object1->size.x > object2->position.x &&
		object1->position.y < object2->position.y + object2->size.y &&
		object1->position.y + object1->size.y > object2->position.y &&
		-object1->position.z < -object2->position.z + object2->size.z &&
		-object1->position.z + object1->size.z > -object2->position.z) {
		return true;
	}

	return false;
}
