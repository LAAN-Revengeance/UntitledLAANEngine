#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "glm/gtx/hash.hpp"
#include <iostream>
#include <fstream>
#include <unordered_map>
	/**
	*	@struct vertex
	*	@brief Used to store vertex information of
	*	3D model data
	*/
struct vertex {
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 texCoord;

};
	/**
	*	@struct OBJData
	*	@brief Used to store .obj file vertex
	*	data. currently does not support .mtl files
	*/
struct OBJData
{
	std::vector<unsigned int> elements;
	std::vector<vertex> vertexData;

	float boundingShpere = 0.0f;
};
	/*
	*	@brief Reads in vertex information and returns and OBJ data
	*	struct.
	*	@return reference to heap allocated obj data struct
	*/
OBJData* ReadObjFile(const char* fileName);