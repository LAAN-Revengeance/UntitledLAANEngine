#pragma once

class GameObject;

class Affordance
{
public:
	Affordance(GameObject* owner) {};
	~Affordance() {};

	virtual void Activate(GameObject* go) = 0;
	virtual void Deactivate() = 0;
	virtual void Update(double deltaTime) = 0;

protected:

	bool _active = false;

	GameObject* _parentObject = nullptr;
	GameObject* _otherObject = nullptr;
};