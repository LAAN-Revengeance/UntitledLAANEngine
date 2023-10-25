#pragma once
#include <Physics/RigidBody.h>

/*
*	@class ForceGenerator
*	
*	applys specific forces to a physicsbody each update
*/
class ForceGenerator
{
public:
	virtual void Update(PhysicsBody* pb, double deltaTime) = 0;
};