#include "GameObject.h"


GameObject::GameObject(){
}

GameObject::~GameObject() {}

bool GameObject::CheckInFrustum(Frustum& frustum)
{
	if (!model_data)
		return false;
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
	if(physicsBody)
	physicsBody->SetPosition(nPos.x,nPos.y,nPos.z);
	position = nPos;
}

glm::vec3 GameObject::GetPosition()
{
	return position;
}

void GameObject::SetRotationEuler(float x, float y, float z)
{
	if (!physicsBody)
		return;

	physicsBody->SetRotationEuler(x,y,z);

	glm::mat4 modelMat(1.0f);
	modelMat = glm::rotate(modelMat, (z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::rotate(modelMat, (y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, (x), glm::vec3(1.0f, 0.0f, 0.0f));

	SetRotation(glm::quat(modelMat));
}

void GameObject::Rotate(float x, float y, float z, float angle)
{
	glm::mat4 rotationMat = glm::mat4_cast(orientation);
	rotationMat = glm::rotate(rotationMat, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
	rotationMat = glm::rotate(rotationMat, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMat = glm::rotate(rotationMat, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));

	SetRotation(glm::quat(rotationMat));
}


void GameObject::SetRotation(glm::quat nRot)
{
	orientation = glm::normalize(nRot);
	if (!physicsBody)
		return;
	physicsBody->SetRotation(nRot);
	
}

void GameObject::SetScale(glm::vec3 nScale)
{
	scale = nScale;
}

void GameObject::SetUniforms()
{
}

void GameObject::Update(double dt)
{
	if (physicsBody)
	{
		SetPosition(physicsBody->GetPosition());
		SetRotation(physicsBody->GetRotation());
	}
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

glm::vec3 GameObject::GetRotationEuler()
{
	return (glm::eulerAngles(orientation));
}

glm::mat4 GameObject::GetTransformMatrix()
{
	glm::mat4 modelMat(1.0f);
	//modelMat = glm::scale(modelMat, scale);
	modelMat = glm::translate(modelMat, position);

	glm::mat4 rotationMat = glm::mat4_cast(orientation);
	modelMat *= rotationMat;

	return modelMat;
}

DrawItem* GameObject::GetDrawItem()
{
	if (model_data) {
		return model_data;
	}
	return nullptr;
}
