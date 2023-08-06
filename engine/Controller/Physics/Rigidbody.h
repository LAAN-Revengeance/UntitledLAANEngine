#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Collider.h"

// RigidBody type
#define KINE 1
#define STAT 2
#define DYNA 3

class Physics;
class GameObject;
class Terrain;

/**
*	@class Rigidbody
*	@brief contains basic function to modify and control rigidbodies
*	@version 1.5
*	@author Nathan Choo /  Andres Comeros-Ochtman
*	@date 19/04/2023
*/


/**
*	@class CollisionListener
*	@brief listens for collision contacts for the specific object
*	@version 1.0
*	@author Nathan Choo
*	@date 21/5/2023
*/
class CollisionListener : public rp3d::EventListener
{
public:
	virtual void onContact(const CollisionCallback::CallbackData& callbackData) override
	{
		if (contactListenerState)
		{
			for (int p = 0; p < callbackData.getNbContactPairs(); p++) {
			
				CollisionCallback::ContactPair contactPair = callbackData.getContactPair(p);

				if (contactPair.getEventType() == CollisionCallback::ContactPair::EventType::ContactStart)
				{
					if (contactPair.getBody1()->getEntity().id == ID || contactPair.getBody2()->getEntity().id == ID) {
						isContact = true;
					}
				}

				if (contactPair.getEventType() == CollisionCallback::ContactPair::EventType::ContactExit)
				{
					if (contactPair.getBody1()->getEntity().id == ID || contactPair.getBody2()->getEntity().id == ID) {
						isContact = false;
					}
				}
			}
		}
	}

	void ToggleState() {contactListenerState = !contactListenerState;}

	bool isContact = false;
	int ID = -1;
private:
	bool contactListenerState = false;
	
};

class Rigidbody
{
public:
		/**
		*	@brief default constructor
		*/
	Rigidbody();

		/**
		*	@brief applies vector force to rigidbody
		*   @param force the direction and magnitude of the force
		*	@return void
		*/
	void ApplyForce(glm::vec3 force);

		/**
		*	@brief applies vector force to rigidbody
		*   @param force the direction and magnitude of the force
		*	@return void
		*/
	void ApplyForceLocal(glm::vec3 force);

		/**
		*	@brief applies a torque force to the rigidbody
		*   @param torque torque to apply to rigidbody
		*	@return void
		*/
	void ApplyTorqueLocal(glm::vec3 torque);

		/**
		*	@brief modifies type of the rigidbody
		*   @param type int corresponding to the desire rigidbody type
		*	@return void
		*/
	void ModType(int type);

		/**
		*	@brief enables or disables gravity for rigidbodies
		*   @param isGravity determines whether gravity is on(true) or off(false)
		*	@return void
		*/
	void SetUseGravity(bool isGravity);

		/**
		*	@brief set the world position of the rigidbody
		*   @param newPos the intended position to set
		*	@return void
		*/
	void SetPosition(glm::vec3 newPos);

		/**
		*	@brief set the rotation of the rigidbody
		*   @param newRot the intended rotation to set
		*	@return void
		*/
	void SetRotation(glm::vec3 newRot);

		/**
		*	@brief returns world position of the rigidbody
		*	@return glm::vec3
		*/
	glm::vec3 GetPosition();

		/**
		*	@brief sets the mass of the rigidbody
		*   @param nMass the new mass of the rigidbody
		*	@return void
		*/
	void SetMass(float nMass);

		/**
		*	@brief sets the position of the center of mass
		*   @param center the coordinates of the new center of mass
		*	@return void
		*/
	void SetCenterOfMass(glm::vec3 center);

		/**
		*	@brief sets the linear dampening value
		*   @param damp the value to dampen by
		*	@return void
		*/
	void SetDampeningLinear(float damp);

		/**
		*	@brief sets the angular dampen value
		*   @param damp the value to dampen by
		*	@return void
		*/
	void SetDampeningAngle(float damp);

		/**
		*	@brief restricts the linear motion of a rigidbody in worldspace
		*   @param x determines motion along x axis
		*   @param y determines motion along y axis
		*   @param z determines motion along z axis
		*	@return void
		*/
	void SetAxisLinearFactor(float x, float y, float z);

		/**
		*	@brief restricts the angular motion of a rigidbody in worldspace
		*   @param x determines motion along x axis
		*   @param y determines motion along y axis
		*   @param z determines motion along z axis
		*	@return void
		*/
	void SetAxisAngleFactor(float x, float y, float z);

		/**
		*	@brief restricts the linear motion of a rigidbody in worldspace
		*   @param x determines motion along x axis
		*   @param y determines motion along y axis
		*   @param z determines motion along z axis
		*	@return void
		*/
	void SetLinearVelocity(float x, float y, float z);
		/**
		*	@brief restricts the linear motion of a rigidbody in worldspace
		*   @param x determines motion along x axis
		*   @param y determines motion along y axis
		*   @param z determines motion along z axis
		*	@return void
		*/
	void SetAngularVelocity(float x, float y, float z);

		/**
		*	@brief returns the rigidbodies velocity along the x,y and z axis
		*	@return vec3 - Current linear velocity
		*/
	glm::vec3 GetLinearVelocty();

		/**
		*	@brief returns the rigidbodies rotation velocity along the x,y and z axis
		*	@return vec3 - Current angular velocity
		*/
	glm::vec3 GetAngularVelocity();

	void SetColliderBox		(float mass, float bounce, float friction, glm::vec3 offset, glm::vec3 rotation, glm::vec3 scale);
	void SetColliderSphere	(float mass, float bounce, float friction, glm::vec3 offset, glm::vec3 rotation, float radius);
	void SetColliderCapsule	(float mass, float bounce, float friction, glm::vec3 offset, glm::vec3 rotation, float radius, float height);

		/**
		*	@brief returns the rigidbodies collider pointer
		*	@return PhysicsCollider* - pointer of the collider
		*/
	PhysicsCollider* GetCollider();

		/**
		*	@brief returns the rigidbody type
		*	@return int
		*/
	int GetModType();
		/**
		*	@brief returns the rigidbodies mass
		*	@return float
		*/
	float GetMass();
		/**
		*	@brief returns the rigidbodies linear dampening value
		*	@return float
		*/
	float GetDampeningLinear();
		/**
		*	@brief returns the rigidbodies dampening angle value
		*	@return float
		*/
	float GetDampeningAngle();
		/**
		*	@brief returns the rigidbodies center of mass in coordinates
		*	@return glm::vec3
		*/
	glm::vec3 GetCenterOfMass();
		/**
		*	@brief returns the rigidbodies linear factor
		*	@return glm::vec3
		*/
	glm::vec3 GetAxisLinearFactor();
		/**
		*	@brief returns the rigidbodies axis angle value
		*	@return float
		*/
	glm::vec3 GetAxisAngleFactor();
		/**
		*	@brief returns the rigidbodies current collision contact state
		*	@return bool - true = rigidbody is in contact
		*/
	bool GetIsContactListen();

		/**
		*	@brief enables the listener for the rigidbody
		*	@return void
		*/
	void ToggleContactListenState()
	{
		if (!registered) {
			worldPtr->setEventListener(&collideListen);
			registered = true;
		}

		collideListen.ToggleState();
	}
	
	bool GetIsContact() 
	{
		return collideListen.isContact;
	}

	friend class Physics;
	friend class GameObject;
	friend class Terrain;

private:
	int mod = 1;
	rp3d::RigidBody* rbPtr = nullptr;
	rp3d::PhysicsWorld* worldPtr = nullptr;

	bool registered = false;
	CollisionListener collideListen;

	PhysicsCollider* collider = new PhysicsCollider;
};