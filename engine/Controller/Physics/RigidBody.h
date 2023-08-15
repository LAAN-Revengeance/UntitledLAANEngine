#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>

class PhysicsBody
{
public:
	PhysicsBody();
	~PhysicsBody();

	void ApplyForce(float x, float y, float z);
	void ApplyTorque(float x, float y, float z);

	//get position and rotation of rigidbody
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();

	//set position and rotation of rigidbody
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	//get/set collider fucntions go here


	void SetID(unsigned int nID);
	unsigned int GetID();

private:

	unsigned int ID = -1;

	//rp3d collision body, stores position and rotation in physics space.
	//also stored rp3d collider objects
	rp3d::CollisionBody* body;

	//mass of the rigidbody
	float mass = 1;
};
