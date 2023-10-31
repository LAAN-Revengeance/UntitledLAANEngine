#pragma once
#include "Affordance.h"

class AffordanceGiveMoney : public Affordance
{
public:
	AffordanceGiveMoney(GameObject* go);
	~AffordanceGiveMoney();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetType()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "giveMoney";
};
