#include "DebugDrawables.h"

GaemGizmo::Line::Line()
{
}

GaemGizmo::Line::Line(std::vector<glm::vec3>& positions)
{
	SetLine(positions);
}

GaemGizmo::Line::~Line()
{
}

void GaemGizmo::Line::SetLine(std::vector<glm::vec3>& positions)
{
	if (positions.size() <= 0)
		return;

	_lineSize = positions.size();

	VAO nVAO;
	VBO nVBO(&positions[0].x, sizeof(float) * _lineSize * 3);

	nVAO.AddAttribute(nVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	_vao = nVAO;
}

void GaemGizmo::Line::Render(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
	if (_lineSize <= 0) return;

	shader->Use();
	//Set view and projection matricies
	shader->SetUniform("view", view);
	shader->SetUniform("projection", projection);
	
	_vao.Bind();

	glDrawArrays(GL_LINES,0, _lineSize);

	_vao.UnBind();
}
