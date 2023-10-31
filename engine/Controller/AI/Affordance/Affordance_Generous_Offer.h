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

	AffordanceDescriptor GetType()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "generousOffer";
};
#pragma once
