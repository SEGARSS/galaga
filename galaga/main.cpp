#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <chrono>

using namespace sf;
using namespace std;

/*
* дз
* добавить врага
* Попал ли по врагу или нет ?
* Если попал, то вывести надпись про это
*/

/*
* поле 16 колонок на 14 строк (1 клетка 50 на 50)
* игрок
* управление
* стрелять игрока
* - вектор с ракетами, которые есть на экране сейчас
* - таймер, по которому будем двигать наши ракеты
* - добавить событие - выстрел (по кнопке пробел)
* - удалять ракеты при выходе из зоны видимости экрана
* враг 1 и научить ее умирать
* добавить счетчик очков
* генерация врагов
* движение врагов на игрока
* добавить жизни игрока
*/
//--------------------------------------------------------------------------
Vector2f getPosition(int col, int str)
{
    Vector2f result;
    result.x = col * 50.f;
    result.y = str * 50.f;

    return result;
}
//--------------------------------------------------------------------------
Vector2f convertPosition(Vector2f pos)
{
    Vector2f position;
    position.x /= 50.f;
    position.y /= 50.f;

    return position;
}
//--------------------------------------------------------------------------
enum class Direction//перечисление, хранит варианты направления движения
{
    stop,

    left,
    right
};
//-------------------------------------------------------------------------------------------------------
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 700 }), "SFML window");

    // Load a texture
    const sf::Texture playerTexture("player.png");
    const sf::Texture bulletTexture("bullet.png");
    const sf::Texture enemyTexture("enemy.png");

    // Create a sprite
    sf::Sprite player(playerTexture);
    player.setPosition(getPosition(7, 12));

    sf::Sprite enemy(enemyTexture);
    enemy.setPosition(getPosition(7,1));

    vector <Sprite> bulletsPlayer; // Создаём вектор картинок (пулек)

    Clock clock; // Создаём часы (начало таймера)
    chrono::milliseconds tick(500); // После какого времени таймер начал делать всё сначала или сброс.

    Direction direction = Direction::stop;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<Event::KeyPressed>())
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                {
                    direction = Direction::right;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                {
                    direction = Direction::left;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    /*
                    Создаём переменную newBullet которая будет только сдесь, и туда сразу помещаем текстуру пулек.
              
                    */
                    sf::Sprite newBullet(bulletTexture);//Создаём переменную newBullet которая будет только сдесь, и туда сразу помещаем текстуру пулек.
                    Vector2f pos = player.getPosition();//Запрашиваем позицию игрока, чтобы пульки стрелял там где игрок и вылитали от него.
                    //Ниже, попровляем корддиныты пули, чтобы по центру от игрока стреляли и выше него. (тоесть от его центра носа попровляем)
                    pos.y -= 50.f; //Выше
                    pos.x += 12.5f;//Правее                    
                    newBullet.setPosition(pos);//После всех поправок кординат, ложим эти кординаты в нашу переменную newBullet (уже с готовыми поправленными кординатами)
                    bulletsPlayer.push_back(newBullet);//И возвращем это в нашь вектор пуль, чтобы могли по пробелу стрелять ими.
                }
            }
        }

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

        if (clock.getElapsedTime() > Time(tick)) //Начальное время 0 > 500.
        {
            clock.restart();// и как только это произошло (по условию стало больше 500, делаем рестарт.

            for (int i = 0; i < bulletsPlayer.size(); i++) // Проходим по всему вектору пулек
            {
                Vector2f pos = bulletsPlayer[i].getPosition(); // Запрашиваем позицию пули.
                pos.y -= 50.f; //Назначаем ей направление, куда ей лететь.
                bulletsPlayer[i].setPosition(pos); // И уже ставим эту позию для пули.

                Vector2f posEnemy = enemy.getPosition();
                if (pos.y == posEnemy.y || pos.x <= posEnemy.x)
                {
                    cout << "Hit! \n";
                }
                else if (pos.y == posEnemy.y || pos.x != posEnemy.x);
            }            
        }

        //for (int i = 0; i < bulletsPlayer.size(); i++)
        //{
        //    Vector2f posBullets = bulletsPlayer[i].getPosition();
        //    Vector2f posEnemy = enemy.getPosition();
        //    if (posBullets.y == posEnemy.y)
        //    {
        //        cout << "Hit! \n";
        //    }
        //}

        // Clear screen
        window.clear();

        // Draw it
        window.draw(player);
        window.draw(enemy);

        for (int i = 0; i < bulletsPlayer.size(); i++)
        {
            window.draw(bulletsPlayer[i]);
        }

        // Update the window
        window.display();
    }
}