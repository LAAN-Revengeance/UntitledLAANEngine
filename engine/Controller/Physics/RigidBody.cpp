#include "RigidBody.h"


PhysicsBody::PhysicsBody()
{
}

PhysicsBody::~PhysicsBody()
{
}

void PhysicsBody::ApplyForce(float x, float y, float z)
{
	force.x += x;
	force.y += y;
	force.z += z;
}

glm::vec3 PhysicsBody::GetForce()
{
	return force;
}

void PhysicsBody::ApplyTorque(float x, float y, float z)
{
	torque.x += x;
	torque.y += y;
	torque.z += z;
}

glm::vec3 PhysicsBody::GetTorque()
{
	return torque;
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
	if (body)
	{
		rp3d::Quaternion rp3dRot = body->getTransform().getOrientation();
		return glm::quat(rp3dRot.w, rp3dRot.x, rp3dRot.y, rp3dRot.z);
	}
	return glm::quat(0, 0, 0, 0);
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
	if (body)
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

void PhysicsBody::SetAngularVelocity(float x, float y, float z)
{
	angularVelocity.x = x;
	angularVelocity.y = y;
	angularVelocity.z = z;
}

void PhysicsBody::SetMass(float nMass)
{
	//cannot have 0 or negative mass
	if (nMass < 0) {
		mass = 0.00001;
		inverseMass = nMass;
		return;
	}
	if (nMass == 0)
	{
		mass = 0;
		inverseMass = 0;
		return;
	}

	mass = nMass;
	inverseMass = 1 / nMass;
}

glm::vec3 PhysicsBody::GetAngularVelocity()
{
	return angularVelocity;
}

float PhysicsBody::GetMass()
{
	return mass;
}

float PhysicsBody::GetInverseMass()
{
	return inverseMass;
}

void PhysicsBody::DeleteCollider(unsigned int colliderIndex)
{
	if (colliderIndex < colliders.size())
		body->removeCollider(colliders.at(colliderIndex).rp3dCollider);
	colliders.erase(colliders.begin() + colliderIndex);
}

void PhysicsBody::SetLinearVelocity(float x, float y, float z)
{
	linearVelocity.x = x;
	linearVelocity.y = y;
	linearVelocity.z = z;
}

glm::vec3 PhysicsBody::GetLinearVelocity()
{
	return linearVelocity;
}

void PhysicsBody::ClearAccumulator()
{
	force = { 0,0,0 };
	torque = { 0,0,0 };
}

void PhysicsBody::SetGravity(bool nGrav)
{
	useGravity = nGrav;
}

void PhysicsBody::UpdateTensor()
{
	for (auto& collider : colliders)
	{
		if (collider.type == COLLIDER_SPHERE)
		{
			SphereCollider* sphereShape = static_cast<SphereCollider*>(&collider);
			float radius = sphereShape->GetRadius();
			float a = (2 / 5) * mass * radius * radius;

			if (a != 0)
			{
				a = 1 / a;
				inverseTensor = { a,a,a};
			}
			else
			{
				inverseTensor = { a,a,a };
			}
			
		}
		if (collider.type == COLLIDER_BOX)
		{
			BoxCollider* cubeShape = static_cast<BoxCollider*>(&collider);
			glm::vec3 scale = cubeShape->GetScale();
			
			float a = (1.0f / 12.0f); 
			
			inverseTensor.x = (a * mass * (scale.y * scale.y + scale.z * scale.z));
			inverseTensor.y = (a * mass * (scale.x * scale.x + scale.z * scale.z));
			inverseTensor.z = (a * mass * (scale.x * scale.x + scale.y * scale.y));

			if (inverseTensor.x != 0)
				inverseTensor.x = 1.0f / inverseTensor.x;
			if (inverseTensor.y != 0)
				inverseTensor.y = 1.0f / inverseTensor.y;
			if (inverseTensor.z != 0)
				inverseTensor.z = 1.0f / inverseTensor.z;

			//std::cout <<	inverseTensor.x << "::" <<
			//				inverseTensor.y << "::" <<
			//				inverseTensor.z << "\n\n";
		}
	}
}

glm::vec3 PhysicsBody::GetInverseTensor()
{
	return inverseTensor;
}

glm::vec3 PhysicsBody::GetInverseTensorWorld()
{
	glm::mat3 rot = glm::mat3_cast(GetRotation());
	glm::mat3 invRot = glm::mat3_cast(glm::conjugate(GetRotation()));
	return rot * inverseTensor * invRot;
}
