#pragma once
#include <glm/glm.hpp>

const int COLLIDER_INVALID	= 0;
const int COLLIDER_BOX		= 1;
const int COLLIDER_SPHERE	= 2;
const int COLLIDER_CAPSULE	= 3;
const int COLLIDER_TERRAIN	= 4;

struct PhysicsCollider
{
	PhysicsCollider() :
		mass(1),
		bounce(0.5),
		friction(0.3),
		offset({0,0,0}),
		rotation({0,0,0})
	{
		type = COLLIDER_INVALID;
	}

	float mass;
	float bounce;
	float friction;
	glm::vec3 offset;
	glm::vec3 rotation;

	int GetType() {
		return type;
	}

protected:
	int type;
};

struct BoxCollider : public PhysicsCollider
{
	BoxCollider(float nMass, float nBounce, float nFriction, glm::vec3 nOffset, glm::vec3 nRotation,glm::vec3 nScale)
	{
		mass = nMass;
		bounce = nBounce;
		friction = nFriction;
		type = COLLIDER_BOX;
		offset = nOffset;
		rotation = nRotation;
		scale = nScale;
		type = COLLIDER_BOX;
	}
	glm::vec3 scale;
};

struct SphereCollider : public PhysicsCollider
{
	SphereCollider(float nMass, float nBounce, float nFriction, glm::vec3 nOffset, glm::vec3 nRotation,float nRadius)
	{
		mass = nMass;
		bounce = nBounce;
		friction = nFriction;
		type = COLLIDER_BOX;
		offset = nOffset;
		rotation = nRotation;
		radius = nRadius;
		type = COLLIDER_SPHERE;
	}
	float radius;
};

struct CapsuleCollider : public PhysicsCollider
{
	CapsuleCollider(float nMass, float nBounce, float nFriction, glm::vec3 nOffset, glm::vec3 nRotation, float nRadius, float nHeight)
	{
		mass = nMass;
		bounce = nBounce;
		friction = nFriction;
		type = COLLIDER_BOX;
		offset = nOffset;
		rotation = nRotation;
		radius = nRadius;
		height = nHeight;
		type = COLLIDER_CAPSULE;
	}
	float radius;
	float height;
};

struct TerrainCollider : public PhysicsCollider
{
	TerrainCollider(float nMass, float nBounce, float nFriction, glm::vec3 nOffset, glm::vec3 nRotation, int nRows, int nCols, int nMin, int nMax, float* nHeights)
	{
		mass = nMass;
		bounce = nBounce;
		friction = nFriction;
		type = COLLIDER_BOX;
		offset = nOffset;
		rotation = nRotation;

		rows = nRows;
		columns = nCols;
		min = nMin;
		max = nMax;
		heights = nHeights;
		type = COLLIDER_TERRAIN;
	}
	int rows;
	int columns;
	float min;
	float max;
	float* heights;
};