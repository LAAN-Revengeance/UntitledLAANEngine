#include "ForceRegister.h"

void ForceRegistry::Update(double deltaTime)
{
	for (auto& it : forceRegistrations) {
		it.fg->Update(it.pb,deltaTime);
	}
}

void ForceRegistry::Add(PhysicsBody* pb, ForceGenerator* fg)
{
	forceRegistrations.push_back({ pb,fg });
}

void ForceRegistry::Remove(PhysicsBody* pb, ForceGenerator* fg)
{
	for (size_t i = 0; i < forceRegistrations.size(); i++)
	{
		if (forceRegistrations[i].pb == pb && forceRegistrations[i].fg == fg) {
			forceRegistrations.erase(forceRegistrations.begin() + i);
		}
	}
}

void ForceRegistry::Clear()
{
	forceRegistrations.clear();
}
