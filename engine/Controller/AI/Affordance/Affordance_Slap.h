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

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	AffordanceDescriptor GetType()const override { return type; }

private:
	AffordanceDescriptor descriptor = "slap";

	AffordanceDescriptor type = "agent";
};