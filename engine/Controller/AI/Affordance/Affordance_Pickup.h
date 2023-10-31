#pragma once
#include "Affordance.h"


class AffordancePickup : public Affordance
{
public:
	AffordancePickup(GameObject* go);
	~AffordancePickup();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetType()const override { return descriptor; }

	void SetOffset(float offset);
	
private:
	AffordanceDescriptor descriptor = "pickup";
	float _offset = 1.0f;
};