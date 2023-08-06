#pragma once
#include <GameObject.h>
#include <map>
#include <string>

/**
*	@Class GameObject
*	@brief Subclass of GameObject. Provides functionality for typical NPC
*	behaviour and ability to store arbitrary data in a map<string,float>
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 27/05/2023
*/
class NPC : public GameObject
{
public:
		/**
		*	@brief Default constructor
		*/
	NPC();

		/**
		*	@brief destructor
		*/
	~NPC();
	
		/**
		*	@brief Add npc data
		*	@param dataName name of data
		*	@param value value of data
		*	@return void
		*/
	void AddData(const std::string& dataName, float value);

		/**
		*	@brief check if data exists
		*	@param dataName data to be checked
		*	@return bool if data dataName exists in map
		*/
	bool HasData(const std::string& dataName);

		/**
		*	@brief Returns stored data, returns 0.0 if data does not exist
		*	@param dataName name of data to be retrieved
		*	@return float data with name dataName
		*/
	float GetData(const std::string& dataName);

		/**
		*	@brief Implementation of GameObject Update method, moves NPC closer
		*	to its target position if it is targeting
		*	@param dt time since last update
		*	@return void
		*/
	void Update(double dt);

		/**
		*	@brief set target to a positon in 3D space. Will not stop moving to target until it is 
		*	within offset distance of target
		*	@param nPos position of taregt
		*	@param speed how fast to move to nPos
		*	@param offset how close to get to nPos
		*	@return void
		*/
	void MoveTo3D(glm::vec3 nPos, float speed, float offset);

		/**
		*	@brief set target to a positon in 2D space along the X and Z axis. Will not stop moving to target until it is
		*	within offset distance of target
		*	@param nPos position of taregt
		*	@param speed how fast to move to nPos
		*	@param offset how close to get to nPos
		*	@return void
		*/
	void MoveTo2D(glm::vec3 nPos, float speed, float offset);

		/**
		*	@brief stops this NPC from targeting
		*	@return void
		*/
	void StopMoving();

		/**
		*	@brief Retuns if this NPC is currently targting a position
		*	@return if NPC is currently targeting
		*/
	bool IsTargeting();


		///stores arbitrary values to be used later. Allows information about NPC to be serialized
	std::map<std::string, float> data;
private:

		///Position NPC is moving to
	glm::vec3 targetPos;
		///How fast NPC is moving
	float moveSpeed;
		///how close to stop in front of target
	float moveOffset;
		///If NPC is currently moving to targetPos
	bool isTargeting;
		///If NPC is moving in 2D or 3D space
	bool move3D;

	///movement variables
	glm::vec3 offsetPos;
	glm::vec3 toOffset;
	glm::vec3 newPos;
	glm::vec3 toTarget;
};
