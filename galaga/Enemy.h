#pragma once

#include "Base.h"

class Enemy : public Base
{
public:
    Enemy(Vector2f pos);   
    
    void shoot() override;
    void move(); 

    Sprite enemy;

private:
};
//-------------------------------------------------------------------------------------------------------
