#include "VBO.h"

VBO::VBO() {
	glGenBuffers(1, &ID);
}

VBO::VBO(void* verts, int size) {
	glGenBuffers(1,&ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER,size,verts,GL_STATIC_DRAW);
}

VBO::VBO(std::vector<glm::mat4> mData) {
	if(ID < 0)
		glGenBuffers(1, &ID);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, mData.size() * sizeof(glm::mat4), mData.data(), GL_STATIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::UnBind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}

void VBO::SubData(int offset, int size, void* data)
{
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
