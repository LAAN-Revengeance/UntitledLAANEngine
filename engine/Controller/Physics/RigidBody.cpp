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
		return { position.x, position.y, position.z};
	}
	return glm::vec3(0);
}

glm::quat PhysicsBody::GetRotation() const
{
	rp3d::Quaternion rp3dRot = body->getTransform().getOrientation();
	return glm::quat(orientation.w, orientation.x, orientation.y, orientation.z);
}

void PhysicsBody::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
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

float PhysicsBody::GetInverseMass()
{
	return 1.0f/mass;
}

void PhysicsBody::DeleteCollider(unsigned int colliderIndex)
{
	if (colliderIndex < colliders.size())
		body->removeCollider(colliders.at(colliderIndex).rp3dCollider);
	colliders.erase(colliders.begin() + colliderIndex);
}

void PhysicsBody::SetVelocity(glm::vec3 newVelocity)
{
	velocity.x = newVelocity.x;
	velocity.y = newVelocity.y;
	velocity.z = newVelocity.z;
}

void PhysicsBody::ClearAccumilator()
{
}

void PhysicsBody::SetGravity(bool nGrav)
{
	useGravity = nGrav;
}

glm::vec3 PhysicsBody::GetVelocity()
{
	return velocity;
}

glm::vec3 PhysicsBody::GetAngularVelocity()
{
	return angularVelocity;
}

void PhysicsBody::SetAngularVelocity(glm::vec3 newVelocity)
{
	angularVelocity.x = newVelocity.x;
	angularVelocity.y = newVelocity.y;
	angularVelocity.z = newVelocity.z;
}

glm::mat3 PhysicsBody::GetInertiaTensor()
{
	return inertiaTensor;
}

glm::mat3 PhysicsBody::GetInverseInertiaTensor()
{
	return inverseInertiaTensor;
}

void PhysicsBody::SetIntertiaTensor(glm::mat3 tensor)
{
	inertiaTensor = tensor;
}

void PhysicsBody::SetOrientation(glm::quat newOrientation)
{
	orientation = newOrientation;
}

glm::quat PhysicsBody::GetOrientation()
{
	return orientation;
}

void PhysicsBody::SetIsKinematic(bool value)
{
	isKinematic = value;
}

void PhysicsBody::UpdateBody()
{
	rp3d::Vector3 tempPos;
	tempPos.x = position.x;
	tempPos.y = position.y;
	tempPos.z = position.z;

	rp3d::Quaternion tempQuat;
	tempQuat.x = orientation.x;
	tempQuat.y = orientation.y;
	tempQuat.z = orientation.z;
	tempQuat.w = orientation.w;
	
	rp3d::Transform temp = rp3d::Transform(tempPos, tempQuat);
	body->setTransform(temp);
}

void PhysicsBody::CalculateInertiaTensor()
{	
	for (auto& collider : colliders)
	{
		if (collider.type == COLLIDER_BOX)
		{
			glm::mat3 inertia{};
			
			BoxCollider* box = static_cast<BoxCollider*>(&collider);

			glm::vec3 boxScale = box->GetScale();

			inertia[0][0] = (1.0f / 12.0f) * mass * (boxScale.y * boxScale.y + boxScale.x * boxScale.x);
			inertia[1][1] = (1.0f / 12.0f) * mass * (boxScale.z * boxScale.z + boxScale.x * boxScale.x);
			inertia[2][2] = (1.0f / 12.0f) * mass * (boxScale.z * boxScale.z + boxScale.y * boxScale.y);

			inertiaTensor = inertia;
			inverseInertiaTensor = glm::inverse(inertia);
		}
		if (collider.type == COLLIDER_SPHERE)
		{
			glm::dmat3 inertia{};

			SphereCollider* sphere = static_cast<SphereCollider*>(&collider);

			float radius = sphere->GetRadius();

			inertia[0][0] = (2.0 / 5.0) * mass * (radius * radius);
			inertia[1][1] = (2.0 / 5.0) * mass * (radius * radius);
			inertia[2][2] = (2.0 / 5.0) * mass * (radius * radius);

			inertiaTensor = inertia;
			inverseInertiaTensor = glm::inverse(inertia);
		}
	}

}
