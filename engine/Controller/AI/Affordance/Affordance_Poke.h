#pragma once
#include "Affordance.h"


class AffordancePoke : public Affordance
{
public:
	AffordancePoke(GameObject* go);
	~AffordancePoke();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	AffordanceDescriptor GetType()const override { return type; }

private:
	AffordanceDescriptor descriptor = "poke";

	AffordanceDescriptor type = "agent";
};
