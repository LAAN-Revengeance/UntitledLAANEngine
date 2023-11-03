#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "RigidBody.h"
#include <map>
#include <Graphics/Graphics.h>
#include <Mesh.h>
#include "PhysicsIntegrator.h"
#include "Collisionsolver.h"
#include <Utils/DebugLogger.h>
#include <GameObjects/NPC_GameObject.h>	

class rp3dCollisionCallback : public rp3d::CollisionCallback {

public:
	void SetPhysicsManager(PhysicsManager* pManager) {
		physicsManager = pManager;
	}
	virtual void onContact(const CallbackData& callbackData) override;
private:
	PhysicsManager* physicsManager;
};

class rp3dRaycastCallback : public rp3d::RaycastCallback {

public:
	virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) {
		_hitID = info.body->getEntity().id;
		return rp3d::decimal(0.0);
		
	}
	unsigned int _hitID;
};

/**
*	@Class PhysicsManager
*	@brief Singleton class used for managing rigidbodies, collisions and physics
*	resolution. acts as an abstraction of the physics world.
*
*	@author Andres Comeros-Ochtman, Nathan Choo
*	@version 1.0
*	@date 16/08/2023
*/
class PhysicsManager
{
public:

	PhysicsManager();
	~PhysicsManager();
		/**
		*	@brief updates the physics world
		*	@param deltaTime - time since last update
		*	@return void
		*/
	void Update(double deltaTime);

		/**
		*	@brief adds physics body to physics world, assign it to a game object
		*	@param go - game object to add a physicsbody to
		*	@return Created physics body
		*/
	PhysicsBody* CreatePhysicsBody(GameObject* go);

		/**
		*	@brief add sphere collider to supplied physics body
		*	@param pb - physicsbody to add sphere collider to
		*	@param radius - radius of sphere collider
		*	@return void
		*/
	SphereCollider* AddSphereCollider(PhysicsBody& pb, float radius);

		/**
		*	@brief add box collider to supplied physics body
		*	@param pb - physicsbody to add sphere collider to
		*	@param scale - x,y and z axis size of the box collider
		*	@return void
		*/
	BoxCollider* AddBoxCollider(PhysicsBody& pb, glm::vec3 scale);

		/**
		*	@brief add capsule collider to supplied physics body
		*	@param pb - physicsbody to add sphere collider to
		*	@param radius - radius of sphere collider
		*	@param height - height of capsule colluder
		*	@return void
		*/
	CapsuleCollider* AddCapsuleCollider(PhysicsBody& pb, float radius, float height);

		/**
		*	@brief Get a physics body based on its ID
		*	@param id - ID of physics body
		*	@return Physics body with ID id
		*/
	PhysicsBody& GetPhysicsBody(unsigned int id);

		/**
		*	@brief Draw debug rendering of the physics world to currently bound draw buffer
		*	@param camera - camera data used to render the physics debug data
		*	@return void
		*/
	void DrawPhysicsWorld(Camera& camera);

		/**
		*	@brief Delete a physicsbody from the physics world
		*	@param physicsBody reference to body being deleted
		*	@return void
		*/
	void DeletePhysicsBody(PhysicsBody* physicsBody);

		/**
		*	@brief Reset the state of the physics world
		*	@return void
		*/
	void ResetPhysicsWorld();

		/**
		 *	@brief Get a poionter to an object intersected by a ray.
		 *	@param origin start location of ray
		 *	@param direction forward direction of ray
		 *	@param distance length of ray
		 *	@return reference to the first rigidbody intersected by the ray
		*/
	GameObject* Raycast(glm::vec3 origin, glm::vec3 direction, float distance = FLT_MAX);
	NPC* RaycastNPC(glm::vec3 origin, glm::vec3 direction, float distance = FLT_MAX);
private:

	//ID to physics body map
	std::map<unsigned int, PhysicsBody> physicsBodies;

	//ID to game object map
	std::map<unsigned int, GameObject*> gameObjects;

	//rp3d collision world
	rp3d::PhysicsCommon rp3dPhysicsCommon;
	rp3d::PhysicsWorld* rp3dWorld;
	rp3dCollisionCallback mCallback;

	//debug rendering
	Mesh* debugMesh = nullptr;
	Shader* debugShader = nullptr;

	//timers stuff
	double accumilator = 0.0;

	//friendship
	friend class rp3dCollisionCallback;
};



