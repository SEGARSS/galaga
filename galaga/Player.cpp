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
    sf::Sprite newBullet(bulletTexture);//������ ���������� newBullet ������� ����� ������ �����, � ���� ����� �������� �������� �����.
    Vector2f pos = player.getPosition();//����������� ������� ������, ����� ������ ������� ��� ��� ����� � �������� �� ����.
    //����, ���������� ���������� ����, ����� �� ������ �� ������ �������� � ���� ����. (������ �� ��� ������ ���� ����������)
    pos.y -= 50.f; //����
    pos.x += 12.5f;//������                    
    newBullet.setPosition(pos);//����� ���� �������� ��������, ����� ��� ��������� � ���� ���������� newBullet (��� � �������� ������������� �����������)
    bulletsPlayer.push_back(newBullet);//� ��������� ��� � ���� ������ ����, ����� ����� �� ������� �������� ���.
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
