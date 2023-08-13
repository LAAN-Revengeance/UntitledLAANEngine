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

private:

	//rp3d collision body, stores position and rotation in physics space.
	//also stored rp3d collider objects
	rp3d::CollisionBody* body;

	
	////position and rotation of the rigidbody
	//glm::vec3 position;
	//glm::vec3 rotation;

	//mass of the rigidbody
	float mass;
};
