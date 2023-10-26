#include "DebugDrawables.h"

GaemGizmo::Line::Line()
{
}

GaemGizmo::Line::Line(std::vector<glm::vec3>& positions, glm::vec3 position)
{
	_position = position;
	SetLine(positions);
}

GaemGizmo::Line::~Line()
{
}

void GaemGizmo::Line::SetLine(std::vector<glm::vec3>& positions)
{
	if (positions.size() <= 0)
		return;

	_vao.Delete();

	_vertCount = positions.size();

	VAO nVAO;
	VBO nVBO(&positions[0].x, sizeof(float) * _vertCount * 3);

	nVAO.AddAttribute(nVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	_vao = nVAO;
}

void GaemGizmo::Line::Render(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
	if (_vertCount <= 0) return;

	shader->Use();

	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, _position);


	//Set view and projection matricies
	shader->SetUniform("view", view);
	shader->SetUniform("projection", projection);
	shader->SetUniform("model", modelMat);
	shader->SetUniform("color", _color);
	
	_vao.Bind();

	glDrawArrays(GL_LINE_STRIP,0, _vertCount);

	_vao.UnBind();
}

void GaemGizmo::DebugGizmo::SetColor(glm::vec4 color)
{
	_color = color;
}

void GaemGizmo::DebugGizmo::SetPosition(glm::vec3 position)
{
	_position = position;
}

GaemGizmo::Box::Box()
{
}

GaemGizmo::Box::Box(glm::vec3 position, glm::vec3 scale)
{
}

GaemGizmo::Box::~Box()
{
}

void GaemGizmo::Box::SetScale(glm::vec3 scale)
{
}

void GaemGizmo::Box::Render(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
}
