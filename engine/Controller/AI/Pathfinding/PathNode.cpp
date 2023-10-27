#include "PathNode.h"

GaemPathing::PathNode::PathNode()
{
	UpdateModel();
}

GaemPathing::PathNode::PathNode(glm::vec3 positon)
{
	SetPosition(positon);
	UpdateModel();
}

void GaemPathing::PathNode::SetPosition(glm::vec3 position)
{
	_position = position;
	UpdateModel();
}

void GaemPathing::PathNode::SetObstacle(bool obstacle)
{
	_obstacle = obstacle;

	if (_obstacle) {
		box.SetColor({0.3,0.3,0.3,1.0});
	}
	else {
		box.SetColor({0.0,0.0,1.0,1.0});
	}
}

glm::vec3 GaemPathing::PathNode::GetPosition()
{
	return _position;
}

bool GaemPathing::PathNode::GetObstacle()
{
	return _obstacle;
}

const std::map<GaemPathing::PathNode*, float>& GaemPathing::PathNode::GetNeighbours()
{
	return _neighbours;
}

void GaemPathing::PathNode::Draw(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
	box.Render(projection, view,shader);
	for (auto& line : lines)
		line.second->Render(projection, view, shader);
}

void GaemPathing::PathNode::UpdateModel()
{
	box.SetScale({ 2,2,2 });
	if (_obstacle) {
		box.SetColor({ 0.1,0.1,0.1,1.0 });
	}
	else {
		box.SetColor({ 0.0,0.0,1.0,1.0 });
	}

	box.SetPosition(_position);

	for (auto& line : lines)
		delete line.second;

	lines.clear();

	for (auto& neighbour : _neighbours) {
		std::vector<glm::vec3> pos({_position,neighbour.first->_position});

		GaemGizmo::Line* nLine = new GaemGizmo::Line(pos, { 0,0,0 });
		nLine->SetColor({ 0.3,0.3,0.3,1 });
		lines.insert({ neighbour.first , nLine });
	}
}

void GaemPathing::PathNode::AddNeighbour(PathNode* neighbour)
{
	float distance = glm::length(neighbour->_position - _position);
	_neighbours.insert({ neighbour,distance });
	UpdateModel();
}

void GaemPathing::PathNode::AddNeighbour(PathNode* neighbour, float cost)
{
	_neighbours.insert({ neighbour,cost });
	UpdateModel();
}

void GaemPathing::PathNode::RemoveNeighbour(PathNode* neighbour)
{
	_neighbours.erase(neighbour);
	UpdateModel();
}

void GaemPathing::PathNode::ClearNeighbours()
{
	_neighbours.clear();
	UpdateModel();
}
