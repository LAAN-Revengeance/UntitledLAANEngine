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

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	AffordanceDescriptor GetType()const override { return type; }

	void SetOffset(float offset);
	
private:
	AffordanceDescriptor descriptor = "pickup";
	AffordanceDescriptor type = "agent";
  
	float _offset = 2.5f;

	//track if pickup objects physics body was kinematic before being picked up
	bool _pickupKinematic = false;
};
