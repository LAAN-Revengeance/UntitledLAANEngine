#pragma once
#include "ForceGenerator.h"

class ForceRegistry
{
public:
	
	//Apply forces to physicsbodies with associated force generators
	void Update(double deltaTime);

	//Set object to be affected by force generator
	void Add(PhysicsBody* pb, ForceGenerator* fg);

	//Set object to not be affected by force generator
	void Remove(PhysicsBody* pb, ForceGenerator* fg);

	//Clears all physicsbody, force generator relations
	void Clear();

private:

	typedef struct ForceRegistration
	{
		PhysicsBody* pb;
		ForceGenerator* fg;
	};

	std::vector<ForceRegistration> forceRegistrations;
};
