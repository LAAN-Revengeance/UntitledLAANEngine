#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "RigidBody.h"
#include <map>
#include <Graphics/Graphics.h>
#include <Mesh.h>

class rp3dCollisionCallback : public rp3d::CollisionCallback {

public:
	virtual void onContact(const CallbackData& callbackData) override;
private:
};

/**
*	@Class PhysicsManager
*	@brief Singleton class used for managing rigidbodies, collisions and physics
*	resolution.
*
*	@author Andres Comeros-Ochtman, Nathan Choo
*	@version 1.0
*	@date 16/08/2023
*/
class PhysicsManager
{
public:

		/**
		*	@brief returns the current instance of physics manager
		*	@return PhysicsManager class singleton
		*/
	static PhysicsManager& Get();

		/**
		*	@brief updates the physics world
		*	@param deltaTime - time since last update
		*	@return void
		*/
	void Update(double deltaTime);

		/**
		*	@brief adds physics body to physics world, assign it to a game objects
		*	@param go - game object to add a physicsbody to
		*	@return Created physics body
		*/
	PhysicsBody* CreatePhysicsBody();

		/**
		*	@brief add sphere collider to supplied physics body
		*	@param pb - physicsbody to add sphere collider to
		*	@param radius - radius of sphere collider
		*	@return void
		*/
	void AddSphereCollider(PhysicsBody& pb, float radius);
		
		/**
		*	@brief add box collider to supplied physics body
		*	@param pb - physicsbody to add sphere collider to
		*	@param scale - x,y and z axis size of the box collider
		*	@return void
		*/
	void AddBoxCollider(PhysicsBody& pb, glm::vec3 scale);

		/**
		*	@brief add capsule collider to supplied physics body
		*	@param pb - physicsbody to add sphere collider to
		*	@param radius - radius of sphere collider
		*	@param height - height of capsule colluder
		*	@return void
		*/
	void AddCapsuleCollider(PhysicsBody& pb, float radius, float height);

		/**
		*	@brief resolves the collision between two physicsbodies
		*	@param b1 - first physicsbody
		*	@param b2 - second physicsbody
		*	@return void
		*/
	void ResolveCollision(PhysicsBody& b1, PhysicsBody& b2, float penetrationDepth, glm::vec3 contactNormal);

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

	void DeletePhysicsBody(PhysicsBody* physicsBody);

	void ResetPhysicsWorld();
private:

	//singleton
	PhysicsManager();
	~PhysicsManager();
	PhysicsManager(const PhysicsManager&) = delete;
	PhysicsManager& operator = (const PhysicsManager&) = delete;

	//ID to physics body map
	std::map<unsigned int, PhysicsBody> physicsBodies;

	//rp3d collision world
	rp3d::PhysicsCommon rp3dPhysicsCommon;
	rp3d::PhysicsWorld* rp3dWorld;
	rp3dCollisionCallback mCallback;

	//debug rendering
	Mesh* debugMesh = nullptr;
	Shader* debugShader = nullptr;

	//friendship
	friend class rp3dCollisionCallback;
};



