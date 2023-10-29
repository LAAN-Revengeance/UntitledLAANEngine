#include "Collisionsolver.h"
#include <iomanip>
void CollisionSolver::ResolveCollisions(CollisionData& cd)
{
	ResolvePenetrationLinear(cd);
	ResolveImpulse(cd);
}

void CollisionSolver::ResolvePenetrationLinear(CollisionData& cd)
{
	PhysicsBody* b1 = cd.b1;
	PhysicsBody* b2 = cd.b2;

	glm::vec3 totalCorrectionB1(0.0f);
	glm::vec3 totalCorrectionB2(0.0f);

	float totalMassInverse = b1->GetInverseMass() + b2->GetInverseMass();

	if (totalMassInverse <= 0) return;// both have infinite mass

	// Accumulate corrections based on each contact point.
	for (size_t i = 0; i < cd.numContacts; i++)
	{
		const ContactPointData& contact = cd.contacts[i];

		if (contact.penetrationDepth > 0) {
			glm::vec3 movePerMass = contact.contactNormal * (-contact.penetrationDepth / totalMassInverse);

			totalCorrectionB1 -= (movePerMass * b1->GetInverseMass());
			totalCorrectionB2 += (movePerMass * b2->GetInverseMass());
		}
	}

	//Average out the corrections
	totalCorrectionB1 = (totalCorrectionB1 / static_cast<float>(cd.numContacts));
	totalCorrectionB2 = (totalCorrectionB2 / static_cast<float>(cd.numContacts));

	glm::vec3 nb1Pos = b1->GetPosition() + (totalCorrectionB1);
	glm::vec3 nb2Pos = b2->GetPosition() + (totalCorrectionB2);

	b1->SetPosition(nb1Pos.x, nb1Pos.y, nb1Pos.z);
	b2->SetPosition(nb2Pos.x, nb2Pos.y, nb2Pos.z);
}

void CollisionSolver::ResolveImpulse(CollisionData& cd)
{
	PhysicsBody* b1 = cd.b1;
	PhysicsBody* b2 = cd.b2;

	float restitution = (b1->GetBounce() + b2->GetBounce())/2.0f;

	float totalInverseMass = b1->GetInverseMass() + b2->GetInverseMass();
	if (totalInverseMass <= 0) return;

	glm::vec3 totalLinearB1(0.0f), totalLinearB2(0.0f);
	glm::vec3 totalAngularB1(0.0f), totalAngularB2(0.0f);

	for (size_t i = 0; i < cd.numContacts; i++)
	{
		ContactPointData& contact = cd.contacts[i];

		
		glm::vec3 b1Relative = contact.contactWorldSpace - b1->GetPosition();
		glm::vec3 b2Relative = contact.contactWorldSpace - b2->GetPosition();

		glm::vec3 velocityb1 = b1->GetVelocity() + glm::cross(b1->GetAngularVelocity(), b1Relative);
		glm::vec3 velocityb2 = b2->GetVelocity() + glm::cross(b2->GetAngularVelocity(), b2Relative);

		//calculate the contect velocity as the point of collision
		glm::vec3 contactVel = velocityb2 - velocityb1;
		
		
		//calc impulse force along contact normal
		float impulseForce = glm::dot(contactVel, contact.contactNormal);

		//Set angular velocity to 0 if impulse is low. used to avoid jitter
		if (impulseForce < 0.005) {
			b1->SetAngularVelocity(0, 0, 0);
			b2->SetAngularVelocity(0, 0, 0);
		}

		glm::vec3 inertiaB1 = glm::cross((b1->GetInverseTensorWorld() * glm::cross(b1Relative, contact.contactNormal)), b1Relative);
		glm::vec3 inertiaB2 = glm::cross((b2->GetInverseTensorWorld() * glm::cross(b2Relative, contact.contactNormal)), b2Relative);

		float angularEffect = glm::dot(inertiaB1 + inertiaB2, contact.contactNormal);

		// Calculate the final scalar impulse
		float impulse = (-(1.0f + restitution) * impulseForce) / (totalInverseMass + angularEffect);
		

		// Apply the impulse to the contact normal
		glm::vec3 fullImpulse = impulse * contact.contactNormal;

		// Accumulate the impulses to apply to the bodies' linear and angular velocities
		totalLinearB1 += -fullImpulse * b1->GetInverseMass();
		totalLinearB2 +=  fullImpulse * b2->GetInverseMass();

		totalAngularB1 += b1->GetInverseTensorWorld() * glm::cross(b1Relative, -fullImpulse);
		totalAngularB2 += b2->GetInverseTensorWorld() * glm::cross(b2Relative,  fullImpulse);
	}

	b1->AddVelocity(totalLinearB1);
	b2->AddVelocity(totalLinearB2);

	b1->AddAngularVelocity(totalAngularB1);
	b2->AddAngularVelocity(totalAngularB2);
}


void CollisionSolver::PrintTensors(CollisionData& cd)
{
	PhysicsBody* b1 = cd.b1;
	PhysicsBody* b2 = cd.b2;
	std::cout << b1->GetID() << " Tensor:\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << std::fixed << std::setprecision(4) << b1->GetInverseTensor()[i][j] << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
	std::cout << b2->GetID() << " Tensor:\n";
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			std::cout << std::fixed << std::setprecision(4) << b2->GetInverseTensor()[i][j] << " | ";
		}
		std::cout << "\n";
	}
	std::cout << "-------\n";
}
