#pragma once
#include "Affordance.h"


class AffordancePickup : public Affordance
{
public:
	AffordancePickup(GameObject* go);
	~AffordancePickup();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

private:

	float _offset = 1.0f;
};
