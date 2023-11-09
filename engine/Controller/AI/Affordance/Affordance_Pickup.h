#pragma once
#include "Affordance.h"

/**
*	@Class AffordancePickup
*	@brief Extension of virtual affordance class for pick up affordance
*
*	@author Andres Comeros-Ochtman, Lochy Edward
*	@version 1.0
*	@date 15/10/2023
*/
class AffordancePickup : public Affordance
{
public:
	/**
	 * @brief Constructor. Affordances require an owner to function
	 * @param owner object that owns this affordance
	*/
	AffordancePickup(GameObject* go);
	
	/**
	 * @brief Destructor
	*/
	~AffordancePickup();

	/**
	 * @brief Use this affordance on another agent if they afford this action
	 * @param go
	*/
	void Activate(GameObject* go) override;
	
	/**
	 * @brief Disable this affordances effect on the currently object
	 * this affordance is acting on.
	*/
	void Deactivate() override;
	
	/**
	 * @brief Execute this affordances effect if it is required to occur over time
	 * such as picking up an object
	 * @param deltaTime
	*/
	void Update(double deltaTime) override;

	/**
	 * @brief Get a description of this affordance
	 * @return the descriptor of this affordance
	*/
	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	/**
	 * @brief Get the type of this affordance, used to determine if affordance prospect is relevant
	 * @return the descriptor of this affordance
	*/
	AffordanceDescriptor GetType()const override { return type; }

	/**
	 * @brief Set the offset value for this affordance
	 * @param offset
	*/
	void SetOffset(float offset);
	
private:
	/// descriptor of this affordance, used to determine what action it will perform
	AffordanceDescriptor descriptor = "pickup";
	
	/// type of this affordance, used to determine if affordance prospect is relevant
	AffordanceDescriptor type = "agent";
	
	// how far the object will be held in front of the agent
	float _offset = 2.5f;

	//track if pickup objects physics body was kinematic before being picked up
	bool _pickupKinematic = false;
};
