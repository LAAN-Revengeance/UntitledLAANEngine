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
	glm::vec3 GetPosition() const;

	/**
	*	@brief return current orentation
	*	@return orientation in as quaternion
	*/
	glm::quat GetRotation() const;

	/**
	*	@brief Set the position of object
	*	@param x - x position
	*	@param y - y position
	*	@param z - z position
	*	@return void
	*/
	void SetPosition(glm::vec3 newPosition);

	/**
	*	@brief Set the rotation of object
	*	@param x - x rotation in radians
	*	@param y - y rotation in radians
	*	@param z - z rotation in radians
	*	@warning try to avoid using this unstable function
	*	@return void
	*/
	void SetRotationEuler(float x, float y, float z);

	/*
	*	@brief set the rotation of the object
	*	@param nRotation new rotation as quaternion
	*	@return void
	*/
	void SetRotation(glm::quat nRotation);

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
	unsigned int GetNumColliders() const;
	/**
	*	@brief return physicsbody ID in physics world
	*	@return ID of physics body
	*/
	unsigned int GetID();

	void SetMass(float nMass);

	float GetMass();

	float GetInverseMass();

	void DeleteCollider(unsigned int colliderIndex);

	void SetVelocity(glm::vec3 newVelocity);

	void ClearAccumilator();

	void SetGravity(bool);

		//if this object ignores external forces
	bool isKinematic = false;

	bool useGravity = true;

	friend class PhysicsManager;
	friend class SceneEditor;

	glm::vec3 GetVelocity();

	glm::vec3 GetAngularVelocity();

	void SetAngularVelocity(glm::vec3 newVelocity);
	
	glm::mat3 GetInertiaTensor();

	glm::mat3 GetInverseInertiaTensor();

	void SetIntertiaTensor(glm::mat3 tensor);

	void CalculateInertiaTensor();

	glm::vec3 gravity = { 0, -9.8, 0 };

	void SetOrientation(glm::quat newOrientation);

	glm::quat GetOrientation();

	void SetIsKinematic(bool value);

	void UpdateBody();

	glm::quat orientation;

	glm::vec3 position;
private:
	unsigned int ID = -1;

	std::vector<PhysicsCollider> colliders;

	//rp3d collision body, stores position and rotation in physics space.
	//also stored rp3d collider objects
	rp3d::CollisionBody* body = nullptr;

	//mass of the rigidbody
	float mass = 1;

	glm::vec3 velocity;

	glm::vec3 angularVelocity;

	glm::mat3 inertiaTensor = {};

	glm::mat3 inverseInertiaTensor = {};
};
