#include "EBO.h"
EBO::EBO() {
	ID = 0;
}

EBO::EBO(unsigned int *indexes, int size) {
	glGenBuffers(1,&ID);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,size,indexes,GL_STATIC_DRAW);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ID);
}

void EBO::UnBind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}