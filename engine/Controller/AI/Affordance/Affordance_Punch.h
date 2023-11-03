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

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }
	AffordanceDescriptor GetType()const override { return type; }

private:
	AffordanceDescriptor descriptor = "punch";
	AffordanceDescriptor type = "agent";
};