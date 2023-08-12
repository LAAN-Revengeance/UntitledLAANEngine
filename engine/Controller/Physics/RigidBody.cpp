#include "RigidBody.h"


RigidBody::RigidBody()
{
}

RigidBody::~RigidBody()
{
}

void RigidBody::ApplyForce(float x, float y, float z)
{
	
}

void RigidBody::ApplyTorque(float x, float y, float z)
{

}

glm::vec3 RigidBody::GetPosition()
{
	return position;
}

glm::vec3 RigidBody::GetRotation()
{
	return rotation;
}

void RigidBody::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void RigidBody::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}
