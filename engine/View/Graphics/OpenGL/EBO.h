#pragma once
#include<glad/glad.h>
class EBO {

public:
	///ID used by opengl to identify the EBO
	unsigned int ID;
	/**
	*	@brief Default constructor
	*/
	EBO();
		/**
		*	@brief constructor with inital values
		*	@indexes array of index values
		*	@size amount of indexes in indexes
		*/
	EBO(unsigned int *indexes, int size);
		/**
		*	@brief Binds this EBO to be used on next draw
		*	@return void
		*/
	void Bind();
		/**
		*	@brief Un-Binds this EBO
		*	@return void
		*/
	void UnBind();
		/**
		*	@brief deletes the EBO data
		*/
	void Delete();
};