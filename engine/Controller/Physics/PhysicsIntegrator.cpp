#include "PhysicsIntegrator.h"

glm::vec3 PhysicsIntegrator::gravity = {0, -9.805,0};

void PhysicsIntegrator::IntergrateLinear(PhysicsBody& pb, float deltaTime)
{	
	//calculate linear acceleration
	glm::vec3 totalAcceleration = pb.acceleration;

	//apply gravity if object uses it or doesnt have infinite mass
	if (pb.useGravity && pb.GetInverseMass() > 0)
		totalAcceleration += gravity;

	totalAcceleration += pb.inverseMass * pb.forceAccumilator;
	pb.acceleration = { 0,0,0 };

	//calculate velocity and add simulated drag
	pb.velocity += totalAcceleration * deltaTime;
	pb.velocity *= powf(pb.dampeningLinear, deltaTime);

	//update position
	glm::vec3 newPos = pb.GetPosition() + pb.velocity * deltaTime;
	pb.SetPosition(newPos.x, newPos.y, newPos.z);
}

void PhysicsIntegrator::IntergrateRotational(PhysicsBody& pb, float deltaTime)
{
	//calculate angular acceleration
	glm::vec3 totalAngularAcceleration = pb.angularAcceleration;
	totalAngularAcceleration += pb.inverseTensorWorld * pb.touqueAccumilator;
	pb.angularAcceleration = { 0,0,0 };

	//calculate angular velocity and add simulated drag
	pb.angularVelocity = pb.angularVelocity + (totalAngularAcceleration * deltaTime);
	pb.angularVelocity *= powf(pb.dampeningAngular, deltaTime);

	//update rotation
	glm::quat nRotation = pb.GetRotation() + (glm::quat(0.0f, (pb.angularVelocity * deltaTime * 0.5f)) * pb.GetRotation());
	pb.SetRotation(glm::normalize(nRotation));

}

void PhysicsIntegrator::Integrate(PhysicsBody& pb, float deltaTime)
{
	if (pb.isKinematic)
		return;

	//intergrate position and rotation
	IntergrateLinear(pb,deltaTime);
	IntergrateRotational(pb, deltaTime);
	

	//clear accumilator and cache derived data
	pb.CalcDerivedData();
	pb.ClearAccumilator();
}


PhysicsIntegrator::PhysicsIntegrator()
{
}

PhysicsIntegrator::~PhysicsIntegrator()
{
}