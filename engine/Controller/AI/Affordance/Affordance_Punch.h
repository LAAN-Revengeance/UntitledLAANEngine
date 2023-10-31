#pragma once
#include "Affordance.h"

class AffordancePunch : public Affordance
{
public:
	AffordancePunch(GameObject* go);
	~AffordancePunch();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetType()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "punch";
};