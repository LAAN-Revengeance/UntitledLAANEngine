#include "A_Star.h"

std::vector<glm::vec3> GaemPathing::FindPathA_StarPositions(PathNode* start, PathNode* end, const std::vector<PathNode*>& nodes)
{

	std::stack<PathNode*> nodeStack = FindPathA_Star(start, end, nodes);
	std::vector<glm::vec3> pathVec;

	while (!nodeStack.empty()) {
		pathVec.push_back(nodeStack.top()->GetPosition());
		nodeStack.pop();
	}

	return pathVec;
}

std::stack<GaemPathing::PathNode*> GaemPathing::FindPathA_Star(PathNode* start, PathNode* end, const std::vector<PathNode*>& nodes)
{
	//create AS_Star nodes and fill out map
	std::map<PathNode*, AS_Node*> mapNode;
	for (auto& node : nodes)
	{
		AS_Node* nNode = new AS_Node(node, end);
		mapNode.insert({ node,nNode });
	}

	//start point
	AS_Node* nodeStart = mapNode.at(start);

	//end point
	AS_Node* nodeEnd = mapNode.at(end);

	//start conditions
	AS_Node* nodeCurrent = nodeStart;
	nodeStart->global = nodeStart->lineOfSight;
	nodeStart->local = 0.0f;

	std::list<AS_Node*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	//search node graph
	while (!listNotTestedNodes.empty())
	{
		//sort list
		listNotTestedNodes.sort([](const AS_Node* lhs, const AS_Node* rhs) {return lhs->global < rhs->global; });

		AS_Node* currentNode = listNotTestedNodes.front();

		currentNode->visited = true;
		for (auto& pNeighbour : currentNode->node->GetNeighbours()) {

			AS_Node* aNeighbour = mapNode.at(pNeighbour.first);

			//check if current node is shortest path to this node
			float possibleNewLocal = currentNode->local + pNeighbour.second;
			if (aNeighbour->local > possibleNewLocal && !aNeighbour->node->GetObstacle()) {
				aNeighbour->parent = currentNode;
				aNeighbour->local = possibleNewLocal;
				aNeighbour->global = possibleNewLocal + aNeighbour->lineOfSight;
				listNotTestedNodes.push_back(aNeighbour);
			}
		}

		//remove current node from list
		listNotTestedNodes.pop_front();
	}

	//assemble a stack of nodes
	std::stack<PathNode*> PathStack;
	AS_Node* p = nodeEnd;
	while (p->parent)
	{
		PathStack.push(p->node);
		p = p->parent;
	}
	PathStack.push(start);
	return PathStack;
}
