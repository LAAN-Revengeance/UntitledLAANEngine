#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>

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

	void ToggleState() { contactListenerState = !contactListenerState; }

	bool isContact = false;
	int ID = -1;
private:
	bool contactListenerState = false;

};

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	void ApplyForce(float x, float y, float z);
	void ApplyTorque(float x, float y, float z);

	//get position and rotation of rigidbody
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();

	//set position and rotation of rigidbody
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);


private:
	//position and rotation of the rigidbody
	glm::vec3 position;
	glm::vec3 rotation;

	//mass of the rigidbody
	float mass;

	//drag and angular drag of the rigidbody
	float drag;
	float angularDrag;
};
