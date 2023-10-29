#include "Affordance.h"

Affordance::Affordance()
{
	actionName = Actions::NONE;
	actionWeight = 0.0f;
}

Affordance::Affordance(Actions newAction, float newWeight)
{
	actionName = newAction;
	actionWeight = newWeight;
}

Affordance::~Affordance()
{
}

float Affordance::GetWeight()
{
	return actionWeight;
}

void Affordance::SetWeight(float newWeight)
{
	// Keep weight values between 0 and 1
	if (newWeight > 1.0f)
	{
		actionWeight = 1.0f;
	}
	else if (newWeight < 0.0f) {
		actionWeight = 0.0f;
	}
	else {
		actionWeight = newWeight;
	}
}

Actions Affordance::GetAction()
{
	return actionName;
}

void Affordance::SetAction(Actions newAction)
{
	actionName = newAction;
}