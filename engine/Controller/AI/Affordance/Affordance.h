#pragma once
#include <string>
class GameObject;

using AffordanceDescriptor = std::string;

/**
*	@Class Affordance
*	@brief Virtual class that descrbes a common interface for adding actions
*	that objects can perform on other objects or allow other objects to perform on
*	them
*
*	@author Andres Comeros-Ochtman, Lochy Edward
*	@version 1.0
*	@date 15/10/2023
*/
class Affordance
{
public:
	
	/**
	 * @brief Constructor. Affordances require an owner to function
	 * @param owner object that owns this affordance
	*/
	Affordance(GameObject* owner) {};

	/**
	 * @brief Destructor
	*/
	~Affordance() {};

	/**
	 * @brief Use this affordance on another agent if they afford this action
	 * @param go 
	*/
	virtual void Activate(GameObject* go) = 0;

	/**
	 * @brief Disable this affordances effect on the current object
	 * this affordance is acting on.
	*/
	virtual void Deactivate() = 0;

	/**
	 * @brief Execute this affordances effect if it is required to occur over time
	 * such as picking up an object
	 * @param deltaTime 
	*/
	virtual void Update(double deltaTime) = 0;

	/**
	 * @brief Set whether the owner of this affordance can afford this action to other objects,
	 * for example a sit affordance would be true for a chair
	 * @param canAfford if this object can afford this affordances effect
	*/
	void SetCanAfford(bool canAfford) { _canAfford = canAfford; }

	/**
	 * @brief Set whether the owner of this affordance can perform the effect of this affordance
	 * @param canPerform if this object can perform this affordances effect on other objects
	*/
	void SetCanPerform(bool canPerform) { _canPerform = canPerform; }

	/**
	 * @brief Get if this object can perform this affordance
	 * @return if can perform this action
	*/
	bool GetCanPerform() { return _canPerform; }

	/**
	 * @brief Get kf this object can afford this affordance
	 * @return if other objects can perform this action on this affordances owner
	*/
	bool GetCanAfford() { return _canAfford; }

	/**
	 * @brief Get if this affordance is currently being used
	 * @return if this affordance is being used
	*/
	bool GetIsActive() { return _active; }

	/**
	 * @brief Get strength of this affordance
	 * @return the affordance strength
	*/
	float GetStrength() { return _strength; }

	/**
	 * @brief Get if this affordance is dangerous
	 * @return id this affordance is dangerous
	*/
	float GetDanger() { return _dangerous; }

	/**
	 * @brief return object being acted upon
	 * @return object being activated by this affordance
	*/
	GameObject* GetOther() { return _otherObject; }

	/**
	 * @brief Get a description of this affordance
	 * @return the descriptor of this affordance
	*/
	virtual AffordanceDescriptor GetDescriptor()const = 0;

	/**
	 * @brief Get the type of this affordance, used to determine if affordance prospect is relevant
	 * @return the descriptor of this affordance
	*/
	virtual AffordanceDescriptor GetType()const = 0;

protected:

	///hidden default constructor, affordances require an owner
	Affordance() {}

	///if this affordance is in effect/being used
	bool _active = false;

	//if other objects can use this affordance on _parentObject
	bool _canAfford = false;

	//if _parentObject can perform this affordance
	bool _canPerform = false;

	//strength of current affordance
	float _strength;

	//if _parentObject can perform this affordance
	bool _dangerous = false;

	///the owner of this affordance data
	GameObject* _parentObject = nullptr;

	///the object this affordance is currently affecting
	GameObject* _otherObject = nullptr;

};