#include "Physics.h"

Physics::Physics() {
	CreatePhysicsWorld();
	SetTimeStep(60);
}

Physics::~Physics() {
	DestroyPhysicsWorld();
}


void Physics::CreatePhysicsWorld()
{
	world = physicsCommon.createPhysicsWorld();
}

void Physics::DestroyPhysicsWorld()
{
	physicsCommon.destroyPhysicsWorld(world);
}

void Physics::AddRigidBody(GameObject &go, int rbType)
{
	Vector3 position(go.position.x, go.position.y, go.position.z);
	Quaternion orientation = Quaternion::fromEulerAngles((float)go.rotation.x, (float)go.rotation.y, (float)go.rotation.z);
	Transform transform(position, orientation);

	go.rigidBody.rbPtr = world->createRigidBody(transform);
	go.rigidBody.worldPtr = world;

	go.rigidBody.collideListen.ID = go.rigidBody.rbPtr->getEntity().id;

	ModRigidBodyType(go,rbType);
}

void Physics::DelRigidBody(GameObject &go)
{
	//delete rb from simulation
	world->destroyRigidBody(go.rigidBody.rbPtr);
}

void Physics::AddRigidBodyColliderBox(GameObject& go, glm::vec3 scale, glm::vec3 offset, float mass, float bounce, float friction)
{
	RigidBody* rb = go.rigidBody.rbPtr;

	//refactor
	BoxCollider* box = new BoxCollider(mass, bounce, friction, offset, { 0,0,0 }, scale);
	delete go.rigidBody.collider;
	go.rigidBody.collider = box;
	

	BoxShape* shape = physicsCommon.createBoxShape({scale.x,scale.y,scale.z});

	rp3d::Quaternion q = Quaternion::identity();
	Transform transform({ offset.x,offset.y, offset.z }, q);
	rb->addCollider(shape, transform);

	rp3d::Material& mat= rb->getCollider(rb->getNbColliders() - 1)->getMaterial();
	mat.setMassDensity(mass);
	mat.setBounciness(bounce);
	mat.setFrictionCoefficient(friction);
}

void Physics::AddRigidBodyColliderSphere(GameObject &go, float radius, glm::vec3 offset, float mass, float bounce, float friction)
{
	RigidBody* rb = go.rigidBody.rbPtr;

	//refactor
	SphereCollider* sphere = new SphereCollider(mass, bounce, friction, offset, { 0,0,0 }, radius);
	delete go.rigidBody.collider;
	go.rigidBody.collider = sphere;

	SphereShape* shape = physicsCommon.createSphereShape(radius);
	rp3d::Quaternion q = Quaternion::identity();

	Transform transform({ offset.x,offset.y, offset.z }, q);

	rb->addCollider(shape, transform);

	rp3d::Material& mat = rb->getCollider(rb->getNbColliders() - 1)->getMaterial();
	mat.setMassDensity(mass);
	mat.setBounciness(bounce);
	mat.setFrictionCoefficient(friction);
}

void Physics::AddRigidBodyColliderCapsule(GameObject &go, float radius ,float height, glm::vec3 offset, glm::vec3 rotation,float mass, float bounce, float friction)
{
	RigidBody* rb = go.rigidBody.rbPtr;

	//refactor
	CapsuleCollider* capsule = new CapsuleCollider(mass, bounce, friction, offset, rotation, radius, height);
	delete go.rigidBody.collider;
	go.rigidBody.collider = capsule;

	CapsuleShape* shape = physicsCommon.createCapsuleShape(radius, height);

	rotation = glm::radians(rotation);
	rp3d::Quaternion q = Quaternion::fromEulerAngles({ rotation.x,rotation.y,rotation.z });

	Transform transform({offset.x,offset.y, offset.z},q);

	rb->addCollider(shape, transform);

	rp3d::Material& mat = rb->getCollider(rb->getNbColliders() - 1)->getMaterial();
	mat.setMassDensity(mass);
	mat.setBounciness(bounce);
	mat.setFrictionCoefficient(friction);
}

void Physics::AddRigidBodyColliderHeightMap(Terrain& terrain)
{
	int rows = terrain.GetSize();
	int cols = terrain.GetSize();
	float min = terrain.GetMinHeight();
	float max = terrain.GetMaxHeight();
	float* hv = &terrain.GetHeightArray()->at(0);

	//refactor
	TerrainCollider* collider = new TerrainCollider(1, 1, 1, { 0,0,0 }, {0,0,0}, rows,cols,min,max, hv);
	delete terrain.rigidBody.collider;
	terrain.rigidBody.collider = collider;

	HeightFieldShape* shape = physicsCommon.createHeightFieldShape(rows, cols, min, max+2, hv, HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);
	shape->setScale({terrain.scaleX,1,terrain.scaleZ});
	Transform transform = Transform::identity();
	terrain.rigidBody.rbPtr->addCollider(shape, transform);
}

void Physics::ModRigidBodyType(GameObject &go, int type)
{
	go.rigidBody.ModType(type);
}

void Physics::ModRigidBodyGravity(GameObject &go, bool state)
{
	go.rigidBody.rbPtr->enableGravity(state);
}

void Physics::ApplyRigidBodyForce(GameObject &go, Vector3 force)
{
	go.rigidBody.rbPtr->applyLocalForceAtCenterOfMass(force);
}

void Physics::ApplyRigidBodyForce(GameObject &go, Vector3 force, Vector3 point)
{
	go.rigidBody.rbPtr->applyLocalForceAtLocalPosition(force, point);
}

void Physics::ApplyRigidBodyTorque(GameObject &go, Vector3 torque)
{
	go.rigidBody.rbPtr->applyLocalTorque(torque);
}

void Physics::SetRigidBodyPosition(GameObject &go, Vector3 newPos)
{
	Quaternion quat = Quaternion::identity();
	Transform transform(newPos, quat);
	go.rigidBody.rbPtr->setTransform(transform);
}

Vector3 Physics::GetRigidBodyPosition(GameObject& go)
{
	Transform transform = go.rigidBody.rbPtr->getTransform();
	Vector3 vec = transform.getPosition();
	return vec;
}

void Physics::UpdateGameObjects(std::map<std::string, GameObject*>& goStore)
{
	for (auto& it : goStore) {
		
		if (it.second->rigidBody.rbPtr) {
			Transform transform = it.second->rigidBody.rbPtr->getTransform();
			Vector3 position = transform.getPosition();
			Quaternion rot = transform.getOrientation();
			if (rot.length() > std::numeric_limits<double>::epsilon()) {
				rot.normalize();

				float w = rot.w;
				float x = rot.x;
				float y = rot.y;
				float z = rot.z;

				glm::vec3 rotvec;
				rotvec.x = atan2(2.0f * (w * x + y * z), 1 - 2 * (x * x + y * y)) * 57.2957795131f;
				rotvec.y = asin(std::clamp(2.0f * (w * y - z * x),-1.0f,1.0f)) * 57.2957795131f;
				rotvec.z = atan2(2.0f * (w * z + x * y), 1 - 2 * (y * y + z * z)) * 57.2957795131f;

				it.second->rotation = rotvec;
			}
			it.second->position = glm::vec3(position.x, position.y, position.z);
		}
	}
}

void Physics::SetTimeStep(float time)
{
	timeStep = 1.0f / time; 
	//30: 0.0333333333
	//60: 0.0166666666
	//144: 0.006944444
}

void Physics::StepPhysics(float deltaTime)
{
	accumulator += deltaTime;

	// While there is enough accumulated time to take one or several physics steps 
	if (accumulator >= timeStep)
	{
		// Update the Dynamics world with a constant time step 
		world->update(timeStep);
		// Decrease the accumulated time 
		//accumulator -= deltaTime;
		accumulator = timeStep;
	}
}

void Physics::ToggleDebugDisplay()
{
	isDebug = !isDebug;
	world->setIsDebugRenderingEnabled(isDebug);
}

void Physics::DrawDebug(Camera* cam, Shader* shader)
{
	if (isDebug) {
		// Enable debug rendering 
		world->setIsDebugRenderingEnabled(true);
		// Get a reference to the debug renderer 
		DebugRenderer& debugRenderer = world->getDebugRenderer();
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

			if (debug_model)
				debug_model->FreeData();

			debug_model = new Model();
			shader->SetUniform("model", modelMat);

			debug_model->SetDebugVertexData((float*)&tri->point1.x, nTri * 3);
			debug_model->Render(cam, shader, false, GL_TRIANGLES);
			
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
	
	}
	
}