#include "PhysicsManager.h"

using namespace rp3d;

PhysicsManager::PhysicsManager()
{
	rp3dWorld = rp3dPhysicsCommon.createPhysicsWorld();
	mCallback.SetPhysicsManager(this);
}

PhysicsManager::~PhysicsManager()
{
	rp3dPhysicsCommon.destroyPhysicsWorld(rp3dWorld);
}

PhysicsBody* PhysicsManager::GetPhysicsBody(unsigned int id)
{
	return &physicsBodies.at(id);
}

void PhysicsManager::Update(float deltaTime)
{
	//run rp3d collision detection callback
	rp3dWorld->testCollision(mCallback);
	
	// Set linear and angular velocity of physics bodies that are calculated within the onContact function 
	for (auto& physicsBody : physicsBodies)
	{
		// If physics body has gravity enabled, exert downward force on it
		if (physicsBody.second.useGravity)
		{
			physicsBody.second.SetVelocity(physicsBody.second.GetVelocity() + (deltaTime * physicsBody.second.gravity));
		}	

		physicsBody.second.CalculateDamping(deltaTime);
		physicsBody.second.CalculateInertiaTensor();	
		// Uncomment these lines for a scuffed method of making bodies slow down and eventually stop
		//glm::vec3 friction = { 0.99, 0.99, 0.99 };
		//physicsBody.second.SetVelocity(physicsBody.second.GetVelocity() * friction);
		//physicsBody.second.SetAngularVelocity(physicsBody.second.GetAngularVelocity() * friction);

		// Actual physics updates for velocity and rotation
		physicsBody.second.SetPosition(physicsBody.second.GetPosition() + physicsBody.second.GetVelocity() * deltaTime);
		physicsBody.second.SetOrientation(glm::normalize(physicsBody.second.GetOrientation() + ((0.5f * glm::quat(0.0, physicsBody.second.GetAngularVelocity()) * physicsBody.second.GetOrientation()) * deltaTime)));

		physicsBody.second.UpdateBody();
	}
}

PhysicsBody* PhysicsManager::CreatePhysicsBody()
{
	//add collision body to react and get its ID
	rp3d::Vector3 pos(0.0, 0.0, 0.0);
	rp3d::CollisionBody* bPtr;
	bPtr = rp3dWorld->createCollisionBody({ {0,0,0},rp3d::Quaternion::identity() });
	unsigned int id = bPtr->getEntity().id;

	//create a physics body and set ID.
	PhysicsBody pb;
	pb.body = bPtr;
	pb.ID = id;
	physicsBodies.insert({id,pb});

	return &physicsBodies.at(id);
}

void PhysicsManager::AddSphereCollider(PhysicsBody& pb, float radius)
{
	SphereShape* shape = rp3dPhysicsCommon.createSphereShape(radius);
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	SphereCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
}

void PhysicsManager::AddBoxCollider(PhysicsBody& pb, glm::vec3 scale)
{
	BoxShape* shape = rp3dPhysicsCommon.createBoxShape(Vector3(scale.x, scale.y, scale.z));
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	BoxCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
}

void PhysicsManager::AddCapsuleCollider(PhysicsBody& pb, float radius, float height)
{
	CapsuleShape* shape = rp3dPhysicsCommon.createCapsuleShape(radius, height);
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	CapsuleCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
}

void PhysicsManager::DrawPhysicsWorld(Camera& camera)
{
	if (!debugShader)
		debugShader = new Shader("resources/shaders/Physics_Debug/Physics.vert", "resources/shaders/Physics_Debug/Physics.frag", "");

	// Enable debug rendering 
	rp3dWorld->setIsDebugRenderingEnabled(false);

	//needs an update to generate verts
	rp3dWorld->update(0.1f);

	// Get a reference to the debug renderer 
	DebugRenderer& debugRenderer = rp3dWorld->getDebugRenderer();
	// Select the contact points and contact normals to be displayed 
	debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_POINT, true);
	debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::CONTACT_NORMAL, true);
	debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLISION_SHAPE, true);
	debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLIDER_AABB, true);
	debugRenderer.setIsDebugItemDisplayed(DebugRenderer::DebugItem::COLLIDER_BROADPHASE_AABB, true);

	int nLines = debugRenderer.getNbLines();
	int nTri = debugRenderer.getNbTriangles();

	if (nTri > 0) {
		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glClearColor(1, 0, 0, 0);
		//set model matrix uniforms
		glm::mat4 modelMat(1.0f);

		const reactphysics3d::DebugRenderer::DebugTriangle* tri = debugRenderer.getTrianglesArray();

		if (debugMesh)
			debugMesh->FreeData();

		debugMesh = new Mesh();
		glm::mat4 proj = camera.GetProjection();
		glm::mat4 view = camera.GetView();

		debugShader->SetUniform("model", modelMat);

		debugMesh->SetDebugVertexData((float*)&tri->point1.x, nTri * 3);
		debugMesh->Render(proj, view, debugShader, false, GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
}

void PhysicsManager::DeletePhysicsBody(PhysicsBody* physicsBody)
{
	if(physicsBody)
		rp3dWorld->destroyCollisionBody(physicsBody->body);
}

void PhysicsManager::ResetPhysicsWorld()
{
	for (auto& it : physicsBodies)
	{
		DeletePhysicsBody(&it.second);
	}
	physicsBodies.clear();

	while (rp3dWorld->getNbRigidBodies() > 0) {
		rp3d::RigidBody* body = rp3dWorld->getRigidBody(0);
		rp3dWorld->destroyCollisionBody(body);
		delete body;
	}
	rp3dPhysicsCommon.destroyPhysicsWorld(rp3dWorld);
	rp3dWorld = rp3dPhysicsCommon.createPhysicsWorld();
}

void ResolvePenetrations(float penetration, PhysicsBody* body1Ptr, PhysicsBody* body2Ptr, glm::vec3 bodyContactNormal)
{
	// Resolve penetration between the two colliding bodies
	// Only occurs if the body is not kinematic, to prevent bodies that shouldn't be moving from still moving due to penetration
	//TO DO: Causing an issue with straight on collisions, penetration pushes bodies away from each other causing flat items to not push each other properly
	if (!body1Ptr->isKinematic)
	{
		body1Ptr->SetPosition(body1Ptr->GetPosition() + ((-(penetration / 2)) * bodyContactNormal));
	}
	if (!body2Ptr->isKinematic)
	{
		body2Ptr->SetPosition(body2Ptr->GetPosition() - ((-(penetration / 2)) * bodyContactNormal));
	}
}

void rp3dCollisionCallback::onContact(const CallbackData& callbackData)
{			
	for (int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		const auto& contactPair  = callbackData.getContactPair(i);

		// Id of each physics body involved in the collision
		unsigned int id1 = contactPair.getBody1()->getEntity().id;
		unsigned int id2 = contactPair.getBody2()->getEntity().id;

		//if physics bodies exist, resolve collision.
		auto rbMap = &physicsManager->physicsBodies;
		if (rbMap->find(id1) != rbMap->end() && rbMap->find(id2) != rbMap->end()) {
			
			for (int x = 0; x < contactPair.getNbContactPoints(); x++)
			{
				ContactPoint contactPoint = contactPair.getContactPoint(x);
				// Penetration depth between the two colliding bodies
				float penetration = contactPoint.getPenetrationDepth();
				
				// Contact normal vector at the point of contact
				rp3d::Vector3 contactNormal = contactPair.getContactPoint(x).getWorldNormal();
				
				// Pointer to physics bodies involved in the collision
				PhysicsBody* body1Ptr = physicsManager->GetPhysicsBody(id1);
				PhysicsBody* body2Ptr = physicsManager->GetPhysicsBody(id2);
				
				// Converting contact normal vector to glm
				glm::vec3 bodyContactNormal = body1Ptr->Convertrp3dVector3ToGlm(contactNormal);

				// Contact point on each body involved in the collision
				rp3d::Vector3 body1Contact = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
				rp3d::Vector3 body2Contact = contactPair.getCollider2()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider2();
				
				ResolvePenetrations(penetration, body1Ptr, body2Ptr, bodyContactNormal);

				// Converting from rp3d::Vector3 to glm
				glm::vec3 body1ContactPoint = body1Ptr->Convertrp3dVector3ToGlm(body1Contact);
				glm::vec3 body2ContactPoint = body1Ptr->Convertrp3dVector3ToGlm(body2Contact);

				// Value for the ratio of initial and final speed for the colliding bodies
				float coefficecientOfRestitution = 0.6f;

				// Linear and angular velocity of each body, before collision
				glm::vec3 linearVelocity1 = body1Ptr->GetVelocity();
				glm::vec3 angularVelocity1 = body1Ptr->GetAngularVelocity();

				glm::vec3 linearVelocity2 = body2Ptr->GetVelocity();
				glm::vec3 angularVelocity2 = body2Ptr->GetAngularVelocity();

				// Distance from contact point to centre of mass. In this case, the mass is evenly distributed, 
				// so it is simply at the centre of the physics body
				glm::vec3 r1 = body1ContactPoint - (body1Ptr->GetPosition());
				glm::vec3 r2 = body2ContactPoint - (body2Ptr->GetPosition());

				// -(1 + coefficient of restitution), at the beginning of the frictionless impulse equation 
				float restitution = -(1.0f + coefficecientOfRestitution);

				// (V1 - V2), velocity of bodies before collision
				glm::vec3 relativeVelocity = linearVelocity1 - linearVelocity2;

				// (1/m1 + 1/m2), addition of inverse mass of body1 and body2
				float combinedInverseMass = body1Ptr->GetInverseMass() + body2Ptr->GetInverseMass();
				 
				// (r1 x n) and (r2 x n) cross product
				glm::vec3 r1CrossNormal = glm::cross(r1, bodyContactNormal);
				glm::vec3 r2CrossNormal = glm::cross(r2, bodyContactNormal);

				// Values for the frictionless collision impulse equation
				float lambdaNumerator = restitution * (glm::dot(bodyContactNormal, relativeVelocity) + glm::dot(angularVelocity1, r1CrossNormal) - glm::dot(angularVelocity2, r2CrossNormal));
				float lambdaDenominator = combinedInverseMass + (glm::dot(r1CrossNormal, body1Ptr->GetInverseInertiaTensor() * r1CrossNormal) + glm::dot(r2CrossNormal, body2Ptr->GetInverseInertiaTensor() * r2CrossNormal));

				// Lambda, to be multiplied with the unit normal vector at the collision point
				float lambda = lambdaNumerator / lambdaDenominator;

				// Final frictionless collision impulse equation
				glm::vec3 collisionImpulse = lambda * bodyContactNormal;

				// For each body, if it is not kinematic, ie. it is affected by outside forces, update its angular and linear velocity
				if (!body1Ptr->isKinematic)
				{
					linearVelocity1 += collisionImpulse / body1Ptr->GetMass();
					linearVelocity1 = linearVelocity1 * body1Ptr->GetDamping();
					angularVelocity1 += (lambda * body1Ptr->GetInverseInertiaTensor() * r1CrossNormal); // w = w + lambda*inverse intertia tensor * (r1 x n) cross product
					angularVelocity1 = angularVelocity1 * body1Ptr->GetDamping();
					body1Ptr->SetVelocity(linearVelocity1);
					body1Ptr->SetAngularVelocity(angularVelocity1);
				}
				if (!body2Ptr->isKinematic)
				{
					linearVelocity2 -= collisionImpulse / body2Ptr->GetMass();
					linearVelocity2 = linearVelocity2 * body2Ptr->GetDamping();
					angularVelocity2 -= (lambda * body2Ptr->GetInverseInertiaTensor() * r2CrossNormal);
					angularVelocity2 = angularVelocity2 * body2Ptr->GetDamping();
					body2Ptr->SetVelocity(linearVelocity2);
					body2Ptr->SetAngularVelocity(angularVelocity2);
				}
			}
			
			
		}
	}
}

