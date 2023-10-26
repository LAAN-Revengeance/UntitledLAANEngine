#include "PathNode.h"

GaemPathing::PathNode::PathNode()
{

}

GaemPathing::PathNode::PathNode(glm::vec3 positon)
{
	_position = positon;
}

void GaemPathing::PathNode::AddNeighbour(PathNode* neighbour)
{
	float distance = glm::length(neighbour->_position - _position);
	_neighbours.insert({ neighbour,distance });
}

void GaemPathing::PathNode::RemoveNeighbour(PathNode* neighbour)
{
	_neighbours.erase(neighbour);
}

void GaemPathing::PathNode::ClearNeighbours()
{
	_neighbours.clear();
}
