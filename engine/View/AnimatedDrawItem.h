#pragma once
#include "DrawItem.h"


class AnimatedDrawItem : public DrawItem
{
public:

    //Set an animation cycle and its name
    virtual void SetAnimation(const std::string& animName, int start, int end, float speed) = 0;

    //animate a specific animation cycle
    virtual void Animate(const std::string& animation) = 0;

    //set how fast animations are played
    virtual  void SetAnimationSpeed(float speed) = 0;

private:

};