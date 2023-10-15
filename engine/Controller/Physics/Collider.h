#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

const int COLLIDER_INVALID	= 0;
const int COLLIDER_BOX		= 1;
const int COLLIDER_SPHERE	= 2;
const int COLLIDER_CAPSULE	= 3;
const int COLLIDER_TERRAIN	= 4;

class PhysicsCollider
{
public:
	PhysicsCollider() :
		mass(1),
		bounce(0.5),
		friction(0.3)
	{
	}

	glm::vec3 GetOffset() const {
		rp3d::Vector3 rVec = rp3dCollider->getLocalToBodyTransform().getPosition();
		return {rVec.x,rVec.y,rVec.z};
	}

	glm::vec3 GetRotation() const {
		rp3d::Quaternion rp3dquat = rp3dCollider->getLocalToBodyTransform().getOrientation();

		glm::quat glmquat(rp3dquat.w, rp3dquat.x, rp3dquat.y, rp3dquat.z);
		glm::vec3 rVec = glm::degrees(glm::eulerAngles(glmquat));
		return rVec;
	}

	float GetMass() const {
		return mass;
	}

	void SetRotation(glm::vec3 nRot) {

		glm::vec3 nRotRad = glm::radians(nRot);
		if (rp3dCollider) {
			rp3d::CollisionBody* body = rp3dCollider->getBody();
			rp3d::CollisionShape* shape = rp3dCollider->getCollisionShape();
			rp3d::Transform nTransform(rp3dCollider->getLocalToBodyTransform().getPosition(), rp3d::Quaternion().fromEulerAngles(nRotRad.x, nRotRad.y, nRotRad.z));

			body->removeCollider(rp3dCollider);
			body->addCollider(shape, nTransform);
		}
	}

	void SetOffset(glm::vec3 nOffset) {
		if (rp3dCollider) {
			
			rp3d::CollisionBody* body = rp3dCollider->getBody();
			rp3d::CollisionShape* shape = rp3dCollider->getCollisionShape();
			rp3d::Transform nTransform({ nOffset.x,nOffset.y,nOffset.z }, rp3dCollider->getLocalToBodyTransform().getOrientation());

			body->removeCollider(rp3dCollider);
			body->addCollider(shape,nTransform);
		}
	}

	void SetMass(float nMass) {
		mass = nMass;
	}

	int GetType() {
		return type;
	}

protected:
	rp3d::Collider* rp3dCollider = nullptr;

	float mass;
	float bounce;
	float friction;

	int type = COLLIDER_INVALID;

	friend class PhysicsManager;
	friend class PhysicsBody;
	friend class SceneEditor;
};

class BoxCollider : public PhysicsCollider
{
public:
	BoxCollider() {
		type = COLLIDER_BOX;
	}

	glm::vec3 GetScale() {

		if (rp3dCollider) {
			rp3d::BoxShape* boxShape = static_cast<rp3d::BoxShape*>(rp3dCollider->getCollisionShape());

			rp3d::Vector3 rVec = boxShape->getHalfExtents();
			return {rVec.x, rVec.y, rVec.z};
		}
		return glm::vec3(0);
	}

	void SetScale(glm::vec3 nScale) {
		if (rp3dCollider) {
			rp3d::BoxShape* boxShape = static_cast<rp3d::BoxShape*>(rp3dCollider->getCollisionShape());
			boxShape->setHalfExtents({nScale.x,nScale.y,nScale.z});
		}
	}
private:
};

class SphereCollider : public PhysicsCollider
{
public:
	SphereCollider() {
		type = COLLIDER_SPHERE;
	}

	float GetRadius() {
		if (rp3dCollider)
		{
			return static_cast<rp3d::SphereShape*>(rp3dCollider->getCollisionShape())->getRadius();
		}
		return 0.0f;
	}

	void SetRadius(float nRadius) {
		if (rp3dCollider)
		{
			rp3d::SphereShape* sphereShape = static_cast<rp3d::SphereShape*>(rp3dCollider->getCollisionShape());
			sphereShape->setRadius(nRadius);
		}
	}

private:
};

class CapsuleCollider : public PhysicsCollider
{
public:
	CapsuleCollider() {
		type = COLLIDER_CAPSULE;
	}

	float GetRadius() {
		if (rp3dCollider)
		{
			return static_cast<rp3d::CapsuleShape*>(rp3dCollider->getCollisionShape())->getRadius();
		}
		return 0.0f;
	}

	void SetRadius(float nRadius) {
		if (rp3dCollider)
		{
			rp3d::CapsuleShape* capsuleShape = static_cast<rp3d::CapsuleShape*>(rp3dCollider->getCollisionShape());
			capsuleShape->setRadius(nRadius);
		}
	}

	float GetHeight() {
		if (rp3dCollider)
		{
			return static_cast<rp3d::CapsuleShape*>(rp3dCollider->getCollisionShape())->getHeight();
		}
		return 0.0f;
	}

	void SetHeight(float nHeight) {

		if (rp3dCollider)
		{
			rp3d::CapsuleShape* capsuleShape = static_cast<rp3d::CapsuleShape*>(rp3dCollider->getCollisionShape());
			capsuleShape->setHeight(nHeight);
		}
	}

private:

};

class TerrainCollider : public PhysicsCollider
{
public:
	TerrainCollider() {
		type = COLLIDER_TERRAIN;
	}

private:
	int rows;
	int columns;
	float min;
	float max;
	float* heights;
};