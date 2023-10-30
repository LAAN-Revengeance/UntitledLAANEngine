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

void PhysicsManager::Update(double deltaTime)
{
	accumilator += deltaTime;
	static double timeStep = 30.0/ 1000.0;//30 times per second update

	for (auto& physicsBody : physicsBodies)
	{
		PhysicsIntegrator::Integrate(physicsBody.second, deltaTime);
	}

	if (true)//accumilator >= timeStep
	{
		accumilator = 0.0;
		//run rp3d collision detection callback
		rp3dWorld->testCollision(mCallback);
	}
}

PhysicsBody* PhysicsManager::CreatePhysicsBody(GameObject* go)
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
	physicsBodies.insert({ id,pb });
	gameObjects.insert({id,go});
	BoxShape* shape = rp3dPhysicsCommon.createBoxShape(Vector3(0.03, 0.3, 0.2));

	go->physicsBody = &physicsBodies.at(id);
	return &physicsBodies.at(id);
}


SphereCollider* PhysicsManager::AddSphereCollider(PhysicsBody& pb, float radius)
{
	SphereShape* shape = rp3dPhysicsCommon.createSphereShape(radius);
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	SphereCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
	pb.CalcCenterOfMass();
	return static_cast<SphereCollider*>(&pb.colliders[pb.colliders.size() - 1]);
}

BoxCollider* PhysicsManager::AddBoxCollider(PhysicsBody& pb, glm::vec3 scale)
{
	BoxShape* shape = rp3dPhysicsCommon.createBoxShape(Vector3(scale.x, scale.y, scale.z));
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	BoxCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
	pb.CalcCenterOfMass();
	return static_cast<BoxCollider*>(&pb.colliders[pb.colliders.size() - 1]);
}

CapsuleCollider* PhysicsManager::AddCapsuleCollider(PhysicsBody& pb, float radius, float height)
{
	CapsuleShape* shape = rp3dPhysicsCommon.createCapsuleShape(radius, height);
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	CapsuleCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
	pb.CalcCenterOfMass();
	return static_cast<CapsuleCollider*>(&pb.colliders[pb.colliders.size() - 1]);
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
		glLineWidth(1);
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
	if (physicsBody) {
		rp3dWorld->destroyCollisionBody(physicsBody->body);
		
		physicsBodies.erase(physicsBody->GetID());
		gameObjects.erase(physicsBody->GetID());
	}
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

GameObject* PhysicsManager::Raycast(glm::vec3 origin, glm::vec3 direction, float distance)
{
	glm::vec3 end = origin + (direction * distance);

	rp3d::Vector3 startPos = { origin.x,origin.y,origin.z };
	rp3d::Vector3 endPos = { end.x,end.y,end.z };

	rp3d::Ray ray(startPos, endPos);

	
	rp3dRaycastCallback callback;
	rp3dWorld->raycast(ray,&callback);

	unsigned int hitID = callback._hitID;

	if (physicsBodies.find(hitID) != physicsBodies.end()) {
		DebugLogger::Log(GAEM_DEBUG, "hit: " + gameObjects.at(hitID)->name, "Raycast");
		return gameObjects.at(hitID);
	}
	return nullptr;
}


void rp3dCollisionCallback::onContact(const CallbackData& callbackData)
{
	unsigned int numPairs = callbackData.getNbContactPairs();

	const unsigned int MAX_CONTACTS = 8;
	//static CollisionData contactBuffer[MAX_CONTACTS];
	static CollisionData collisionBuffer;

	for (int i = 0; i < numPairs; i++)
	{
		const auto& contactPair = callbackData.getContactPair(i);

		unsigned int id1 = contactPair.getBody1()->getEntity().id;
		unsigned int id2 = contactPair.getBody2()->getEntity().id;

		unsigned int numContacts = contactPair.getNbContactPoints();

		if (numContacts > MAX_CONTACTS) numContacts = MAX_CONTACTS;
		
		collisionBuffer.numContacts = numContacts;

		for (int j = 0; j < numContacts; j++)
		{
			rp3d::Vector3 contactPoint1 = contactPair.getContactPoint(j).getLocalPointOnCollider1();
			rp3d::Vector3 contactPoint2 = contactPair.getContactPoint(j).getLocalPointOnCollider2();

			float penDepth = contactPair.getContactPoint(j).getPenetrationDepth();
			rp3d::Vector3 contactNormal = -contactPair.getContactPoint(j).getWorldNormal();

			//if physics bodies exist, resolve collision.
			auto rbMap = &physicsManager->physicsBodies;
			if (rbMap->find(id1) != rbMap->end() && rbMap->find(id2) != rbMap->end()) {

				ContactPointData contact;
				collisionBuffer.b1 = &physicsManager->GetPhysicsBody(id1);
				collisionBuffer.b2 = &physicsManager->GetPhysicsBody(id2);
				contact.contactNormal = { contactNormal.x,contactNormal.y,contactNormal.z };
				contact.penetrationDepth = penDepth;
				contact.localPositionB1 = { contactPoint1.x, contactPoint1.y, contactPoint1.z };
				contact.localPositionB2 = { contactPoint2.x, contactPoint2.y, contactPoint2.z };
				
				//calc pos in worldspace.
				glm::vec4 localPoint = glm::vec4(contactPoint1.x, contactPoint1.y, contactPoint1.z, 1.0f);

				glm::vec4 worldPoint = collisionBuffer.b1->GetTransformMatrix() * localPoint;
				contact.contactWorldSpace = glm::vec3(worldPoint);

				collisionBuffer.contacts[j] = contact;
			}
		}

		if(numContacts > 0)
			CollisionSolver::ResolveCollisions(collisionBuffer);

	}
}