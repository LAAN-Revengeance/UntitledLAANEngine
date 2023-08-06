#include "NPC.h"

NPC::NPC() : 
targetPos({ 0.0f,0.0f,0.0f }),
moveSpeed(0.0f),
moveOffset(0.0f),
isTargeting(false),
move3D(false)
{
}

NPC::~NPC()
{
}

void NPC::AddData(const std::string& dataName, float value)
{
	data[dataName] = value;
	//data.insert({ dataName, value });
}



float NPC::GetData(const std::string& dataName)
{
	if (data.find(dataName) != data.end()) {
		return data.at(dataName);
	}
	else {
		return 0.0f;
	}
}


bool NPC::HasData(const std::string& dataName)
{
	if (data.find(dataName) != data.end()) {
		return true;
	}
	else {
		return false;
	}
}

void NPC::Update(double dt)
{
	if (!isTargeting) {
		return;
	}

	if (move3D)
	{
		toTarget = glm::normalize(targetPos - position);
		newPos = position + (toTarget) * (float)(moveSpeed * dt);
		offsetPos = targetPos - (toTarget * moveOffset);
		toOffset = offsetPos - newPos;

		if (glm::dot(toOffset, toTarget) < 0.0f) {
			newPos = offsetPos;
			isTargeting = false;
		}
	}
	else {
		glm::vec3 targetPosFlat(targetPos.x, position.y, targetPos.z);

		toTarget = glm::normalize(targetPosFlat - position);
		newPos = position + (toTarget) * (float)(moveSpeed * dt);
		offsetPos = targetPos - (toTarget * moveOffset);
		toOffset = offsetPos - newPos;

		if (glm::dot(toOffset, toTarget) < 0.0f) {
			newPos = offsetPos;
			isTargeting = false;
		}
	}

	position = newPos;
}

void NPC::MoveTo3D(glm::vec3 nPos, float speed, float offset)
{
	isTargeting = true;
	move3D = true;
	targetPos = nPos;
	moveSpeed = speed;
	moveOffset = offset;
}

void NPC::MoveTo2D(glm::vec3 nPos, float speed, float offset)
{
	isTargeting = true;
	move3D = false;
	targetPos = nPos;
	moveSpeed = speed;
	moveOffset = offset;
}

void NPC::StopMoving()
{
	isTargeting = false;
}

bool NPC::IsTargeting()
{
	return isTargeting;
}
