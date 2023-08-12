#include "PhysicsManager.h"


PhysicsManager::PhysicsManager()
{
	rigidBodies.reserve(100);
	
	//rp3d physics world
	rp3dWorld = rp3dPhysicsCommon.createPhysicsWorld();

}

PhysicsManager::~PhysicsManager()
{
	rp3dPhysicsCommon.destroyPhysicsWorld(rp3dWorld);
}

void PhysicsManager::Update(double deltaTime)
{
	rp3dWorld->testCollision(collisionListener);
	
}
