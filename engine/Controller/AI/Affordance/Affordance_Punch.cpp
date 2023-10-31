#include "Affordance_Punch.h"
#include <GameObject.h>


AffordancePunch::AffordancePunch(GameObject* go)
{
	_parentObject = go;
}

AffordancePunch::~AffordancePunch()
{

}

void AffordancePunch::Activate(GameObject* go)
{
	_active = true;
	_otherObject = go;
}

void AffordancePunch::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordancePunch::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting punched" << std::endl;
	}
}