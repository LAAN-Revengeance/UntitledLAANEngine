#pragma once
#include<glad/glad.h>
#include "VBO.h"

class VAO {
public:
		///ID used by opengl to identify the VAO
	unsigned int ID;

		/**
		*	@breif default constructor
		*	@Details Generates a new VAO with a unique ID.
		*/
	VAO();
		/**
		*	@brief sets up how opengl should interpret vertex data when using this VAO
		*	@param VBO the vbo this VAO will be used with
		*	@param layout index of the attribute to be modified
		*	@param numComponents number of values this attribute has, e.g UV coords would have 2.
		*	@param type type of data the attribute is composed of, uses OPENGL Enums to identift: e.g GL_FLOAT
		*	@param stride offset from this attribute to its next occurence in the VBO
		*	@param offset in bytes from start of VBO to the first occurence of this attribute.
		* 
		*	@return void
		*/
	void AddAttribute(VBO &VBO, unsigned int layout, unsigned int numComponents, unsigned int type, GLsizeiptr stride, void* offset);
		/**
		*	@brief binds this VAO for use in next draw
		*	@return void
		*/
	void Bind();
		/**
		*	@breif unbinds this VAO
		*	@return void
		*/
	void UnBind();
		/**
		*	@breif deletes this VAO
		*	@return void
		*/
	void Delete();
};