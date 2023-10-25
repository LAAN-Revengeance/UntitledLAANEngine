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
	*	@brief Applies a linear force as an impulse
	*	@param x - force along x axis
	*	@param y - force along y axis
	*	@param z - force along z axis
	*	@return void
	*/
	void ApplyForceImpulse(float x, float y, float z);

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
	void SetPosition(float x, float y, float z);

	/**
	*	@brief Set the rotation of object
	*	@param x - x rotation in radians
	*	@param y - y rotation in radians
	*	@param z - z rotation in radians
	*	@return void
	*/
	void SetRotationEuler(float x, float y, float z);

	/*
	*	@brief Set the orientation
	*	@param nRotation new rotation in world space
	*	@return void
	*/
	void SetRotation(glm::quat nRotation);

	/*
	*	@brief Add linear velocity
	*	@param nVel velocity vector to add to current velocity
	*	@return void
	*/
	void AddVelocity(glm::vec3 nVel);

	/*
	*	@brief Add angular velocity
	*	@param nVel velocity vector to add to current angular
	*	@return void
	*/
	void AddAngularVelocity(glm::vec3 nVel);

	/*
	*	@brief Get the current angular velocity
	*	@return current angular velocity
	*/
	glm::vec3 GetAngularVelocity();

	/*
	*	@brief Get the current linear velocity
	*	@return current linear velocity
	*/
	glm::vec3 GetVelocity();

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

	/**
	*	@brief Based on current colliders, calc and cache center of mass
	*	@return void
	*/
	void CalcCenterOfMass();

	/**
	*	@brief Get center of mass
	*	@return center of mass relative to local space
	*/
	glm::vec3 GetCenterOfMass() const;

	/**
	*	@brief Set new Mass value
	*	@param nMass new mass value
	*	@return void
	*/
	void SetMass(float nMass);

	/**
	*	@brief Set mass as infinite by setting tensor to 0 and inverse mass to 0
	*	@return void
	*/
	void SetMassInf();

	/**
	*	@brief Get Mass
	*	@return current mass
	*/
	float GetMass();

	/**
	*	@brief Get Inverse Mass
	*	@return current inverse mass
	*/
	float GetInverseMass();

	/**
	*	@brief Get Inertia tensor
	*	@return current inertia tensor
	*/
	glm::mat3 GetTensor();

	/**
	*	@brief Get inverse Inertia tensor
	*	@return current  inverse inertia tensor
	*/
	glm::mat3 GetInverseTensor();

	/**
	*	@brief Get inverse Inertia tensor in world space
	*	@return current inverse inertia tensor in world space
	*/
	glm::mat3 GetInverseTensorWorld();

	/**
	*	@brief Get matrix describing this objects rotation and translation in worldspace
	*	@return object transform matrix
	*/
	glm::mat4 GetTransformMatrix();

	/**
	*	@brief remove a collider from this body
	*	@param colliderIndex index of collider to be removed
	*	@return void
	*/
	void DeleteCollider(unsigned int colliderIndex);

	/**
	*	@brief Set the linear velocity directly
	*	@param x - x velocity
	*	@param y - y velocity
	*	@param z - z velocity
	*	@return void
	*/
	void SetVelocity(float x, float y, float z);

	/**
	*	@brief Set the angular velocity directly
	*	@param x - x angular velocity
	*	@param y - y angular velocity
	*	@param z - z angular velocity
	*	@return void
	*/
	void SetAngularVelocity(float x, float y, float z);

	/**
	*	@brief Reset force accumilators
	*	@return void
	*/
	void ClearAccumilator();

	/**
	*	@brief Set if this object is affected by gravity
	*	@param isGravity if gravity is to be used
	*	@return void
	*/
	void SetGravity(bool isGravity);

	/**
	*	@brief Get the bounce property of this object
	*	@return how bouncy this object is 1 is perfectly elastic, 0 is completly non-elastic
	*/
	float GetBounce();

	/**
	*	@brief Get linear dampening factor
	*	@return proportion if this objects linear velocity is removed per second
	*/
	float GetLinearDampening();

	/**
	*	@brief Get angular dampening factor
	*	@return proportion of this objects angular velocity is removed per second
	*/
	float GetAngularDampening();

	/**
	*	@brief Set bounce factor
	*	@param nBounce how elastic this object is from 0-1
	*	@return void
	*/
	void SetBounce(float nBounce);

	/**
	*	@brief Set linear dampening factor
	*	@parm nDamp proportion of this objects linear velocity to be removed per second
	*	@return void
	*/
	void SetLinearDampening(float nDamp);

	/**
	*	@brief Set angular dampening factor
	*	@parm nDamp proportion of this objects angular velocity to be removed per second
	*	@return void
	*/
	void SetAngularDampening(float nDamp);

		///if this object ignores external forces
	bool isKinematic = false;

		///if this object is affected by gravity
	bool useGravity = true;

private:

	//calculate the local inertia tensor
	void CalcInertiaTensor();

	//calculate the world tensor and other derived data
	void CalcDerivedData();

	//calculate and cache the transform matrix at current location and roataion
	void CalcTransformMatrix();

	//calculate the inverse world inertia tensor
	void CalcInverseTensorWorld();

	//unique ID of this physicsbody
	unsigned int ID = -1;

	//all colliders associated with this body
	std::vector<PhysicsCollider> colliders;

	//rp3d collision body, stores position and rotation in physics space.
	//also stored rp3d collider objects
	rp3d::CollisionBody* body = nullptr;

	//mass of the rigidbody
	float mass = 1;
	float inverseMass = 1/mass;
	glm::vec3 centerOfMass;

	//inverse inertia tensor
	glm::mat3 tensor;
	glm::mat3 inverseTensor;
	//inverse inertia tensor in world space
	glm::mat3 inverseTensorWorld;

	//Linear data
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 forceAccumilator;

	//rotation data
	glm::vec3 angularVelocity;
	glm::vec3 angularAcceleration;
	glm::vec3 touqueAccumilator;

	//Derived Data
	//cached transform matrix
	glm::mat4 transformMatrix;
	
	//simple approximation of drag and other slowing forces.
	//can also prevent accelleration due to precision errors
	//1, means no dampening will occur, 0 means velocity will be completely stopped.
	float dampeningLinear = 1.0f;
	float dampeningAngular = 1.0f;

	//used to calculate the restitution on contact, 0, no bounce, 1 for perfect elastic
	float bounce = 0.2f;

	//friendship <3
	friend class PhysicsManager;
	friend class SceneEditor;
	friend class PhysicsIntegrator;
};
