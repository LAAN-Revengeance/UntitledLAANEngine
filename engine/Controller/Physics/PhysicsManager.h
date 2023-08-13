#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "RigidBody.h"
#include <GameObject.h>

class rp3dCollisionCallback : public rp3d::CollisionCallback {

public:
	virtual void onContact(const CallbackData& callbackData) override;
private:
};

class PhysicsManager
{
public:

	static PhysicsManager& Get();

	void Update(double deltaTime);
	
private:

	//singleton
	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager&) = delete;
	PhysicsManager& operator = (const PhysicsManager&) = delete;

	//TODO: find out all data needed to resolve a collision.
	void ResolveCollision();

	//trying to keep contigous memry for faster physics processing(no pointers)
	std::vector<PhysicsBody> rigidBodies;

	//rp3d collision world
	rp3d::PhysicsCommon rp3dPhysicsCommon;
	rp3d::PhysicsWorld* rp3dWorld;
	rp3dCollisionCallback mCallback;
};



