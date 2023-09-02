#include "RigidBody.h"


PhysicsBody::PhysicsBody()
{
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::ApplyForce(float x, float y, float z)
{
	
}

void PhysicsBody::ApplyTorque(float x, float y, float z)
{

}

glm::vec3 PhysicsBody::GetPosition()
{
	if (body)
	{
		rp3d::Vector3 worldPos = body->getTransform().getPosition();
		return { worldPos.x, worldPos.y, worldPos.z};
	}
	return glm::vec3(0);
}

glm::vec3 PhysicsBody::GetRotation()
{
	//TODO: Change to return quat? also get euler angles from body transform
	return glm::vec3(0);
}

void PhysicsBody::SetPosition(float x, float y, float z)
{
	if (body)
	{
		body->setTransform(rp3d::Transform({ x,y,z }, body->getTransform().getOrientation()));
	}
}


void PhysicsBody::SetRotation(float x, float y, float z)
{
	rp3d::Transform nTransform(body->getTransform().getPosition(), rp3d::Quaternion().fromEulerAngles(x, y, z));
	body->setTransform(nTransform);
}

PhysicsCollider PhysicsBody::GetCollider(unsigned int colliderIndex)
{
	if (colliderIndex < colliders.size())
		return colliders[colliderIndex];
	return PhysicsCollider();
}

unsigned int PhysicsBody::GetNumColliders()
{
	return colliders.size();
}

unsigned int PhysicsBody::GetID()
{
	return ID;
}
