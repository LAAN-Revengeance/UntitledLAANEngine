#pragma once
#include <Graphics/Graphics.h>
#include <Mesh.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Physics/RigidBody.h>
#include <Utils/DebugLogger.h>
#include <Lua/LuaFunction.h>
#include <AI/Affordance/AffordanceController.h>	

/**
*	@Class GameObject
*	@brief Stores information on an object in the game world. Provides
*	information on how to render the object and how it interacts with the
*	physics engine.
*
*	@author Andres Comeros-Ochtman, Dylan Burgess, Nathan Choo
*	@version 2.0
*	@date 15/04/2023
*/
class GameObject
{
public:

		/**
		*	@brief Default constructor
		*/
	GameObject();

		/**
		*	@brief Destructor
		*/
	~GameObject();

		/*
		*   @brief Checks if this models bouding sphere is in a frustum
		*   @param frustum frustum to check if inside
		*   @return if object is in frustum
		*/
	bool CheckInFrustum(Frustum& frustum);

		/**
		*	@brief Set the current position in world space, also updates
		*	the rigidbody position data.
		*	@param nPos new position in metres
		*	@return void
		*/
	void SetPosition(glm::vec3 nPos);

		/**
		*	@brief returns current postion of this game object
		*	@return the current position in worldspace
		*/
	glm::vec3 GetPosition();

		/**
		*	@brief Set the current rotation using euler angles
		*	@param x rotation along x axis
		*	@param y rotation along y axis
		*	@param z rotation along z axis
		*	@return void
		*	@warning this is a very unstable function and may lead to
		*	undesirable behaviour along the Y axis -+90 degrees due to gimbal lock
		*/
	void SetRotationEuler(float x, float y, float z);

		/**
		*	@brief Set the current rotation in world space, also updates
		*	the rigidbody rotation data.
		*	@param nRot new rotation in degrees
		*	@return void
		*/
	void SetRotation(glm::quat nRot);
		/**
		*	@brief rotate around a specified axix
		*	@param x axis X value
		*	@param y axis Y value
		*	@param z axis Z value
		*	@param angle angle in radians to rotate around the axis
		*/
	void Rotate(float x, float y, float z, float angle);

		/**
		*	@brief Set the current scale in world space, also updates
		*	the rigidbody scale data.
		*	@param nSacle new scale ratio
		*	@return void
		*/
	void SetScale(glm::vec3 nScale);

		/**
		*	@brief Sets any special uniform information for subclasses of GameObject
		*	@return void
		*/
	virtual void SetUniforms();

		/**
		*	@brief runs every update loop. used to give special behaviors to subclasses
		*	@return void
		*/
	virtual void Update(double dt);
	
		/**
		*	@brief rotates this game object to face a direction
		*	@param lookPos positon to face towawrds
		*	@return this objects right vector
		*/
	void LookAt(glm::vec3 lookPos);
	glm::vec3 GetRotationEuler();
	glm::mat4 GetTransformMatrix();

		///Position in worldspace
	glm::vec3 position = {0,0,0};
		///Scale along each axis
	glm::vec3 scale =	{1,1,1};
		///Rotation in degrees
	glm::quat orientation = glm::quat(1,0,0,0);

		///3D model data
	DrawItem *model_data = nullptr;
	bool isCastShadow = true;

		///Custom shader
	Shader* shader = nullptr;
	
		///physics body
	PhysicsBody* physicsBody = nullptr;

		///Name of this object
	std::string name;
		///Return this objects ID
	int GetID() { return ID; }
		///Set this objects ID  
	void SetID(int id) { ID = id; };

		//Retuns a reference to model data
	DrawItem* GetDrawItem();
		
		//Material properties
	Material material;

	AffordanceController affordanceController;

	void SetUpdateFunction(LuaFunction<void, GameObject&> function);
	LuaFunction<void, GameObject&> GetUpdateFunction();
	
protected:
		///Unique identifier
	unsigned int ID = 0;

	LuaFunction<void, GameObject&> updateFunction;

	friend class PhysicsManager;
};
