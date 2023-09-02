#include "PhysicsManager.h"

using namespace rp3d;


PhysicsManager& PhysicsManager::Get()
{
	static PhysicsManager p_instance;
	return p_instance;
}

PhysicsManager::PhysicsManager()
{
	//rigidBodies.reserve(100

	//rp3d physics world
	rp3dWorld = rp3dPhysicsCommon.createPhysicsWorld();

	//test
	//Vector3 position(0.0, 0.0, 0.0);
	//Quaternion orientation = Quaternion::identity();
	//Transform transform(position, orientation);
	//// Create a collision body in the world
	//CollisionBody* body;
	//body = rp3dWorld->createCollisionBody(transform);
	//
	//float radius = 5.0f;
	//SphereShape* sphereShape = rp3dPhysicsCommon.createSphereShape(radius);
	//Collider* collider;
	//collider = body->addCollider(sphereShape, transform);
	//
	//Vector3 position2(0.0, 4.9, 0.0);
	//Quaternion orientation2 = Quaternion::identity();
	//Transform transform2(position2, orientation2);
	//// Create a collision body in the world
	//CollisionBody* body2;
	//body2 = rp3dWorld->createCollisionBody(transform2);
	//
	//SphereShape* sphereShape2 = rp3dPhysicsCommon.createSphereShape(radius);
	//Collider* collider2;
	//collider2 = body2->addCollider(sphereShape2, transform2);
}

PhysicsManager::~PhysicsManager()
{
	rp3dPhysicsCommon.destroyPhysicsWorld(rp3dWorld);
}

void PhysicsManager::ResolveCollision(PhysicsBody& b1, PhysicsBody& b2)
{

}

PhysicsBody& PhysicsManager::GetPhysicsBody(unsigned int id)
{
	return physicsBodies.at(id);
}

void PhysicsManager::Update(double deltaTime)
{
	//run rp3d collision detection callback
	rp3dWorld->testCollision(mCallback);
}

PhysicsBody& PhysicsManager::AddPhysicsBody(GameObject& go)
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

	//assign rigidbody to gameobject
	//maybe it should be the other way around? PhysicsBody has game object refernce?
	go.physicsBody = &physicsBodies.at(id);

	return physicsBodies.at(id);
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


void rp3dCollisionCallback::onContact(const CallbackData& callbackData)
{
	PhysicsManager& pManager = PhysicsManager::Get();

	for (int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		unsigned int id1 = callbackData.getContactPair(i).getBody1()->getEntity().id;
		unsigned int id2 = callbackData.getContactPair(i).getBody2()->getEntity().id;

		std::cout << "body: " << id1 << " | " << "body: " << id2 << "\n";
		//if physics bodies exist, resolve collision.
		auto rbMap = &pManager.physicsBodies;
		if (rbMap->find(id1) != rbMap->end() && rbMap->find(id2) != rbMap->end()) {
			pManager.ResolveCollision(pManager.GetPhysicsBody(id1), pManager.GetPhysicsBody(id2));
		}
	}
}