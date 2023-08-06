#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1,&ID);
	Bind();
}

void VAO::AddAttribute(VBO &VBO, unsigned int layout, unsigned int numComponents, unsigned int type, GLsizeiptr stride, void* offset) {
	Bind();
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.UnBind();
}

void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::UnBind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}