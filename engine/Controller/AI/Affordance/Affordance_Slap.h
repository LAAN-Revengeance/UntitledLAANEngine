#pragma once
#include "Affordance.h"


class AffordanceSlap : public Affordance
{
public:
	AffordanceSlap(GameObject* go);
	~AffordanceSlap();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetType()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "slap";
};