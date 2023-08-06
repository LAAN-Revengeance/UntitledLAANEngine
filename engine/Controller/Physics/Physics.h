#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <GameObject.h>
#include <Terrain.h>
#include <map>
#include <math.h>
#include <algorithm>
#include <glm/gtx/euler_angles.hpp>

using namespace reactphysics3d;
/**
*	@class Physics
*	@brief manages rigidbody data and collider, 
*	simulates physics and checks collision
*	@version 3.0
*	@author Nathan Choo /  Andres Comeros-Ochtman 
*	@date 19/04/2023
*/

class Physics
{
public:
	Physics();
	~Physics();

	/**
	*	@brief Create physics world 
	*	@return void
	*/
	void CreatePhysicsWorld();
	/**
	*	@brief Destroys physics world
	*	@return void
	*/
	void DestroyPhysicsWorld();

	/**
	*	@brief attaches rigidbody to provided gameobject
	*   @param go containing rigidbody
	* 	@param rbType int corresponding to the desire rigidbody type
	*	@return void
	*/
	void AddRigidBody(GameObject &go, int rbType);
	/**
	*	@brief delete rigidbody from gameobject
	*   @param go containing rigidbody
	*	@return void
	*/
	void DelRigidBody(GameObject &go);

	//modify rigidbodies
	/**
	*	@brief modifies type of the attached rigidbody
	*   @param go containing rigidbody
	*   @param rbType int corresponding to the desire rigidbody type
	*	@return void
	*/
	void ModRigidBodyType(GameObject &go, int rbType);
	/**
	*	@brief enables or disables gravity for rigidbodies
	*   @param go containing rigidbody
	*   @param state determines whether gravity is on(true) or off(false)
	*	@return void
	*/
	void ModRigidBodyGravity(GameObject &go, bool state);	// true == on

	//apply force to rigidbodies
	/**
	*	@brief applies vector force to rigidbody
	*   @param go containing rigidbody
	*   @param force the direction and magnitude of the force
	*	@return void
	*/
	void ApplyRigidBodyForce(GameObject &go, Vector3 force);
	/**
	*	@brief applies a vector force to a provided location on the rigidbody
	*   @param go containing rigidbody
	*   @param force the magnitude and direction of the force
	*   @param point local rigidbody coordinate to apply force
	*	@return void
	*/
	void ApplyRigidBodyForce(GameObject &go, Vector3 force, Vector3 point);
	/**
	*	@brief applies a torque force to the rigidbody
	*   @param go containing rigidbody
	*   @param torque torque to apply
	*	@return void
	*/
	void ApplyRigidBodyTorque(GameObject &go, Vector3 torque);

	//add and del rigidbody colliders
	/**
	*	@brief adds box collider to rigidbody
	*   @param go containing rigidbody
	*   @param scale the scale of the collider
	*   @param offset the offset of the collider
	*   @param mass the mass of the collider 
	*   @param bounce the bounciness of the collider
	*   @param friction the friction of the collider
	*	@return void
	*/
	void AddRigidBodyColliderBox(GameObject &go, glm::vec3 scale, glm::vec3 offset ,float mass, float bounce, float friction);
	/**
	*	@brief adds sphere collider to rigidbody
	*   @param go containing rigidbody
	*   @param radius the radius of the collider
	*   @param offset the offset of the collider 
	*   @param mass the mass of the collider
	*   @param bounce the bounciness of the collider
	*   @param friction the friction of the collider
	*	@return void
	*/
	void AddRigidBodyColliderSphere(GameObject &go, float radius, glm::vec3 offset, float mass, float bounce, float friction);
	/**
	*	@brief adds capsule collider to rigidbody
	*   @param go containing rigidbody
	*   @param radius the radius of the collider
	*   @param height the height of the collider
	*   @param offset the offset of the collider
	*   @param rotation the initial rotation of the collider
	*   @param mass the mass of the collider
	*   @param bounce the bounciness of the collider
	*   @param friction the friction of the collider
	*	@return void
	*/
	void AddRigidBodyColliderCapsule(GameObject& go, float radius, float height, glm::vec3 offset, glm::vec3 rotation, float mass, float bounce, float friction);
	//TODO add Model3D to convex function
	//TODO add Model3D to concave function
	/**
	*	@brief adds heightmap collider to rigidbody
	*   @param terrain a terrain class with the heightmap data
	*	@return void
	*/
	void AddRigidBodyColliderHeightMap(Terrain &terrain);

	//setter and getter
	/**
	*	@brief set the world position of the rigidbody
	*   @param go containing rigidbody
	*   @param newPos the intended position to set
	*	@return void
	*/
	void SetRigidBodyPosition(GameObject &go, Vector3 newPos);
	/**
	*	@brief returns world position of the rigidbody
	*   @param go containing rigidbody
	*	@return Vector3
	*/
	Vector3 GetRigidBodyPosition(GameObject& go);

	//ray casting
	//TODO add Ray casting

	//update GO vector
	/**
	*	@brief updates the position of gameobjects with the position of the rigidbodies
	*   @param goStore the vector containing the gameobjects
	*	@return void
	*/
	void UpdateGameObjects(std::map<std::string, GameObject*>& goStore);

	//simulation settings
	/**
	*	@brief sets how many times per frame to step simulation
	*   @param time the desired frames per second
	*	@return void
	*/
	void SetTimeStep(float time);
	/**
	*	@brief steps the physics simulation
	*   @param deltaTime the time difference since the last timer call
	*	@return void
	*/
	void StepPhysics(float deltaTime);					

	//misc
	inline glm::vec3 react2glm(const Vector3& vec){return { vec.x, vec.y, vec.z };}
	inline Vector3 glm2react(glm::vec3& vec) { return {vec.x, vec.y, vec.z}; }
	/**
	*	@brief toggles whether to display the debug mode
	*	@return void
	*/
	void ToggleDebugDisplay();

	//display debug 
	void DrawDebug(Camera* cam, Shader* shader);

protected:
	//physics factory and world
	PhysicsCommon physicsCommon;	
	PhysicsWorld* world;

	Model* debug_model = nullptr;

	//updates per second
	decimal timeStep = 0.0f;
	float accumulator = 0.0f;

	bool isDebug = false;
};
