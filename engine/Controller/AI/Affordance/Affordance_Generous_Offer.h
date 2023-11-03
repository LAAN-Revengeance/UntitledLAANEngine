#pragma once
#include "Affordance.h"

class AffordanceGenerousOffer : public Affordance
{
public:
	AffordanceGenerousOffer(GameObject* go);
	~AffordanceGenerousOffer();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	AffordanceDescriptor GetType()const override { return type; }

private:
	AffordanceDescriptor descriptor = "generousOffer";

	AffordanceDescriptor type = "event";
};
#pragma once
