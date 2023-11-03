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

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	AffordanceDescriptor GetType()const override { return type; }

private:
	AffordanceDescriptor descriptor = "threaten";
	AffordanceDescriptor type = "event";
};
#pragma once
