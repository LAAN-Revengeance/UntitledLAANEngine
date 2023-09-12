#include "GameObject.h"


GameObject::GameObject() : stateMachine(this) {}

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

void GameObject::SetRotation(glm::vec3 nRot)
{
	if (physicsBody)
	physicsBody->SetRotation(nRot.x, nRot.y, nRot.z);
	rotation = nRot;
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
	std::cout << "aaaaaaaaaaa\n";
	physicsBody->SetPosition(position.x,position.y,position.z);
	physicsBody->SetRotation(rotation.x, rotation.y, rotation.z);
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

glm::mat4 GameObject::GetTransformMatrix()
{
	glm::mat4 modelMat(1.0f);
	//modelMat = glm::scale(modelMat, scale);
	modelMat = glm::translate(modelMat, position);

	//pitch roll and yaw rotationss
	modelMat = glm::rotate(modelMat, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	modelMat = glm::rotate(modelMat, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	return modelMat;
}

DrawItem& GameObject::GetDrawItem()
{
	if (model_data) {
		return *model_data;
	}
	return *(new DrawItem);
}
