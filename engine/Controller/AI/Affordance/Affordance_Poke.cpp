#include "Affordance_Poke.h"
#include <GameObject.h>


AffordancePoke::AffordancePoke(GameObject* go)
{
	_parentObject = go;
}

AffordancePoke::~AffordancePoke()
{

}

void AffordancePoke::Activate(GameObject* go)
{
	_active = true;
	_otherObject = go;
}

void AffordancePoke::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordancePoke::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting poked" << std::endl;
	}
}