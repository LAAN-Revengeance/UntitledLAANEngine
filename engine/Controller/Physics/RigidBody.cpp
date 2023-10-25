#include "RigidBody.h"


PhysicsBody::PhysicsBody()
{
}

PhysicsBody::~PhysicsBody()
{

}

void PhysicsBody::ApplyForce(float x, float y, float z)
{
	forceAccumilator.x += x;
	forceAccumilator.y += y;
	forceAccumilator.z += z;
}

void PhysicsBody::ApplyForceImpulse(float x, float y, float z)
{
	velocity.x += x * inverseMass;
	velocity.y += y * inverseMass;
	velocity.z += z * inverseMass;
}

void PhysicsBody::ApplyTorque(float x, float y, float z)
{
	touqueAccumilator.x += x;
	touqueAccumilator.y += y;
	touqueAccumilator.z += z;
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

void PhysicsBody::AddVelocity(glm::vec3 nVel)
{
	velocity += nVel;
}

void PhysicsBody::AddAngularVelocity(glm::vec3 nVel)
{
	angularVelocity += nVel;
}

glm::vec3 PhysicsBody::GetAngularVelocity()
{
	return angularVelocity;
}

glm::vec3 PhysicsBody::GetVelocity()
{
	return velocity;
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

glm::vec3 PhysicsBody::GetCenterOfMass() const
{
	return centerOfMass;
}



void PhysicsBody::SetMass(float nMass)
{
	//cannot have 0 or negative mass
	if (mass <= 0) {
		mass = 0.00001;
		return;
	}

	mass = nMass;
	inverseMass = 1 / mass;
}

void PhysicsBody::SetMassInf()
{
	mass = 0;
	inverseMass = 0;
	tensor = glm::mat3(0.0f);
	inverseTensor = glm::mat3(0.0f);
	inverseTensorWorld = glm::mat3(0.0f);
}

float PhysicsBody::GetMass()
{
	return mass;
}

float PhysicsBody::GetInverseMass()
{
	return inverseMass;
}

glm::mat3 PhysicsBody::GetTensor()
{
	return tensor;
}

glm::mat3 PhysicsBody::GetInverseTensor()
{
	return inverseTensor;
}

glm::mat3 PhysicsBody::GetInverseTensorWorld()
{
	return inverseTensorWorld;
}

glm::mat4 PhysicsBody::GetTransformMatrix()
{
	return transformMatrix;
}

void PhysicsBody::DeleteCollider(unsigned int colliderIndex)
{
	if (colliderIndex < colliders.size())
		body->removeCollider(colliders.at(colliderIndex).rp3dCollider);
	colliders.erase(colliders.begin() + colliderIndex);
}

void PhysicsBody::SetVelocity(float x, float y, float z)
{
	velocity = { x,y,z };
}

void PhysicsBody::SetAngularVelocity(float x, float y, float z)
{
	angularVelocity = { x,y,z };
}

void PhysicsBody::ClearAccumilator()
{
	forceAccumilator = { 0,0,0 };
	touqueAccumilator = { 0,0,0 };
}

void PhysicsBody::SetGravity(bool nGrav)
{
	useGravity = nGrav;
}

float PhysicsBody::GetBounce()
{
	return bounce;
}

float PhysicsBody::GetLinearDampening()
{
	return dampeningLinear;
}

float PhysicsBody::GetAngularDampening()
{
	return dampeningAngular;
}

void PhysicsBody::SetBounce(float nBounce)
{
	bounce = nBounce;
}

void PhysicsBody::SetLinearDampening(float nDamp)
{
	dampeningLinear = nDamp;
}

void PhysicsBody::SetAngularDampening(float nDamp)
{
	dampeningAngular = nDamp;
}

void PhysicsBody::CalcDerivedData()
{
	CalcTransformMatrix();
	CalcInertiaTensor();//temp should only do once
	CalcInverseTensorWorld();
}

void PhysicsBody::CalcTransformMatrix()
{
	transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, GetPosition());
	glm::mat4 rotationMat = glm::mat4_cast(GetRotation());
	transformMatrix = transformMatrix * rotationMat;
}

void PhysicsBody::CalcInverseTensorWorld()
{
	glm::mat3 rotMat = glm::toMat3(GetRotation());
	glm::mat3 invRot = glm::toMat3(glm::conjugate(GetRotation()));
	inverseTensorWorld = rotMat * inverseTensor * invRot;
}

void PhysicsBody::CalcInertiaTensor()
{	
	glm::mat3 inertiaTensor(0.0f);
	for (auto& collider : colliders) {

		if (collider.type == COLLIDER_BOX) {
			BoxCollider* box = static_cast<BoxCollider*>(&collider);
			
			glm::vec3 extents = box->GetScale();
			inertiaTensor[0][0] = (1.0f / 3.0f) * (collider.GetMass()) * ((extents.y * extents.y) + (extents.z * extents.z));
			inertiaTensor[1][1] = (1.0f / 3.0f) * (collider.GetMass()) * ((extents.x * extents.x) + (extents.z * extents.z));
			inertiaTensor[2][2] = (1.0f / 3.0f) * (collider.GetMass()) * ((extents.x * extents.x) + (extents.y * extents.y));
		}
		if (collider.type == COLLIDER_SPHERE) {
			SphereCollider* sphere = static_cast<SphereCollider*>(&collider);

			inertiaTensor[0][0] = 0.4 * collider.GetMass() * (sphere->GetRadius() * sphere->GetRadius());
			inertiaTensor[1][1] = 0.4 * collider.GetMass() * (sphere->GetRadius() * sphere->GetRadius());
			inertiaTensor[2][2] = 0.4 * collider.GetMass() * (sphere->GetRadius() * sphere->GetRadius());
		}
	}

	tensor = inertiaTensor;
	inverseTensor = inverse(inertiaTensor);
}

void PhysicsBody::CalcCenterOfMass()
{
	float totalMass = 0;
	glm::vec3 firstMoment(0.0f, 0.0f, 0.0f);

	for (auto& collider : colliders)
		totalMass += collider.GetMass();

	for (auto& collider : colliders)
		firstMoment += (collider.GetMass() * collider.GetOffset());

	centerOfMass = firstMoment / totalMass;
	SetMass(totalMass);
}

