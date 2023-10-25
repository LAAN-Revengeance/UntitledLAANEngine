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
	elapsedTime += deltaTime;
	dt = deltaTime;

	if (elapsedTime >= timeStep)
	{
		//rigidbody for loop
		for (int i = 0; i < physicsBodies.size(); i++)
		{
			//if not kinematic
			if (!physicsBodies[i].isKinematic)
			{
				IntegrateAcceleration(i);
				IntegrateVelocity(i);

				//clear forces
				physicsBodies[i].ClearAccumulator();
			}
		}

		//run rp3d collision detection callback
		rp3dWorld->testCollision(mCallback);

		//reset elapsedtime
		elapsedTime = 0;
	}
}

void PhysicsManager::IntegrateAcceleration(int i)
{
	//===================================================================================================LinearStart
	//calc acceleration    --    a = f*m^-1
	glm::vec3 acceleration = physicsBodies[i].GetForce() * physicsBodies[i].GetInverseMass();

	//apply gravity if enabled and mass greater than 0
	if (physicsBodies[i].useGravity && physicsBodies[i].GetMass() > 0)
		acceleration.y += gravity;

	//apply acceleration to velocity
	glm::vec3 newVelocity = physicsBodies[i].GetLinearVelocity() + acceleration * dt;

	//set new velocity
	physicsBodies[i].SetLinearVelocity(newVelocity.x, newVelocity.y, newVelocity.z);
	//===================================================================================================LinearEnd


	//===================================================================================================AngularStart
	//update inertia tensor
	physicsBodies[i].UpdateTensor();

	//calc angular acceleration
	glm::vec3 angularAcceleration = physicsBodies[i].GetInverseTensor() * physicsBodies[i].GetTorque();

	//apply accelertaion to velocity
	glm::vec3 newAngularVelocity = physicsBodies[i].GetAngularVelocity() + angularAcceleration * dt;

	//set new angular velocity
	physicsBodies[i].SetAngularVelocity(newAngularVelocity.x, newAngularVelocity.y, newAngularVelocity.z);
	//===================================================================================================AngularEnd
}

void PhysicsManager::IntegrateVelocity(int i)
{
	//update position from velocity
	glm::vec3 newPos = physicsBodies[i].GetPosition() + physicsBodies[i].GetLinearVelocity() * dt;
	physicsBodies[i].SetPosition(newPos.x, newPos.y, newPos.z);

	//update rotation from velocity
	glm::vec3 angVel = (physicsBodies[i].GetAngularVelocity());
	glm::quat orient = physicsBodies[i].GetRotation();	
	orient = orient + (glm::quat(angVel*dt*0.5f) * orient);
	orient = glm::normalize(orient);
	physicsBodies[i].SetRotation(orient);

	//apply damping
	float damping = powf(physicsBodies[i].GetDamping(),dt);
	glm::vec3 linearVelocity = physicsBodies[i].GetLinearVelocity() * damping;
	glm::vec3 angularVelocity = physicsBodies[i].GetAngularVelocity() * damping;

	//set velocities
	physicsBodies[i].SetLinearVelocity(linearVelocity.x, linearVelocity.y, linearVelocity.z);
	physicsBodies[i].SetAngularVelocity(angularVelocity.x, angularVelocity.y, angularVelocity.z);
}

void PhysicsManager::ResolveCollision(int id1, int id2, float penDepth, glm::vec3 contactNormal, glm::vec3 contact1, glm::vec3 contact2)
{
	PhysicsBody rb1 = physicsBodies[id1];
	PhysicsBody rb2 = physicsBodies[id2];

	float totalMass = rb1.GetInverseMass() + rb2.GetInverseMass();

	//seperation
	glm::vec3 transform1 = rb1.GetPosition() - (contactNormal * penDepth * (rb1.GetInverseMass() / totalMass));
	glm::vec3 transform2 = rb2.GetPosition() + (contactNormal * penDepth * (rb2.GetInverseMass() / totalMass));

	physicsBodies[id1].SetPosition(transform1.x, transform1.y, transform1.z);
	physicsBodies[id2].SetPosition(transform2.x, transform2.y, transform2.z);

	/*
			//linear impulse 
	//relative velocity of the two objects		-- vr = v1 - v2
	glm::vec3 relativeVel1 = rb1.GetLinearVelocity() - rb2.GetLinearVelocity();
	glm::vec3 relativeVel2 = rb2.GetLinearVelocity() - rb1.GetLinearVelocity();
	
	//calc impulse velocity						-- vj = -(1+e)vr dot normal
	float impulseVel1 = ( - (1.0f + rb1.GetCoRestitution())) * glm::dot(relativeVel1, contactNormal);
	float impulseVel2 = ( - (1.0f + rb2.GetCoRestitution())) * glm::dot(relativeVel2, contactNormal);

	//calc final impulse force					-- J = (vj)	/	(m1^-1 + m2^-1)
	float impulse1 = impulseVel1 / totalMass;
	float impulse2 = impulseVel2 / totalMass;

	//apply final impulse force to velocity		-- vf = vi + m^-1*J*normal
	glm::vec3 newVel1 = rb1.GetLinearVelocity() + (rb1.GetInverseMass() * impulse1 * contactNormal);
	glm::vec3 newVel2 = rb2.GetLinearVelocity() + (rb2.GetInverseMass() * impulse2 * contactNormal);

	physicsBodies[id1].SetLinearVelocity(newVel1.x, newVel1.y, newVel1.z);
	physicsBodies[id2].SetLinearVelocity(newVel2.x, newVel2.y, newVel2.z);
	*/

	//calc kinetic energy



		//angular impulse
	//calc relative position					-- r = contact.pos - transform.pos
	glm::vec4 worldContact1 = rb1.GetTransformMatrix() * glm::vec4(contact1, 1.0f);		//possible issue source
	glm::vec4 worldContact2 = rb2.GetTransformMatrix() * glm::vec4(contact2, 1.0f);		//possible issue source
	
	glm::vec3 relativePos1 = glm::vec3(worldContact1) - rb1.GetPosition();				//possible issue source
	glm::vec3 relativePos2 = glm::vec3(worldContact2) - rb2.GetPosition();				//possible issue source
   
	//calc angular velocity
	glm::vec3 angleVel1 = glm::cross(rb1.GetAngularVelocity(), relativePos1);
	glm::vec3 angleVel2 = glm::cross(rb2.GetAngularVelocity(), relativePos2);

	//calc full velocity
	glm::vec3 fullVel1 = rb1.GetLinearVelocity() + angleVel1;
	glm::vec3 fullVel2 = rb2.GetLinearVelocity() + angleVel2;
	
	//											--  J = (-(1+e)Vr X N) / (totalMass + (inertia1 + inertia2) . N)
	//calc inertia								--  inertia = I^-1 * (relative cross normal) cross relative
	glm::vec3 inertia1	= glm::cross(rb1.GetInverseTensor() * glm::cross(relativePos1, contactNormal), relativePos1);
	glm::vec3 inertia2	= glm::cross(rb2.GetInverseTensor() * glm::cross(relativePos2, contactNormal), relativePos2);
	float	VrN = glm::dot(fullVel2 - fullVel1, contactNormal);
	glm::vec3 totalI	= inertia1 + inertia2;
		//std::cout <<	glm::dot(totalI, contactNormal) << "::" << 
		//				glm::dot(totalI, contactNormal) << "::" << 
		//				glm::dot(totalI, contactNormal) << "\n\n";
	//calc J
	float j1 = ((-(1.0f + rb1.GetCoRestitution())) * VrN) / (totalMass + glm::dot(totalI, contactNormal));
	float j2 = ((-(1.0f + rb2.GetCoRestitution())) * VrN) / (totalMass + glm::dot(totalI, contactNormal));

	//apply J to the contact normal
	glm::vec3 fullImpulse1 = contactNormal * j1;
	glm::vec3 fullImpulse2 = contactNormal * j2;


	//apply linear impulse
	physicsBodies[id1].ApplyLinearImpulse(-fullImpulse1);
	physicsBodies[id2].ApplyLinearImpulse(fullImpulse2);

	//apply angular impulse
	physicsBodies[id1].ApplyAngularImpulse(glm::cross(relativePos1, -fullImpulse1));
	physicsBodies[id2].ApplyAngularImpulse(glm::cross(relativePos2, fullImpulse2));
	
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
	pb.UpdateTensor();
}

void PhysicsManager::AddBoxCollider(PhysicsBody& pb, glm::vec3 scale)
{
	BoxShape* shape = rp3dPhysicsCommon.createBoxShape(Vector3(scale.x, scale.y, scale.z));
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	BoxCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
	pb.UpdateTensor();
}

void PhysicsManager::AddCapsuleCollider(PhysicsBody& pb, float radius, float height)
{
	CapsuleShape* shape = rp3dPhysicsCommon.createCapsuleShape(radius, height);
	rp3d::Collider* rpCollider = pb.body->addCollider(shape, Transform::identity());

	CapsuleCollider collider;
	collider.rp3dCollider = rpCollider;
	pb.colliders.push_back(collider);
	//pb.UpdateTensor();
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
	//loop through all current contact pairs
	for (int i = 0; i < callbackData.getNbContactPairs(); i++)
	{
		//get contact pair
		const auto& contactPair  = callbackData.getContactPair(i);
		
		//get id of colliding bodies
		unsigned int id1 = contactPair.getBody1()->getEntity().id;
		unsigned int id2 = contactPair.getBody2()->getEntity().id;

		rp3d::Vector3 contactPoint1;
		rp3d::Vector3 contactPoint2;
		rp3d::Vector3 contactNormal;

		auto rbMap = &physicsManager->physicsBodies;

		//get average of contact points and contact normal
		float NbContact = contactPair.getNbContactPoints();
		for (int x = 0; x < NbContact; x++)
		{
			contactPoint1 += contactPair.getContactPoint(x).getLocalPointOnCollider1();
			contactPoint2 += contactPair.getContactPoint(x).getLocalPointOnCollider2();

			contactNormal += contactPair.getContactPoint(x).getWorldNormal();
		}

		contactPoint1 = contactPoint1 / NbContact;
		contactPoint2 = contactPoint1 / NbContact;

		contactNormal = contactNormal / NbContact;
		//contactNormal = contactPair.getContactPoint(0).getWorldNormal();

		//get penetration depth
		float penDepth = contactPair.getContactPoint(0).getPenetrationDepth();

		//if physics bodies exist, resolve collision.
		//auto rbMap = &physicsManager->physicsBodies;
		if (rbMap->find(id1) != rbMap->end() && rbMap->find(id2) != rbMap->end()) 
		{
			//resolve collision
			glm::vec3 contact1 = glm::vec3(contactPoint1.x, contactPoint1.y, contactPoint1.z);
			glm::vec3 contact2 = glm::vec3(contactPoint2.x, contactPoint2.y, contactPoint2.z);
			glm::vec3 normal = glm::vec3(contactNormal.x, contactNormal.y, contactNormal.z);
			physicsManager->ResolveCollision(id1, id2, penDepth, normal, contact1, contact2);
		}
	}
}