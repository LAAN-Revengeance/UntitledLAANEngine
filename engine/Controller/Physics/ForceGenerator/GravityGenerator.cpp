#include "GravityGenerator.h"

void GravityGenerator::Update(PhysicsBody* pb, double deltaTime)
{
	pb->ApplyForce(gravity.x, gravity.y, gravity.z);
}
