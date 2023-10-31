#include "Affordance_Sit.h"
#include <GameObject.h>

AffordanceSit::AffordanceSit(GameObject* go)
{
	_parentObject = go;
}

AffordanceSit::~AffordanceSit()
{
}

void AffordanceSit::Activate(GameObject* go)
{
	if (!go->affordanceController.GetAffordance<AffordanceSit>()->GetCanAfford())
		return;

	_active = true;
	_otherObject = go;

	if (_parentObject->physicsBody) {
		_sitterKinematic = _parentObject->physicsBody->isKinematic;
		_parentObject->physicsBody->isKinematic = true;
	}
}

void AffordanceSit::Deactivate()
{
	_active = false;
	//return objects kinematic state to its original
	if (_parentObject->physicsBody) {
		_parentObject->physicsBody->isKinematic = _sitterKinematic;
	}

	_otherObject = nullptr;
}

void AffordanceSit::Update(double deltaTime)
{
	if (_active) {
		glm::vec3 up = _otherObject->GetUpVec();

		if (_otherObject) {
			_parentObject->SetPosition(_otherObject->GetPosition() + up * _offset);
		}
	}
}

void AffordanceSit::SetOffset(float offset)
{
	_offset = offset;
}
