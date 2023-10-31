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
	float _offset = 2.0f;

	//track if pickup objects physics body was kinematic before being picked up
	bool _pickupKinematic = false;
};
