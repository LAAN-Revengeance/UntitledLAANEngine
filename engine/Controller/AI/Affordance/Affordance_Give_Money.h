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

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }
	AffordanceDescriptor GetType()const override { return type; }

private:
	AffordanceDescriptor descriptor = "giveMoney";

	AffordanceDescriptor type = "agent";
};
