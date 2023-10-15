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

glm::vec3 PhysicsBody::GetPosition()const
{
	if (body)
	{
		rp3d::Vector3 worldPos = body->getTransform().getPosition();
		return { worldPos.x, worldPos.y, worldPos.z};
	}
	return glm::vec3(0);
}

glm::quat PhysicsBody::GetRotation() const
{
	rp3d::Quaternion rp3dRot = body->getTransform().getOrientation();
	return glm::quat(rp3dRot.w, rp3dRot.x, rp3dRot.y, rp3dRot.z);
}

void PhysicsBody::SetPosition(float x, float y, float z)
{
	if (body)
	{
		body->setTransform(rp3d::Transform({ x,y,z }, body->getTransform().getOrientation()));
	}
}

void PhysicsBody::SetRotationEuler(float x, float y, float z)
{
	rp3d::Transform nTransform(body->getTransform().getPosition(), rp3d::Quaternion().fromEulerAngles(x, y, z));
	body->setTransform(nTransform);
}

void PhysicsBody::SetRotation(glm::quat nRotation)
{
	body->setTransform({ body->getTransform().getPosition(), { nRotation.x,nRotation.y,nRotation.z,nRotation.w } });
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

void PhysicsBody::SetMass(float nMass)
{
	//cannot have 0 or negative mass
	if (mass <= 0) {
		mass = 0.00001;
		return;
	}

	mass = nMass;
}

float PhysicsBody::GetMass()
{
	return mass;
}

void PhysicsBody::DeleteCollider(unsigned int colliderIndex)
{
	if (colliderIndex < colliders.size())
		body->removeCollider(colliders.at(colliderIndex).rp3dCollider);
	colliders.erase(colliders.begin() + colliderIndex);
}

void PhysicsBody::SetVelocity(float x, float y, float z)
{
}

void PhysicsBody::ClearAccumilator()
{
}

void PhysicsBody::SetGravity(bool nGrav)
{
	useGravity = nGrav;
}
