#pragma once
#include "Base.h"


//------------------------------------------------------------------------------------------------------------------------------------------
class Player : public Base
{
public:
    Player();

    void shoot() override;

    void move(Direction& direction);
    void resetPosition();
    void setLife(int l);
    void livesReduce();

    Vector2f getPosition();

    bool isHitted(const FloatRect& bulletRect);
    int getLife();     

    Sprite player;

private:
    int life = 3;
};
//-------------------------------------------------------------------------------------------------------