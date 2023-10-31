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

	AffordanceDescriptor GetType()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "compliment";
};
#pragma once
