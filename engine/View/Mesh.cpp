#include "Mesh.h"
#include <vector>

Mesh::Mesh(void): elementCount(0),
vertCount(0), 
instanceCount(1), 
modelData(nullptr) {}

Mesh::Mesh(const char* fileName): instanceCount(1) {
	ReadOBJ(fileName);
}

Mesh::~Mesh() {}

Mesh::Mesh(const char* fileName, std::vector<glm::mat4> nMatrix) {

	modelData = ReadObjFile(fileName);
	vertCount = (int)modelData->vertexData.size();
	elementCount = (int)modelData->elements.size();
	SetInstanceMatrix(nMatrix);
}

void Mesh::SetInstanceMatrix(std::vector<glm::mat4> nMatrix) {
	
	FreeData();
	instanceMatrixes = nMatrix;
	instanceCount = (int)nMatrix.size();

	VAO nVAO;
	VBO iVBO(nMatrix);
	VBO nVBO(&modelData->vertexData[0].vertex.x, sizeof(float) * (int)modelData->vertexData.size() * 8);
	EBO nEBO(&modelData->elements[0], sizeof(unsigned int) * (int)modelData->elements.size());

	//normal vert data
	nVAO.AddAttribute(nVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	nVAO.AddAttribute(nVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	nVAO.AddAttribute(nVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//instance data
	iVBO.Bind();
	nVAO.AddAttribute(iVBO, 3, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
	nVAO.AddAttribute(iVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
	nVAO.AddAttribute(iVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	nVAO.AddAttribute(iVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	ebo = nEBO;
	vao = nVAO;
	vbo = nVBO;
	ivbo = iVBO;

	vao.UnBind();
	vbo.UnBind();
	ivbo.UnBind();
	ivbo.Delete();
}

OBJData* Mesh::GetModelData() { return modelData; }

std::vector<glm::mat4>* Mesh::getInstanceMatrix() { return &instanceMatrixes; }

void Mesh::ReadOBJ(const char* fileName) {

	FreeData();
	modelData = ReadObjFile(fileName);
	maxBounds = modelData->boundingShpere;
	SetVertexData(&modelData->vertexData[0].vertex.x, (int)modelData->vertexData.size(), &modelData->elements[0], (int)modelData->elements.size());

}

void Mesh::SetVertexData(float* nVertexData, int numData, unsigned int* vertIndexes, int numIndex) {
	FreeData();
	vertCount = numData;
	elementCount = numIndex;
	VAO nVAO;
	VBO nVBO(nVertexData, sizeof(float) * numData * 8);
	EBO nEBO(vertIndexes, sizeof(unsigned int) * numIndex);

	nVAO.AddAttribute(nVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	nVAO.AddAttribute(nVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	nVAO.AddAttribute(nVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vbo = nVBO;
	vao = nVAO;
}

void Mesh::SetVertexData(float* nVertexData, int numData) {
	FreeData();
	vertCount = numData;
	VAO nVAO;
	VBO nVBO(nVertexData, sizeof(float) * numData * 8);

	nVAO.AddAttribute(nVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	nVAO.AddAttribute(nVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	nVAO.AddAttribute(nVBO, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	vbo = nVBO;
	vao = nVAO;
}

void Mesh::SetDebugVertexData(float* nVertexData, int numData) {
	FreeData();
	vertCount = numData;
	VAO nVAO;
	VBO nVBO(nVertexData, ((sizeof(float)* 3) + sizeof(unsigned int))*numData);

	nVAO.AddAttribute(nVBO, 0, 3, GL_FLOAT, sizeof(unsigned int) + 3 * sizeof(float), (void*)0);
	nVAO.AddAttribute(nVBO, 1, 4, GL_UNSIGNED_BYTE, sizeof(unsigned int) + 3 * sizeof(float), (void*)(3 * sizeof(float)));

	vbo = nVBO;
	vao = nVAO;
}


void Mesh::SetVertexElements(unsigned int* vertIndexes, int numIndex) {
	elementCount = numIndex;
	vao.Bind();
	EBO nEBO(vertIndexes, sizeof(unsigned int) * numIndex);
	nEBO.Bind();
}

//isElements specifies if using glDrawElements instead of arrays. 
void Mesh::Render(Camera* camera, Shader* shader,bool isElements = true,unsigned int primative = GL_TRIANGLES) {

	BindMaterial(shader);

	glm::mat4 view = camera->GetView();
	glm::mat4 projection = camera->GetProjection();

	//Set camera position
	shader->SetUniform("cameraPos", camera->position);

	//Set view and projection matricies
	shader->SetUniform("view", view);
	shader->SetUniform("projection", projection);

	vao.Bind();

	if (instanceCount == 1) {
		if (isElements) {
			
			glDrawElements(primative, elementCount, GL_UNSIGNED_INT, 0);
		}
		else {
			
			glDrawArrays(primative, 0, vertCount);
		}
	}
	else {
		glDrawElementsInstanced(primative, elementCount, GL_UNSIGNED_INT, 0, instanceCount);
	}
	
	shader->Use();
	vao.UnBind();
}

void Mesh::FreeData() {
	vbo.Delete();
	ivbo.Delete();
	ebo.Delete();
	vao.Delete();
}

void Mesh::FreeEBO() {
	ebo.Delete();
}

