#include "RigidBody.h"


PhysicsBody::PhysicsBody()
{
}

PhysicsBody::~PhysicsBody()
{

}

void PhysicsBody::ApplyForce(float x, float y, float z)
{
	linearVelocity.x += x;
	linearVelocity.y += y;
	linearVelocity.z += z;
}

void PhysicsBody::ApplyTorque(float x, float y, float z)
{

}

void PhysicsBody::SetLinearDrag(float drag)
{
	linearDrag = drag;
}

void PhysicsBody::SetAngularDrag(float drag)
{
	angularDrag = drag;
}

glm::vec3 PhysicsBody::GetPosition()const
{
	if (body)
	{
		rp3d::Vector3 worldPos = body->getTransform().getPosition();
		return { worldPos.x, worldPos.y, worldPos.z};
	}
	return glm::vec3(0);
}

glm::vec3 PhysicsBody::GetRotation()const
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

PhysicsCollider& PhysicsBody::GetCollider(unsigned int colliderIndex)
{
	if (colliderIndex < colliders.size())
		return colliders[colliderIndex];
}

unsigned int PhysicsBody::GetNumColliders()const
{
	return colliders.size();
}

unsigned int PhysicsBody::GetID()
{
	return ID;
}

void PhysicsBody::Update()
{
	glm::vec3 newPos = GetPosition() + linearVelocity;


	SetPosition(newPos.x, newPos.y, newPos.z);
}

glm::vec3 PhysicsBody::GetCenterOfMass() const
{
	return centerOfMass;
}

void PhysicsBody::DeleteCollider(unsigned int colliderIndex)
{
	if (colliderIndex < colliders.size())
		body->removeCollider(colliders.at(colliderIndex).rp3dCollider);
	colliders.erase(colliders.begin() + colliderIndex);
}

void PhysicsBody::ToggleGravity()
{
	isGravityEnabled = !isGravityEnabled;
}

