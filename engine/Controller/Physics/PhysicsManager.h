#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "RigidBody.h"
#include <GameObject.h>
#include <map>

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
	
	//add physics body to physics world, assign it to a game object
	PhysicsBody& AddPhysicsBody(GameObject& go);

	//TODO: find out all data needed to resolve a collision.
	void ResolveCollision(PhysicsBody& b1, PhysicsBody& b2);

	PhysicsBody& GetPhysicsBody(unsigned int id);
private:

	//singleton
	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager&) = delete;
	PhysicsManager& operator = (const PhysicsManager&) = delete;
	
	//ID to physics body map
	std::map<unsigned int,PhysicsBody> physicsBodies;

	//rp3d collision world
	rp3d::PhysicsCommon rp3dPhysicsCommon;
	rp3d::PhysicsWorld* rp3dWorld;
	rp3dCollisionCallback mCallback;
};



