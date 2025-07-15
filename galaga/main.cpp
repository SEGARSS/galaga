#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <chrono>
#include <random>

using namespace sf;
using namespace std;

Texture enemyTexture("enemy.png");
Texture bulletTexture("bullet.png");
Texture playerTexture("player.png");
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
//------------------------------------------------------------------------------------------------------------------------------------------
Vector2f convertColStrToPosition(int col, int str)
{
    Vector2f result;
    result.x = col * 50.f;
    result.y = str * 50.f;

    return result;
}
//------------------------------------------------------------------------------------------------------------------------------------------
Vector2f convertPosition(Vector2f pos)
{
    Vector2f position;
    position.x /= 50.f;
    position.y /= 50.f;

    return position;
}
//------------------------------------------------------------------------------------------------------------------------------------------
enum class Direction//перечисление, хранит варианты направления движения
{
    stop,

    left,
    right
};
//------------------------------------------------------------------------------------------------------------------------------------------
class Player
{
public:
    Player()
        : player(playerTexture)
    {
        player.setPosition(convertColStrToPosition(7, 12));
    };

    void shoot(vector<Sprite>& bulletsPlayer)
    {
        sf::Sprite newBullet(bulletTexture);//Создаём переменную newBullet которая будет только сдесь, и туда сразу помещаем текстуру пулек.
        Vector2f pos = player.getPosition();//Запрашиваем позицию игрока, чтобы пульки стрелял там где игрок и вылитали от него.
        //Ниже, попровляем корддиныты пули, чтобы по центру от игрока стреляли и выше него. (тоесть от его центра носа попровляем)
        pos.y -= 50.f; //Выше
        pos.x += 12.5f;//Правее                    
        newBullet.setPosition(pos);//После всех поправок кординат, ложим эти кординаты в нашу переменную newBullet (уже с готовыми поправленными кординатами)
        bulletsPlayer.push_back(newBullet);//И возвращем это в нашь вектор пуль, чтобы могли по пробелу стрелять ими.
    }

    void move(Direction& direction)
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

    Vector2f getPosition()
    {
        return player.getPosition();
    }

    Sprite& getPlayerSprite()
    {
        return player;
    }

private:
    Sprite player;
};
//-------------------------------------------------------------------------------------------------------

class Enemy
{
public:
    Enemy(Vector2f pos)
        : enemy(enemyTexture)
    {
        enemy.setPosition(pos);
    };

    FloatRect getGlobalBounds() {
        return enemy.getGlobalBounds();
    }

    Sprite& getEnemySprite()
    {
        return enemy;
    }

    void shoot(vector<Sprite>& bulletsEnemies) {
        sf::Sprite newBullet(bulletTexture);//Создаём переменную newBullet которая будет только сдесь, и туда сразу помещаем текстуру пулек.
        newBullet.setRotation(degrees(180.f));
        Vector2f pos = enemy.getPosition();//Запрашиваем позицию игрока, чтобы пульки стрелял там где игрок и вылитали от него.
        //Ниже, попровляем корддиныты пули, чтобы по центру от игрока стреляли и выше него. (тоесть от его центра носа попровляем)
        pos.y += 100.f; //Ниже
        pos.x += 37.5f;//Правее                    
        newBullet.setPosition(pos);//После всех поправок кординат, ложим эти кординаты в нашу переменную newBullet (уже с готовыми поправленными кординатами)
        bulletsEnemies.push_back(newBullet);//И возвращем это в нашь вектор пуль, чтобы могли по пробелу стрелять ими.
    }

private:
    Sprite enemy;

};
//-------------------------------------------------------------------------------------------------------
int main()
{
    int level = 0;

    vector<vector<Vector2f>> enemiesPositions;
    enemiesPositions.push_back(
        {
        convertColStrToPosition(5, 1),
        });

    enemiesPositions.push_back(
        {
        convertColStrToPosition(5, 1),
        convertColStrToPosition(6, 1),
        convertColStrToPosition(7, 1),
        });

    enemiesPositions.push_back(
        {
        convertColStrToPosition(2, 1),
        convertColStrToPosition(4, 2),
        convertColStrToPosition(6, 3),
        convertColStrToPosition(8, 3),
        convertColStrToPosition(10, 2),
        convertColStrToPosition(12, 1),
        });

    int levelCount = enemiesPositions.size() - 1;

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 700 }), "SFML window");

    Player player;
    vector <Sprite> bulletsPlayer; // Создаём вектор картинок (пулек)
    Direction direction = Direction::stop;

    vector<Enemy> enemiesPlayer; // Создаём вектор картинок (врагов)
    vector<Sprite> bulletsEnemies; // Создаём вектор картинок (пулек)

    for (int i = 0; i < enemiesPositions[level].size(); ++i)
    {
        Enemy enemy(enemiesPositions[level][i]);
        enemiesPlayer.push_back(enemy);
    }

    Clock clock; // Создаём часы (начало таймера)
    chrono::milliseconds tick(100); // После какого времени таймер начал делать всё сначала или сброс.

    Clock clockEnemyShoot; // Создаём часы (начало таймера)
    chrono::milliseconds tickEnemyShoot(1500); // После какого времени таймер начал делать всё сначала или сброс.

    random_device rd;
    mt19937 gen(rd());

    const Font font("ARIAL.TTF");

    Text text(font, L"Очки 0"); //L - чтоб были русские буквы вместо крякозябры.
    text.setCharacterSize(30); //Размер текста
    text.setStyle(Text::Bold);//Стиль текста
    text.setFillColor(Color::Blue);//Цвет текста
    text.setPosition(convertColStrToPosition(13, 1));

    bool gameWin = false;

    Text gameWinText(font, L"Подзравляю, Вы выиграли Игру!"); //L - чтоб были русские буквы вместо крякозябры.
    gameWinText.setCharacterSize(40); //Размер текста
    gameWinText.setStyle(Text::Bold);//Стиль текста
    gameWinText.setFillColor(Color::Blue);//Цвет текста
    gameWinText.setPosition(sf::Vector2f(50.0f - 10, 300.0f - 60));

    int ochki = 0;

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
                    player.shoot(bulletsPlayer);
                }
            }
        }

        player.move(direction);

        if (clock.getElapsedTime() > Time(tick)) //Начальное время 0 > 500.
        {
            clock.restart();// и как только это произошло (по условию стало больше 500, делаем рестарт.

            for (int i = 0; i < bulletsPlayer.size(); ) // Проходим по всему вектору пулек
            {
                Vector2f pos = bulletsPlayer[i].getPosition(); // Запрашиваем позицию пули.
                pos.y -= 50.f; //Назначаем ей направление, куда ей лететь.
                bulletsPlayer[i].setPosition(pos); // И уже ставим эту позию для пули.
                bool enemyDead = false;

                //пробегаемся по врагам и проверяем попала ли пуля в врага
                for (int e = 0; e < enemiesPlayer.size(); )
                {
                    FloatRect enemyRect = enemiesPlayer[e].getGlobalBounds(); //получаем прямоугольник врага, в который вписывается спрайт врага
                    FloatRect bulletRect = bulletsPlayer[i].getGlobalBounds();//получаем прямоугольник пули, в который вписывается спрайт пули
                    // Intersection - это на агл пересечение
                    std::optional <FloatRect> intersect = bulletRect.findIntersection(enemyRect);
                    if (intersect.has_value())
                    {
                        cout << "Hit \n";
                        // erase удаляет элемент из вектора по его позиции
                        // позицию получаем через begin() + e, где e - это индекс врага
                        enemiesPlayer.erase(enemiesPlayer.begin() + e);
                        bulletsPlayer.erase(bulletsPlayer.begin() + i);
                        ochki++;
                        enemyDead = true;
                        break;
                    }
                    else
                    {
                        ++e;
                    }
                }

                if (enemyDead == false)
                {
                    if (pos.y < 40)
                    {
                        bulletsPlayer.erase(bulletsPlayer.begin() + i);
                    }
                    else
                    {
                        ++i;
                    }
                }
            }

            for (int i = 0; i < bulletsEnemies.size(); ++i) {
                Vector2f pos = bulletsEnemies[i].getPosition(); // Запрашиваем позицию пули.
                pos.y += 50.f; //Назначаем ей направление, куда ей лететь.
                bulletsEnemies[i].setPosition(pos); // И уже ставим эту позию для пули.
            }
        }

        if (clockEnemyShoot.getElapsedTime() > Time(tickEnemyShoot)) //Начальное время 0 > 1500.
        {
            clockEnemyShoot.restart();
            if (!enemiesPlayer.empty()) {
                uniform_int_distribution<> distrib(0, enemiesPlayer.size() - 1);
                enemiesPlayer[distrib(gen)].shoot(bulletsEnemies);
            }
        }


        if (gameWin == false)
        {
            if (enemiesPlayer.empty() && level < levelCount)
            {
                ++level;
                for (int i = 0; i < enemiesPositions[level].size(); ++i)
                {
                    Enemy enemyTmp1(enemiesPositions[level][i]);
                    enemiesPlayer.push_back(enemyTmp1);
                }
            }
            if (enemiesPlayer.empty() && level == levelCount)
            {
                gameWin = true;
            }
        }


        // Clear screen
        window.clear();

        // Draw it
        window.draw(player.getPlayerSprite());

        //Рисуем вывод очков на доске
        text.setString(L"очки " + std::to_string(ochki));//Конвертируем int в string.(to_string)
        window.draw(text);

        for (int i = 0; i < enemiesPlayer.size(); i++)
        {
            window.draw(enemiesPlayer[i].getEnemySprite());
        }

        for (int i = 0; i < bulletsPlayer.size(); i++)
        {
            window.draw(bulletsPlayer[i]);
        }

        for (int i = 0; i < bulletsEnemies.size(); i++)
        {
            window.draw(bulletsEnemies[i]);
        }

        if (gameWin)
        {
            window.draw(gameWinText);
        }

        // Update the window
        window.display();
    }
}