#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "RigidBody.h"
#include <GameObject.h>


class CollisionListener;

class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Update(double deltaTime);

private:

	//trying to keep contigous memry for faster physics processing(no pointers)
	std::vector<RigidBody> rigidBodies;

	//rp3d collision world
	rp3d::PhysicsCommon rp3dPhysicsCommon;
	rp3d::PhysicsWorld* rp3dWorld;

	CollisionListener collisionListener;
};


class CollisionListener : public rp3d::EventListener
{
public:
	virtual void onContact(const CollisionCallback::CallbackData& callbackData) override {}
private:

};

