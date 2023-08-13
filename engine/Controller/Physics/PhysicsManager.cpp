#include "PhysicsManager.h"

using namespace rp3d;



PhysicsManager& PhysicsManager::Get()
{
	static PhysicsManager p_instance;
	return p_instance;
}

PhysicsManager::PhysicsManager()
{
	//rigidBodies.reserve(100);
	
	//rp3d physics world
	rp3dWorld = rp3dPhysicsCommon.createPhysicsWorld();

}

PhysicsManager::~PhysicsManager()
{
	rp3dPhysicsCommon.destroyPhysicsWorld(rp3dWorld);
}

void PhysicsManager::Update(double deltaTime)
{
	//run rp3d collision detection callback
	rp3dWorld->testCollision(mCallback);
}

void rp3dCollisionCallback::onContact(const CallbackData& callbackData)
{
	PhysicsManager& pManager = PhysicsManager::Get();

	for (int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		
	}
	
}
