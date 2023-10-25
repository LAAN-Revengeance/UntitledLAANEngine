#pragma once
#include "RigidBody.h"
#include <Utils/GaemUtils.h>

/**
 *  @struct ContactData
 *  @brief Stores information about a point of contact during a collision.
 */
struct ContactPointData {

        /// Position of the contact point relative to body 1.
    glm::vec3 localPositionB1;

        /// Position of the contact point relative to body 2.
    glm::vec3 localPositionB2; 

        /// Position of the contact point in world coordinates.
    glm::vec3 contactWorldSpace;

        /// Normal vector at the contact point, pointing away from body 1 towards body 2.
    glm::vec3 contactNormal;

        /// Depth of penetration at the contact point. A positive value indicates overlapping.
    float penetrationDepth;
};

/**
 *  @struct CollisionData
 *  @brief Holds information about a collision between two bodies.
 */
struct CollisionData {

        /// Pointer to the first physics body involved in the collision.
    PhysicsBody* b1;

        /// Pointer to the second physics body involved in the collision.
    PhysicsBody* b2; 

        /// Number of contact points in the collision.
    unsigned int numContacts = 0; 

        /// Array of contact points. Currently, a maximum of 8 contact points is supported.
    ContactPointData contacts[8]; 
};

/**
*   @class CollisionSolver
*   @brief Responsible for resolving collisions detected between physics bodies.
* 
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 1/10/2023
*/
class CollisionSolver
{
public:

    /**
    *   @brief Resolves all aspects of the collisions described in the CollisionData.
    *   @param collisionData Reference to the CollisionData containing the details of the collision.
    */
    static void ResolveCollisions(CollisionData& collisionData);

    /**
    *   @brief Resolves penetrations between bodies in a collision by adjusting their positions linearly.
    *   @param collisionData Reference to the CollisionData containing the details of the collision.
    */
    static void ResolvePenetrationLinear(CollisionData& collisionData);

    /**
    *   @brief Calculates and applies impulse forces to the bodies in a collision.
    *   @param collisionData Reference to the CollisionData containing the details of the collision.
    */
    static void ResolveImpulse(CollisionData& collisionData);

private:

    /**
    *   @brief Utility function to print tensor information for debugging.
    *   @param cd The CollisionData from which to extract and print tensor information.
    */
    static void PrintTensors(CollisionData& cd);
};