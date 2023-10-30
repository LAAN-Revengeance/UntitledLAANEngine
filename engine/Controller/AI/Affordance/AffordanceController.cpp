#include "AffordanceController.h"

AffordanceController::AffordanceController(GameObject* owner)
{
	_owner = owner;
}

AffordanceController::~AffordanceController()
{

}

void AffordanceController::Update(double delteTime)
{
	for (auto& affordance : _affordances) {
		affordance->Update(delteTime);
	}
}

