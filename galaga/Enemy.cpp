#include "Enemy.h"

Texture enemyTexture("enemy.png");
extern Texture bulletTexture;

using namespace sf;
using namespace std;

extern vector<Sprite> bulletsEnemies;

//--------------------------------------------------------------------------------------------
Enemy::Enemy(Vector2f pos)
: Base(enemyTexture, pos)
{

};
//--------------------------------------------------------------------------------------------
void Enemy::shoot()
{
    Sprite newBullet(bulletTexture);//������ ���������� newBullet ������� ����� ������ �����, � ���� ����� �������� �������� �����.
    newBullet.setRotation(degrees(180.f));
    Vector2f pos = bodu_.getPosition();//����������� ������� ������, ����� ������ ������� ��� ��� ����� � �������� �� ����.
    //����, ���������� ���������� ����, ����� �� ������ �� ������ �������� � ���� ����. (������ �� ��� ������ ���� ����������)
    pos.y += 100.f; //����
    pos.x += 37.5f;//������                    
    newBullet.setPosition(pos);//����� ���� �������� ��������, ����� ��� ��������� � ���� ���������� newBullet (��� � �������� ������������� �����������)
    bulletsEnemies.push_back(newBullet);//� ��������� ��� � ���� ������ ����, ����� ����� �� ������� �������� ���.
}
//--------------------------------------------------------------------------------------------
void Enemy::move()
{
    Vector2f pos = bodu_.getPosition();
    pos.y += 50;
    bodu_.setPosition(pos);
}
//--------------------------------------------------------------------------------------------

