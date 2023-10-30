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
	_active = true;
	_otherObject = go;
}

void AffordancePickup::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordancePickup::Update(double deltaTime)
{
	if (_active) {
		glm::vec3 forward = glm::normalize(_parentObject->GetForwardVec());
		_otherObject->SetPosition(_parentObject->GetPosition() + (forward  * _offset));
	}

}

void AffordancePickup::SetOffset(float offset)
{
	_offset = offset;
}
