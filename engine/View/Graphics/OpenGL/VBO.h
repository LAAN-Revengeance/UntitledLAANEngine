#pragma once
#include<glad/glad.h>
#include<vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class VBO {
public:
	///ID used by opengl to identify the VBO
	unsigned int ID = -1;
		/**
		*	@brief default constructor
		*/
	VBO();
		/**
		*	@brief constructor with inital values
		*	@param verts array of vertex values
		*	@param size amount of vertexes in indexes
		*/
	VBO(void* verts, int size);
		/**
		*	@brief constructor with for a VBO of transform matricies
		*	used in instanced rendering
		*	@param matrix transforms for instances
		*/
	VBO(std::vector<glm::mat4> mData);
		/**
		*	@brief Binds this VBO for use
		*	@return void
		*/
	void Bind();
		/**
		*	@brief Unbinds this VBO
		*	@return void
		*/
	void UnBind();
		/**
		*	@breif deletes this VBO
		*	@return void
		*/
	void Delete();
		/**
		*	@breif replace part of this buffers data
		*	@param offset where data subbing begins
		*	@param size size in bytes being replaced
		*	@param data pointer to data being subbed
		*	@return void
		*/
	void SubData(int offset, int size, void* data);
};