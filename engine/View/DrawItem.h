#pragma once
#pragma once
#include "Camera.h"
#include <iostream>
#include "OBJReader.h"
#include <lighting.h>
#include <Material.h>

struct animation {
    animation() :
        start(0),
        end(0),
        speed(0.0f)
    {}
    animation(int st, int en, float sp) : 
        start(st),
        end(en),
        speed(sp)
    {}

    int start;
    int end;
    float speed;
};

/**
*	@Class DrawItem
*	@brief Shared interface for items that can be drawn using the renderer
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 20/04/2023
*/
class DrawItem {

public:
        /*
        *   @brief Render to the current buffer
        *   @param view perspective matrix
        *   @param projection matrix
        *   @param shader shader to use to draw object
        *   @param isElements if using element buffer
        *   @param primative draw primative used to render
        *   @return void
        */
    virtual  void Render(glm::mat4 projection, glm::mat4 view, Shader* shader, bool isElements, unsigned int primative);

        /*
        *   @brief Generic update method for drawitems that need to change visuals in relation to time
        *   @param deltaTime time since last frame
        *   @return void
        */
    virtual void Update(float deltaTime);

        //Set an animation cycle and its name
    virtual void SetAnimation(const std::string& animName, int start, int end, float speed);

        //animate a specific animation cycle
    virtual void Animate(const std::string& animation);

        //set how fast animations are played
    virtual  void SetAnimationSpeed(float speed);

    float maxBounds = 0.0f;

    bool visible = true;

    std::string name;
protected:
};