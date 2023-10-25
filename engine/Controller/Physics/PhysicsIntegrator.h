#pragma once
#include "RigidBody.h"

/**
*	@Class PhysicsIntegrator
*	@brief Contains functions responsible for updating physics bodies positions overtime
*   and applies forces.
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 1/10/2023
*/
class PhysicsIntegrator
{
public:

    /**
    *   @brief Integrates the physical state of a PhysicsBody over a time step.
    *   
    *   @param pb Reference to the PhysicsBody to integrate.
    *   @param deltaTime time step to intergrate over
    *   @return void
    */
    static void Integrate(PhysicsBody& pb, float deltaTime);
    
        /// global gravity value
    static glm::vec3 gravity;
private:

    static void IntergrateLinear(PhysicsBody& pb, float deltaTime);
    static void IntergrateRotational(PhysicsBody& pb, float deltaTime);
    PhysicsIntegrator();
    ~PhysicsIntegrator();
};