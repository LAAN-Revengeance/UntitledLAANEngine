#pragma once
#include "ForceGenerator.h"

class GravityGenerator : public ForceGenerator {
public:
	void Update(PhysicsBody* pb, double deltaTime);
private:
	glm::vec3 gravity = { 0,-9.807,0 };
};