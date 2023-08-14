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

void PhysicsManager::ResolveCollision(PhysicsBody& b1, PhysicsBody& b2)
{

}

PhysicsBody& PhysicsManager::GetPhysicsBody(unsigned int id)
{
	return physicsBodies.at(id);
}

void PhysicsManager::Update(double deltaTime)
{
	//run rp3d collision detection callback
	rp3dWorld->testCollision(mCallback);
}

PhysicsBody& PhysicsManager::AddPhysicsBody(GameObject& go)
{
	//add collision body to react and get its ID
	rp3d::Vector3 pos(0.0, 0.0, 0.0);
	rp3d::CollisionBody* bPtr;
	bPtr = rp3dWorld->createCollisionBody({ {0,0,0},rp3d::Quaternion::identity() });
	unsigned int id = bPtr->getEntity().id;

	//create a physics body and set ID.
	PhysicsBody pb;
	pb.SetID(id);
	physicsBodies.insert({id,pb});

	return physicsBodies.at(id);
}

void rp3dCollisionCallback::onContact(const CallbackData& callbackData)
{
	PhysicsManager& pManager = PhysicsManager::Get();

	for (int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		unsigned int id1 = callbackData.getContactPair(i).getBody1()->getEntity().id;
		unsigned int id2 = callbackData.getContactPair(i).getBody1()->getEntity().id;
		pManager.ResolveCollision(pManager.GetPhysicsBody(id1), pManager.GetPhysicsBody(id1));
	}
}
