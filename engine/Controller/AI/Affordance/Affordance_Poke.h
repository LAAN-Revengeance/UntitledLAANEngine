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

	AffordanceDescriptor GetType()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "poke";
};
