#pragma once

#include "Actions.h"

class Affordance
{
public:

	/// Default constrcutor
	Affordance();

	/// Destructor
	~Affordance();

	/// Non-default constructor
	Affordance(Actions newAction, float newWeight);
	
	/// Gets the weight of the affordance
	float GetWeight();

	/// Sets the weight of the affordance
	void SetWeight(float newWeight);

	/// Gets the action of the affordance from a set number of actions within an enum
	Actions GetAction();

	/// Sets the action of the affordance
	void SetAction(Actions newAction);

private:
	/// Action from an enum of specified actions that determines what the affordance does
	Actions actionName;

	/// Weight of the action associated with this affordance
	float actionWeight;
};