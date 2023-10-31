#pragma once
#include "Affordance.h"

class AffordanceThreaten : public Affordance
{
public:
	AffordanceThreaten(GameObject* go);
	~AffordanceThreaten();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetType()const override { return descriptor; }

private:
	AffordanceDescriptor descriptor = "threaten";
};
#pragma once
