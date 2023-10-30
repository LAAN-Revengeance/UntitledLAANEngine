#pragma once
#include "Affordance.h"


class AffordancePickup : public Affordance
{
public:
	AffordancePickup();
	~AffordancePickup();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

private:

};
