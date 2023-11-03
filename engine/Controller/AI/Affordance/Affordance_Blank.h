#pragma once
#include "Affordance.h"

class AffordanceBlank : public Affordance
{
public:
	AffordanceBlank(GameObject* go) {}
	~AffordanceBlank() {}

	void Activate(GameObject* go) override {};
	void Deactivate() override {};
	void Update(double deltaTime) override {};

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "blank";
};
#pragma once