#pragma once
#include "Affordance.h"

class AffordanceCompliment : public Affordance
{
public:
	AffordanceCompliment(GameObject* go);
	~AffordanceCompliment();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	AffordanceDescriptor GetType()const override { return type; }

private:
	AffordanceDescriptor descriptor = "compliment";

	AffordanceDescriptor type = "agent";
};
#pragma once
