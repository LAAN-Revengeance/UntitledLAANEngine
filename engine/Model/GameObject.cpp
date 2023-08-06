#include "GameObject.h"


GameObject::GameObject() : stateMachine(this) {}

GameObject::~GameObject() {}

bool GameObject::CheckInFrustum(Frustum& frustum)
{
	float boundingRadius = model_data->maxBounds * std::max(std::max(scale.x,scale.y),scale.z);
	return
	(frustum.top.	CalcSignedDistance(position) > -boundingRadius &&
	frustum.bottom.	CalcSignedDistance(position) > -boundingRadius &&
	frustum.left.	CalcSignedDistance(position) > -boundingRadius &&
	frustum.right.	CalcSignedDistance(position) > -boundingRadius &&
	frustum.far.	CalcSignedDistance(position) > -boundingRadius &&
	frustum.near.	CalcSignedDistance(position) > -boundingRadius);
}

void GameObject::SetPosition(glm::vec3 nPos)
{
	rigidBody.SetPosition(nPos);
	position = nPos;
}

void GameObject::SetRotation(glm::vec3 nRot)
{
	rigidBody.SetRotation(nRot);
	rotation = nRot;
}

void GameObject::SetUniforms()
{
}

void GameObject::Update(double dt)
{
}


glm::vec3 GameObject::GetUpVec()
{
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	return glm::normalize(glm::vec3(rotationMatrix[0][1], rotationMatrix[1][1], rotationMatrix[2][1]));
}

glm::vec3 GameObject::GetForwardVec()
{
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	return glm::normalize(glm::vec3(rotationMatrix[0][2], rotationMatrix[1][2], rotationMatrix[2][2]));
}

glm::vec3 GameObject::GetRightVec()
{
	glm::mat4 rotationMatrix(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return glm::normalize(glm::vec3(rotationMatrix[0][0], rotationMatrix[1][0], rotationMatrix[2][0]));
}

void GameObject::LookAt(glm::vec3 lookvec)
{
	glm::vec3 dir = position - lookvec;
	glm::vec3 newRot(0.0f,0.0f,0.0f);

	dir = glm::normalize(dir);

	newRot.y = glm::degrees(atan2(dir.x, dir.z)) + 90.0f;
	
	newRot.x = glm::degrees(asin(dir.y));

	SetRotation(newRot);
}

DrawItem& GameObject::GetDrawItem()
{
	if (model_data) {
		return *model_data;
	}
	return *(new DrawItem);
}
