#pragma once
#include <glm/glm.hpp>
#include <Debug/DebugDrawables.h>
#include <map>

namespace GaemPathing{

    /**
    *   @class PathNode
    *   @brief Represents a node in a pathfinding system.
    *
    *   This class provides functionalities to represent a node the pathfinding system.
    * 
    *   @author Andres Comeros-Ochtman
	*   @version 1.0
	*   @date 26/10/2023
    */
    class PathNode
    {
    public:
        /**
        *   @brief default constructor.
        */
        PathNode();

        /**
        *   @brief constructor with initial position.
        *   @param position Initial position of the node.
        */
        PathNode(glm::vec3 position);

        /**
        *   @brief Set the position of the node.
        *   @param position New position to set.
        */
        void SetPosition(glm::vec3 position);

        /**
        *   @brief set if the node is an obstacle.
        *   @param obstacle true if node is an obstacle, false if not.
        */
        void SetObstacle(bool obstacle);

        /**
        *   @brief get the location of the node.
        *   @return position of the node.
        */
        glm::vec3 GetPosition();

        /**
        *   @brief check if the node is an obstacle.
        *   @return true if node is an obstacle, false if not.
        */
        bool GetObstacle();

        /**
        *   @brief add a neighboring node.
        *   @param neighbour pointer to the neighboring node.
        */
        void AddNeighbour(PathNode* neighbour);

        /**
        *   @brief add a neighboring node with specified cost.
        *   @param neighbour pointer to the neighboring node.
        *   @param cost cost of connection to the neighboring node.
        */
        void AddNeighbour(PathNode* neighbour, float cost);

        /**
        *   @brief remove a neighboring node.
        *   @param neighbour Pointer to the neighboring node to be removed.
        */
        void RemoveNeighbour(PathNode* neighbour);

        /**
        *   @brief clear all neighboring nodes.
        */
        void ClearNeighbours();

        /**
        *   @brief get all neighboring nodes and their connection costs.
        *   @return map containing neighboring nodes and their respective connection costs.
        */
        const std::map<PathNode*, float>& GetNeighbours();

        /**
        *   @brief draw the node using the provided projection, view matrices and shader.
        *   @param projection projection matrix.
        *   @param view view matrix.
        *   @param shader pointer to the shader.
        */
        void Draw(glm::mat4 projection, glm::mat4 view, Shader* shader);

        /**
        *   @brief reset neighbours to be nodes that are at a set distance
        *   @param nodes list of nodes to check for connections.
        *   @param distance maximum distance to check for connections.
        */
        void UpdateConnections(std::vector<PathNode*> nodes, float distance);

        /**
        *   @brief Update cached distances and delete null neighbours
        */
        void UpdateConnections();

        /**
        *   @brief update debug gizmos related to this node
        */
        void UpdateModel();

        /**
        *   @brief get the unique identifier of the node.
        *   @return the unique identifier of the node.
        */
        unsigned int GetID();

        /**
        *   @brief set the unique identifier of the node.
        *   @param id The unique identifier to set.
        */
        void SetID(unsigned int id);

    private:

        glm::vec3 _position = { 0,0,0 };
        bool _obstacle = false;

        ///stores pointer to adjacent node and the cost of conection
        std::map<PathNode*, float> _neighbours;

        ///debug rendering
        std::map<PathNode*, GaemGizmo::Line*> lines;
        GaemGizmo::Box box;

        ///node ID, used as refernce for serialization
        unsigned int ID = 0;

        friend class PathNodeManager;

    protected:
        glm::vec3 _box_Scale = { 0.5f, 0.5f, 0.5f };
        glm::vec4 _color_Obstacle = { 0.0f, 0.0f, 0.2f, 1.0f };
        glm::vec4 _color_Active = { 0.0f, 0.0f, 1.0f, 1.0f };
        glm::vec4 _color_connection = { 0.0f, 1.0f, 0.3f, 1.0f };
    };
}