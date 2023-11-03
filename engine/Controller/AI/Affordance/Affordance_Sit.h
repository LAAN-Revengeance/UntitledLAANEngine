#pragma once
#include "Affordance.h"

class AffordanceSit : public Affordance
{
public:
	AffordanceSit(GameObject* go);
	~AffordanceSit();

	void Activate(GameObject* go) override;
	void Deactivate() override;
	void Update(double deltaTime) override;

	AffordanceDescriptor GetDescriptor()const override { return descriptor; }

	AffordanceDescriptor GetType()const override { return type; }

	void SetOffset(float offset);

private:
	AffordanceDescriptor descriptor = "sit";
	AffordanceDescriptor type = "agent";
	float _offset = 1.0f;

	//track if the sitter(_parent) was kinematic before sitting
	bool _sitterKinematic = false;
};
