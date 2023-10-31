#include "Affordance_Pickup.h"
#include <GameObject.h>


AffordancePickup::AffordancePickup(GameObject* go)
{
	_parentObject = go;
}

AffordancePickup::~AffordancePickup()
{

}

void AffordancePickup::Activate(GameObject* go)
{
	if (!go->affordanceController.GetAffordance<AffordancePickup>()->GetCanAfford())
		return;

	_active = true;
	_otherObject = go;
	if (_otherObject->physicsBody) {
		_pickupKinematic = _otherObject->physicsBody->isKinematic;
		_otherObject->physicsBody->isKinematic = true;
	}

}

void AffordancePickup::Deactivate()
{
	_active = false;

	//return objects kinematic state to its original
	if (_otherObject->physicsBody) {
		_otherObject->physicsBody->isKinematic = _pickupKinematic;
	}
	_otherObject = nullptr;
}

void AffordancePickup::Update(double deltaTime)
{
	if (_active) {
		glm::vec3 forward = glm::normalize(_parentObject->GetForwardVec());
		if (_otherObject) {
			_otherObject->SetPosition(_parentObject->GetPosition() - (forward * _offset));
			_otherObject->LookAt(_parentObject->position);
		}
	}

}

void AffordancePickup::SetOffset(float offset)
{
	_offset = offset;
}