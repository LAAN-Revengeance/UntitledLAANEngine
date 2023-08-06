#include "Rigidbody.h"

Rigidbody::Rigidbody()
{
	
}

void Rigidbody::ApplyForce(glm::vec3 force)
{
	if (!rbPtr)
		return;
	rbPtr->applyWorldForceAtCenterOfMass({force.x,force.y,force.z});
}

void Rigidbody::ApplyForceLocal(glm::vec3 force)
{
	if (!rbPtr)
		return;
	rbPtr->applyLocalForceAtCenterOfMass({ force.x,force.y,force.z });
}

void Rigidbody::ApplyTorqueLocal(glm::vec3 torque)
{
	if (!rbPtr)
		return;
	rbPtr->applyLocalTorque({ torque.x, torque.y, torque.z });
}

void Rigidbody::ModType(int type)
{
	if (!rbPtr)
		return;
	mod = 0;
	switch (type)
	{
	case KINE:
		rbPtr->setType(rp3d::BodyType::KINEMATIC);
		mod = type;
		break;
	case STAT:
		rbPtr->setType(rp3d::BodyType::STATIC);
		mod = type;
		break;
	case DYNA:
		rbPtr->setType(rp3d::BodyType::DYNAMIC);
		mod = type;
		break;
	}
	
}

void Rigidbody::SetUseGravity(bool isGravity)
{
	if (!rbPtr)
		return;
	rbPtr->enableGravity(isGravity);
}

void Rigidbody::SetPosition(glm::vec3 newPos)
{
	if (!rbPtr)
		return;
	rbPtr->setTransform(rp3d::Transform({ newPos.x,newPos.y,newPos.z }, rbPtr->getTransform().getOrientation()));
}

void Rigidbody::SetRotation(glm::vec3 newRot)
{
	if (!rbPtr)
		return;
	//convert to radians
	newRot = glm::radians(newRot);
	rp3d::Quaternion q;
	if (newRot.length() < std::numeric_limits<double>::epsilon()) {
		q = rp3d::Quaternion::identity();
	}
	else {
		q = rp3d::Quaternion::fromEulerAngles({ newRot.x,newRot.y,newRot.z });
	}
	q.normalize();
	rbPtr->setTransform(rp3d::Transform(rbPtr->getTransform().getPosition(),q));
}

glm::vec3 Rigidbody::GetPosition()
{
	if (!rbPtr)
		return glm::vec3();
	rp3d::Vector3 pos = rbPtr->getTransform().getPosition();
	return { pos.x,pos.y,pos.z };
}

void Rigidbody::SetMass(float nMass)
{
	if (!rbPtr)
		return;
	rbPtr->setMass(nMass);
}

void Rigidbody::SetCenterOfMass(glm::vec3 center)
{
	if (!rbPtr)
		return;
	rbPtr->setLocalCenterOfMass({ center.x, center.y, center.z });
}

void Rigidbody::SetDampeningLinear(float damp)
{
	if (!rbPtr)
		return;
	rbPtr->setLinearDamping(damp);
}

void Rigidbody::SetDampeningAngle(float damp)
{
	if (!rbPtr)
		return;
	rbPtr->setAngularDamping(damp);
}

void Rigidbody::SetAxisLinearFactor(float x, float y, float z)
{
	if (!rbPtr)
		return;
	rbPtr->setLinearLockAxisFactor({ x,y,z });
}

void Rigidbody::SetAxisAngleFactor(float x, float y, float z)
{
	if (!rbPtr)
		return;
	rbPtr->setAngularLockAxisFactor({ x,y,z });
}

void Rigidbody::SetLinearVelocity(float x, float y, float z)
{
	if (!rbPtr)
		return;

	rbPtr->setLinearVelocity({ x,y,z });
}

void Rigidbody::SetAngularVelocity(float x, float y, float z)
{
	if (!rbPtr)
		return;

	rbPtr->setAngularVelocity({ x,y,z });
}

glm::vec3 Rigidbody::GetLinearVelocty()
{
	if (!rbPtr)
		return glm::vec3(0);

	rp3d::Vector3 rVal = rbPtr->getLinearVelocity();

	return { (float)rVal.x,(float)rVal.y,(float)rVal.z };
}

glm::vec3 Rigidbody::GetAngularVelocity()
{
	if (!rbPtr)
		return glm::vec3(0);

	rp3d::Vector3 rVal = rbPtr->getAngularVelocity();

	return { (float)rVal.x,(float)rVal.y,(float)rVal.z};
}

void Rigidbody::SetColliderBox(float mass, float bounce, float friction, glm::vec3 offset, glm::vec3 rotation, glm::vec3 scale)
{
}

void Rigidbody::SetColliderSphere(float mass, float bounce, float friction, glm::vec3 offset, glm::vec3 rotation, float radius)
{
}

void Rigidbody::SetColliderCapsule(float mass, float bounce, float friction, glm::vec3 offset, glm::vec3 rotation, float radius, float height)
{
}

PhysicsCollider* Rigidbody::GetCollider()
{
	return collider;
}

int Rigidbody::GetModType()
{
	return mod;
}

float Rigidbody::GetMass()
{
	if (!rbPtr)
		return -1.0f;

	return rbPtr->getMass();
}

float Rigidbody::GetDampeningLinear()
{
	if (!rbPtr)
		return 0.0f;

	return rbPtr->getLinearDamping();
}

float Rigidbody::GetDampeningAngle()
{
	if (!rbPtr)
		return 0.0f;

	return rbPtr->getAngularDamping();
}

glm::vec3 Rigidbody::GetCenterOfMass()
{
	if (!rbPtr)
		return {0,0,0};

	rp3d::Vector3 val = rbPtr->getLocalCenterOfMass();
	return { val.x, val.y, val.z};
}

glm::vec3 Rigidbody::GetAxisLinearFactor()
{
	if (!rbPtr)
		return { 0,0,0 };

	rp3d::Vector3 val = rbPtr->getLinearLockAxisFactor();
	return { val.x, val.y, val.z };
}

glm::vec3 Rigidbody::GetAxisAngleFactor()
{
	if (!rbPtr)
		return { 0,0,0 };

	rp3d::Vector3 val = rbPtr->getAngularLockAxisFactor();
	return { val.x, val.y, val.z };
}

bool Rigidbody::GetIsContactListen()
{
	return registered;
}
