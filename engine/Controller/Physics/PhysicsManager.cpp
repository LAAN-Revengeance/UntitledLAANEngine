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

PhysicsBody& PhysicsManager::GetPhysicsBody(unsigned int id)
{
	return physicsBodies.at(id);
}

void PhysicsManager::Update(float deltaTime)
{
	//run rp3d collision detection callback
	rp3dWorld->testCollision(mCallback);

	// Exert gravity on each physics body that has it enabled
	
	for (auto& physicsBody : physicsBodies)
	{
		if (physicsBody.second.useGravity)
		{
			physicsBody.second.SetVelocity(physicsBody.second.GetVelocity() + (deltaTime * physicsBody.second.gravity));
		}
	}
	

	for (auto& physicsBody : physicsBodies)
	{
		physicsBody.second.CalculateInertiaTensor();
		physicsBody.second.SetPosition(physicsBody.second.GetPosition() + physicsBody.second.GetVelocity() * deltaTime);
		physicsBody.second.SetOrientation(glm::normalize(physicsBody.second.GetOrientation() + ((0.5f * glm::quat(0.0, physicsBody.second.GetAngularVelocity()) * physicsBody.second.GetOrientation()) * deltaTime)));
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
	rp3dWorld->setIsDebugRenderingEnabled(true);

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

void rp3dCollisionCallback::onContact(const CallbackData& callbackData)
{		
	for (int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		const auto& contactPair  = callbackData.getContactPair(i);
		
		unsigned int id1 = contactPair.getBody1()->getEntity().id;
		unsigned int id2 = contactPair.getBody2()->getEntity().id;

		//if physics bodies exist, resolve collision.
		auto rbMap = &physicsManager->physicsBodies;
		if (rbMap->find(id1) != rbMap->end() && rbMap->find(id2) != rbMap->end()) {
			
			for (int x = 0; x < contactPair.getNbContactPoints(); x++)
			{
				ContactPoint contactPoint = contactPair.getContactPoint(x);
				float penetration = contactPoint.getPenetrationDepth();
				rp3d::Vector3 contactNormal = contactPair.getContactPoint(x).getWorldNormal();
				PhysicsBody body1Ptr = physicsManager->GetPhysicsBody(id1);
				PhysicsBody body2Ptr = physicsManager->GetPhysicsBody(id2);
				glm::vec3 newVec;
				newVec.x = contactNormal.x;
				newVec.y = contactNormal.y;
				newVec.z = contactNormal.z;

				rp3d::Vector3 body1Contact = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider1();
				rp3d::Vector3 body2Contact = contactPair.getCollider1()->getLocalToWorldTransform() * contactPoint.getLocalPointOnCollider2();
				
				
				if (!body1Ptr.isKinematic)
				{
					physicsManager->GetPhysicsBody(id1).SetPosition(physicsManager->GetPhysicsBody(id1).GetPosition() + ((-(penetration )) * newVec));
				}
				
				if (!body2Ptr.isKinematic)
				{
					physicsManager->GetPhysicsBody(id2).SetPosition(physicsManager->GetPhysicsBody(id2).GetPosition() - ((-(penetration )) * newVec));
				}
				

				glm::vec3 body1ContactPoint;
				body1ContactPoint.x = body1Contact.x;
				body1ContactPoint.y = body1Contact.y;
				body1ContactPoint.z = body1Contact.z;
				glm::vec3 body2ContactPoint;
				body2ContactPoint.x = body2Contact.x;
				body2ContactPoint.y = body2Contact.y;
				body2ContactPoint.z = body2Contact.z;

				float coefficecientOfRestitution = 0.6f;

				glm::vec3 linearVelocity1 = body1Ptr.GetVelocity();
				glm::vec3 angularVelocity1 = body1Ptr.GetAngularVelocity();

				glm::vec3 linearVelocity2 = body2Ptr.GetVelocity();
				glm::vec3 angularVelocity2 = body2Ptr.GetAngularVelocity();

				glm::vec3 r1 = body1ContactPoint - (body1Ptr.GetPosition());
				glm::vec3 r2 = body2ContactPoint - (body2Ptr.GetPosition());

				float restitution = -(1.0f + coefficecientOfRestitution);

				glm::vec3 relativeVelocity = linearVelocity1 - linearVelocity2;

				float combinedInverseMass = body1Ptr.GetInverseMass() + body2Ptr.GetInverseMass();

				glm::vec3 r1CrossNormal = glm::cross(r1, newVec);
				glm::vec3 r2CrossNormal = glm::cross(r2, newVec);

				float lambdaNumerator = restitution * (glm::dot(newVec, relativeVelocity) + glm::dot(angularVelocity1, r1CrossNormal) - glm::dot(angularVelocity2, r2CrossNormal));
				float lambdaDenominator = combinedInverseMass + (glm::dot(r1CrossNormal, body1Ptr.GetInverseInertiaTensor() * r1CrossNormal) + glm::dot(r2CrossNormal, body2Ptr.GetInverseInertiaTensor() * r2CrossNormal));

				float lambda = lambdaNumerator / lambdaDenominator;

				glm::vec3 collisionImpulse = lambda * newVec;
				
				if (!body1Ptr.isKinematic)
				{
					linearVelocity1 += collisionImpulse / body1Ptr.GetMass();
					angularVelocity1 += (lambda * body1Ptr.GetInverseInertiaTensor() * r1CrossNormal);
					body1Ptr.SetVelocity(linearVelocity1);
					body1Ptr.SetAngularVelocity(angularVelocity1);
				}
				if (!body2Ptr.isKinematic)
				{
					linearVelocity2 -= collisionImpulse / body2Ptr.GetMass();
					angularVelocity2 -= (lambda * body2Ptr.GetInverseInertiaTensor() * r2CrossNormal);
					body2Ptr.SetVelocity(linearVelocity2);
					body2Ptr.SetAngularVelocity(angularVelocity2);
				}
			}
			
			
		}
	}
}

