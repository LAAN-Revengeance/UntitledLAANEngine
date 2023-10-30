#pragma once
#include <string>

class GameObject;

using AffordanceDescriptor = std::string;

class Affordance
{
public:
	Affordance() {}
	Affordance(GameObject* owner) {};
	~Affordance() {};

	virtual void Activate(GameObject* go) = 0;
	virtual void Deactivate() = 0;
	virtual void Update(double deltaTime) = 0;

	void SetCanAfford(bool canAfford) { _canAfford = canAfford; }
	void SetCanPerform(bool canPerform) { _canPerform = canPerform; }
	bool GetIsActive() { return _active; }

	virtual AffordanceDescriptor GetType()const = 0;

protected:

	bool _active = false;
	//if other objects can use this affordance on _parentObject
	bool _canAfford = false;

	//if _parentObject can perform this affordance
	bool _canPerform = false;

	GameObject* _parentObject = nullptr;
	GameObject* _otherObject = nullptr;


};