#include "Enemy.h"

Texture enemyTexture("enemy.png");
Texture bulletTexture("bullet.png");

using namespace sf;
using namespace std;

extern vector<Sprite> bulletsEnemies;

//--------------------------------------------------------------------------------------------
Enemy::Enemy(Vector2f pos)
: enemy(enemyTexture)
{
    enemy.setPosition(pos);
};
//--------------------------------------------------------------------------------------------
void Enemy::shoot()
{
    Sprite newBullet(bulletTexture);//Создаём переменную newBullet которая будет только сдесь, и туда сразу помещаем текстуру пулек.
    newBullet.setRotation(degrees(180.f));
    Vector2f pos = enemy.getPosition();//Запрашиваем позицию игрока, чтобы пульки стрелял там где игрок и вылитали от него.
    //Ниже, попровляем корддиныты пули, чтобы по центру от игрока стреляли и выше него. (тоесть от его центра носа попровляем)
    pos.y += 100.f; //Ниже
    pos.x += 37.5f;//Правее                    
    newBullet.setPosition(pos);//После всех поправок кординат, ложим эти кординаты в нашу переменную newBullet (уже с готовыми поправленными кординатами)
    bulletsEnemies.push_back(newBullet);//И возвращем это в нашь вектор пуль, чтобы могли по пробелу стрелять ими.
}
//--------------------------------------------------------------------------------------------
void Enemy::move()
{
    Vector2f pos = enemy.getPosition();
    pos.y += 50;
    enemy.setPosition(pos);
}
//--------------------------------------------------------------------------------------------

