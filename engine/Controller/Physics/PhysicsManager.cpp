#include "PhysicsManager.h"

using namespace rp3d;


enum colliderShape {
	sphere = 1,
	cube = 2,
	capsule = 3
};


PhysicsManager& PhysicsManager::Get()
{
	static PhysicsManager p_instance;
	return p_instance;
}

PhysicsManager::PhysicsManager()
{
	//rigidBodies.reserve(100);
	
	//rp3d physics world
	rp3dWorld = rp3dPhysicsCommon.createPhysicsWorld();
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

void PhysicsManager::DrawPhysicsWorld(Camera& camera)
{
	if(!debugShader)
		debugShader = new Shader("resources/shaders/Physics_Debug/Physics.vert", "resources/shaders/Physics_Debug/Physics.frag", "");

	// Enable debug rendering 
	rp3dWorld->setIsDebugRenderingEnabled(true);
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

		//set model matrix uniforms
		glm::mat4 modelMat(1.0f);

		const reactphysics3d::DebugRenderer::DebugTriangle* tri = debugRenderer.getTrianglesArray();

		if (debugMesh)
			debugMesh->FreeData();

		debugMesh = new Mesh();
		debugShader->SetUniform("model", modelMat);

		debugMesh->SetDebugVertexData((float*)&tri->point1.x, nTri * 3);
		debugMesh->Render(&camera, debugShader, false, GL_TRIANGLES);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
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
	pb.SetID(id);
	physicsBodies.insert({id,pb});

	//assign rigidbody to gameobject
	//maybe it should be the other way around? PhysicsBody has game object refernce?
	go.rigidBody = &pb;

	return physicsBodies.at(id);
}

void PhysicsManager::AddColliderShape(PhysicsBody& pb, int shape)
{
	//todo custom variables for shape scale

	//create shape from physics common
	switch (shape)
	{
	case 1:		//sphere
		SphereShape* sphere = rp3dPhysicsCommon.createSphereShape(1);
		pb.body->addCollider(sphere, Transform::identity());
		break;
	case 2:		//cube
		BoxShape * cube = rp3dPhysicsCommon.createBoxShape(Vector3(1,1,1));
		pb.body->addCollider(cube, Transform::identity());
		break;
	case 3:		//capsule
		CapsuleShape * capsule = rp3dPhysicsCommon.createCapsuleShape(1, 1);
		pb.body->addCollider(capsule, Transform::identity());
		break;
	}
}

void rp3dCollisionCallback::onContact(const CallbackData& callbackData)
{
	PhysicsManager& pManager = PhysicsManager::Get();

	for (int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		unsigned int id1 = callbackData.getContactPair(i).getBody1()->getEntity().id;
		unsigned int id2 = callbackData.getContactPair(i).getBody1()->getEntity().id;
		pManager.ResolveCollision(pManager.GetPhysicsBody(id1), pManager.GetPhysicsBody(id1));
	}
}
