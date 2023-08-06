#pragma once
#include <Graphics/Graphics.h>
#include <Model.h>
#include <Physics/Rigidbody.h>
#include <AI/StateMachine.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>

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
		*	@brief Set the current rotation in world space, also updates
		*	the rigidbody rotation data.
		*	@param nRot new rotation in degrees
		*	@return void
		*/
	void SetRotation(glm::vec3 nRot);
	
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
		*	@brief calculates this objects up vector.
		*	@return this objects up vector
		*/
	glm::vec3 GetUpVec();

		/**
		*	@brief calculates this objects forward vector.
		*	@return this objects forward vector
		*/
	glm::vec3 GetForwardVec();

		/**
		*	@brief calculates this objects right vector.
		*	@return this objects right vector
		*/
	glm::vec3 GetRightVec();
	
		/**
		*	@brief rotates this game object to face a direction
		*	@param lookvec positon to face towawrds
		*	@return this objects right vector
		*/
	void LookAt(glm::vec3 lookvec);

		///Position in worldspace
	glm::vec3 position = {0,0,0};
		///Scale along each axis
	glm::vec3 scale =	{1,1,1};
		///Rotation in degrees
	glm::vec3 rotation = {0,0,0};

		///3D model data
	DrawItem *model_data = nullptr;
		///Rigidbody physics information for this object
	Rigidbody rigidBody;
		///Custom shader
	Shader* shader = nullptr;
		
		///Name of this object
	std::string name;
		///Return this objects ID
	int GetID() { return ID; }
		///Set this objects ID  
	void SetID(int id) { ID = id; };
		///FSM for AI and sending/receiving messages
	StateMachine stateMachine;

		//retuns a reference to model data
	DrawItem& GetDrawItem();

protected:
		///Unique identifier
	unsigned int ID = 0;

	
};
