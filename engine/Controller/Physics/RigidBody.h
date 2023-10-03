#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "Collider.h"

/**
*	@Class PhysicsBody
*	@brief Stores collider and physics data of a single object
*
*	@author Andres Comeros-Ochtman, Nathan Choo
*	@version 1.0
*	@date 16/08/2023
*/
class PhysicsBody
{
public:

	/**
	*	@brief default constructor
	*/
	PhysicsBody();

	/**
	*	@brief destructor
	*/
	~PhysicsBody();

	/**
	*	@brief Applies linear force to the body
	*	@param x - force along x axis
	*	@param y - force along y axis
	*	@param z - force along z axis
	*	@return void
	*/
	void ApplyForce(float x, float y, float z);

	/**
	*	@brief Applies tourque to the body
	*	@param x - tourque along x axis
	*	@param y - tourque along y axis
	*	@param z - tourque along z axis
	*	@return void
	*/
	void ApplyTorque(float x, float y, float z);


	/**
	*	@brief return current position
	*	@return position of object
	*/
	glm::vec3 GetPosition();

	/**
	*	@brief return current rotation
	*	@return rotation in euler angles
	*/
	glm::vec3 GetRotation();

	/**
	*	@brief Set the position of object
	*	@param x - x position
	*	@param y - y position
	*	@param z - z position
	*	@return void
	*/
	void SetPosition(float x, float y, float z);

	/**
	*	@brief Set the rotation of object
	*	@param x - x rotation in radians
	*	@param y - y rotation in radians
	*	@param z - z rotation in radians
	*	@return void
	*/
	void SetRotation(float x, float y, float z);

	/**
	*	@brief return collider infor
	*	@param colliderIndex index of this collder.
	*	@return collider info at colliderIndex
	*/
	PhysicsCollider& GetCollider(unsigned int colliderIndex);

	/**
	*	@brief return number of colliders
	*	@return number of colliders this physicsBody has
	*/
	unsigned int GetNumColliders();
	/**
	*	@brief return physicsbody ID in physics world
	*	@return ID of physics body
	*/
	unsigned int GetID();

	//returns this objects local center of mass
	glm::vec3 GetCenterOfMass() const;

	void DeleteCollider(unsigned int colliderIndex);

		//if this object ignores external forces
	bool isKinematic = false;

	friend class PhysicsManager;

private:

	unsigned int ID = -1;


	std::vector<PhysicsCollider> colliders;
	//rp3d collision body, stores position and rotation in physics space.
	//also stored rp3d collider objects
	rp3d::CollisionBody* body = nullptr;

	//mass of the rigidbody
	float mass = 1;
	glm::vec3 centerOfMass;


	friend class SceneEditor;
};
