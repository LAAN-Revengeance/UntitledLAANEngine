#include "PathNodeManager.h"

GaemPathing::PathNodeManager::PathNodeManager()
{
}

GaemPathing::PathNodeManager::~PathNodeManager()
{
}

GaemPathing::PathNode* GaemPathing::PathNodeManager::AddNode(glm::vec3 position, bool obstacle)
{
    PathNode* nNode = new PathNode(position);
    nNode->SetObstacle(obstacle);
    for (auto& node : _nodes) {
        
        float dist = glm::distance(position, node->GetPosition());

        if  (dist <= _maxConnectionDist) 
        {
            nNode->AddNeighbour(node, dist);
            node->AddNeighbour(nNode, dist);
        }
    }

    _nodes.push_back(nNode);
    return nNode;
}

void GaemPathing::PathNodeManager::DeleteNode(PathNode* node)
{
    if (!node)
        return;
  
    //get copy of neighbours
    std::map<PathNode*, float> neighbours = node->GetNeighbours();

    //remove references to deleted node
    for (auto& neighbour : neighbours) {
        neighbour.first->RemoveNeighbour(node);
    }

    _nodes.erase(std::remove(_nodes.begin(), _nodes.end(), node), _nodes.end());

    delete node;
    node = nullptr;

    //update deleted nodes neighbours
    for (auto& neighbour : neighbours) {
        if (neighbour.first != nullptr)
            neighbour.first->UpdateConnections();
    }
}

void GaemPathing::PathNodeManager::DrawDebug(glm::mat4 projection, glm::mat4 view, Shader* shader)
{
    for (auto& node : _nodes)
    {
        node->Draw(projection,view,shader);
    }
}

float GaemPathing::PathNodeManager::GetMaxConnectionDist()
{
    return _maxConnectionDist;
}

void GaemPathing::PathNodeManager::SetMaxConnectionDist(float distance)
{
    if (distance < 0)
        distance = -distance;

    _maxConnectionDist = distance;
}

void GaemPathing::PathNodeManager::UpdateNodes()
{
    for (auto& node: _nodes)
    {
        node->UpdateConnections(_nodes, _maxConnectionDist);
    }
}

std::vector<GaemPathing::PathNode*>& GaemPathing::PathNodeManager::GetNodes()
{
    return _nodes;
}
