#include "PathNode.h"

GaemPath::PathNode::PathNode()
{

}

GaemPath::PathNode::PathNode(glm::vec3 positon)
{
	_position = positon;
}

void GaemPath::PathNode::AddNeighbour(PathNode* neighbour)
{
	float distance = glm::length(neighbour->_position - _position);
	_neighbours.insert({ neighbour,distance });
}

void GaemPath::PathNode::RemoveNeighbour(PathNode* neighbour)
{
	_neighbours.erase(neighbour);
}

void GaemPath::PathNode::ClearNeighbours()
{
	_neighbours.clear();
}
