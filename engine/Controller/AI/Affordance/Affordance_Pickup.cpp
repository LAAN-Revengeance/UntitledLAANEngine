#include "Affordance_Pickup.h"
#include <GameObject.h>

AffordancePickup::AffordancePickup()
{

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
		
		//do pickup stuff
		_otherObject->Update(deltaTime);
	}

}
