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
	//_vao.Delete();
}

void GaemGizmo::Line::SetLine(std::vector<glm::vec3>& positions)
{
	if (positions.empty()) {
		_vertCount = 0;
		return;
	}

	_vao.Delete();

	_vertCount = positions.size();

	VAO nVAO;
	VBO nVBO(&positions[0].x, sizeof(float) * _vertCount * 3);

	nVAO.AddAttribute(nVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);

	_vao = nVAO;
}

void GaemGizmo::Line::Render(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
	if (_vertCount <= 0 || !_enabled) return;
	glLineWidth(_width);

	shader->Use();

	glm::mat4 modelMat(1.0f);
	modelMat = glm::translate(modelMat, _position);
	modelMat = glm::scale(modelMat, {1,1,1});
	

	//Set view and projection matricies
	shader->SetUniform("view", view);
	shader->SetUniform("projection", projection);
	shader->SetUniform("model", modelMat);
	shader->SetUniform("color", _color);
	
	_vao.Bind();

	glDrawArrays(GL_LINE_STRIP,0, _vertCount);

	_vao.UnBind();
}

void GaemGizmo::Line::RenderFront(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
	bool depthTest = glIsEnabled(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);
	Render(projection,view,shader);

	if(depthTest)
		glEnable(GL_DEPTH_TEST);
}

void GaemGizmo::Line::SetWidth(float width)
{
	_width = width;
}

void GaemGizmo::DebugGizmo::SetColor(glm::vec4 color)
{
	_color = color;
}

void GaemGizmo::DebugGizmo::SetPosition(glm::vec3 position)
{
	_position = position;
}

void GaemGizmo::DebugGizmo::SetEnabled(bool enabled)
{
	_enabled = enabled;
}

VAO* GaemGizmo::Box::_vao = nullptr;

GaemGizmo::Box::Box()
{
	if (!_vao) {
        SetVertexBuffer();
	}
}

GaemGizmo::Box::Box(glm::vec3 position, glm::vec3 scale)
{
	_scale = scale;
	_position = position;
	if (!_vao) {
		SetVertexBuffer();
	}
}

GaemGizmo::Box::~Box()
{
}

void GaemGizmo::Box::SetScale(glm::vec3 scale)
{
	_scale = scale;
}

void GaemGizmo::Box::Render(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
	if (!_enabled) return;

    shader->Use();

    glm::mat4 modelMat(1.0f);
    modelMat = glm::translate(modelMat, _position);
    modelMat = glm::scale(modelMat, _scale);

    //Set view and projection matricies
    shader->SetUniform("view", view);
    shader->SetUniform("projection", projection);
    shader->SetUniform("model", modelMat);
    shader->SetUniform("color", _color);

    _vao->Bind();

	glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, 0);

    _vao->UnBind();
}

void GaemGizmo::Box::SetVertexBuffer()
{
	float vertices[] = {
		0.5,    0.5,    0.5,
	   -0.5f,   0.5,    0.5,
		0.5,    0.5,   -0.5f,
	   -0.5f,   0.5,   -0.5f,
		0.5,   -0.5f,   0.5,
	   -0.5f,  -0.5f,   0.5,
	   -0.5f,  -0.5f,  -0.5f,
		0.5,   -0.5f,  -0.5f
	};

	unsigned int elements[] = {
	3, 2, 6, 7, 4, 2, 0,
	3, 1, 6, 5, 4, 1, 0
	};

    _vao = new VAO();
    VBO nVBO(vertices, sizeof(vertices));
	EBO nEBO(elements, sizeof(elements));

    _vao->AddAttribute(nVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
}
