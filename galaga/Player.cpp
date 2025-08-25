#include "Player.h"


using namespace sf;
using namespace std;

extern Texture playerTexture("player.png");
extern Texture bulletTexture("bullet.png");

extern vector <Sprite> bulletsPlayer;

//--------------------------------------------------------------------------------------------------------------------------------
Player::Player()
    : player(playerTexture)
{
    player.setPosition(convertColStrToPosition(7, 12));
};
//--------------------------------------------------------------------------------------------------------------------------------
void Player::shoot()
{
    sf::Sprite newBullet(bulletTexture);//Создаём переменную newBullet которая будет только сдесь, и туда сразу помещаем текстуру пулек.
    Vector2f pos = player.getPosition();//Запрашиваем позицию игрока, чтобы пульки стрелял там где игрок и вылитали от него.
    //Ниже, попровляем корддиныты пули, чтобы по центру от игрока стреляли и выше него. (тоесть от его центра носа попровляем)
    pos.y -= 50.f; //Выше
    pos.x += 12.5f;//Правее                    
    newBullet.setPosition(pos);//После всех поправок кординат, ложим эти кординаты в нашу переменную newBullet (уже с готовыми поправленными кординатами)
    bulletsPlayer.push_back(newBullet);//И возвращем это в нашь вектор пуль, чтобы могли по пробелу стрелять ими.
}
//--------------------------------------------------------------------------------------------------------------------------------
void Player::move(Direction& direction)
{
    if (direction != Direction::stop)
    {
        Vector2f pos = player.getPosition();

        if (direction == Direction::left && pos.x > 50)
        {
            pos.x -= 50.f;
        }
        if (direction == Direction::right && pos.x < 600)
        {
            pos.x += 50.f;
        }

        player.setPosition(pos);
        direction = Direction::stop;
    }
}
//--------------------------------------------------------------------------------------------------------------------------------
void Player::resetPosition()
{
    player.setPosition(convertColStrToPosition(7, 12));
}
//--------------------------------------------------------------------------------------------------------------------------------
void Player::setLife(int l)
{
    life = l;
}
//--------------------------------------------------------------------------------------------------------------------------------
void Player::livesReduce()
{
    life--;
}
//--------------------------------------------------------------------------------------------------------------------------------
Vector2f Player::getPosition()
{
    return player.getPosition();
}
//--------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------
bool Player::isHitted(const FloatRect& bulletRect)
{
    FloatRect playerRect = player.getGlobalBounds();
    std::optional <FloatRect> intersect = bulletRect.findIntersection(playerRect);
    return intersect.has_value();
}
//--------------------------------------------------------------------------------------------------------------------------------
int Player::getLife()
{
    return life;
}
//--------------------------------------------------------------------------------------------------------------------------------
