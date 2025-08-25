#include "Base.h"

//--------------------------------------------------------------------------------------------
Base::Base(Texture& texture, Vector2f position)
    :texture(texture), Sprite(bodu_)
{

}
//--------------------------------------------------------------------------------------------
void Base::draw(RenderWindow& window)
{
    window.draw(bodu_);
}
//--------------------------------------------------------------------------------------------
FloatRect Base::getGlobalBounds()
{
    return bodu_.getGlobalBounds();
}
//--------------------------------------------------------------------------------------------
bool Base::isHitted(const FloatRect& bulletRect)
{
    FloatRect enemyRect = bodu_.getGlobalBounds();
    std::optional <FloatRect> intersect = bulletRect.findIntersection(enemyRect);
    return intersect.has_value();
}
//--------------------------------------------------------------------------------------------