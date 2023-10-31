#include "Affordance_Slap.h"
#include <GameObject.h>


AffordanceSlap::AffordanceSlap(GameObject* go)
{
	_parentObject = go;
}

AffordanceSlap::~AffordanceSlap()
{

}

void AffordanceSlap::Activate(GameObject* go)
{
	_active = true;
	_otherObject = go;
}

void AffordanceSlap::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordanceSlap::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting slapped" << std::endl;
	}
}