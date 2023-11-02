#include "AffordanceController.h"

AffordanceBlank AffordanceController::_blankAffordance(nullptr);

AffordanceController::AffordanceController(GameObject* owner)
{
	_owner = owner;
}

AffordanceController::~AffordanceController()
{

}

void AffordanceController::Update(double deltaTime)
{
	for (auto& affordance : _affordances) {
		affordance->Update(deltaTime);
	}
}

Affordance* AffordanceController::GetAffordanceString(const std::string type)
{
	for (auto& affordance : _affordances)
	{
		if (affordance->GetType() == type) {
			return affordance;
		}
	}

	return &_blankAffordance;
}

void AffordanceController::RemoveAffordanceString(const std::string type)
{
	for (auto it = _affordances.begin(); it != _affordances.end();)
	{
		if ((*it)->GetType() ==  type)
		{
		    delete* it;
		    it = _affordances.erase(it);
		}
		else
		{
		    ++it;
		}
	}
}

