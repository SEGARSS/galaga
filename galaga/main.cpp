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
    : bulletTexture("bullet.png"), playerTexture("player.png"), player(playerTexture)  
    {
        player.setPosition(convertColStrToPosition(7, 12));
    };

    void shoot(vector<Sprite> &bulletsPlayer) 
    {
        sf::Sprite newBullet(bulletTexture);//Создаём переменную newBullet которая будет только сдесь, и туда сразу помещаем текстуру пулек.
        Vector2f pos = player.getPosition();//Запрашиваем позицию игрока, чтобы пульки стрелял там где игрок и вылитали от него.
        //Ниже, попровляем корддиныты пули, чтобы по центру от игрока стреляли и выше него. (тоесть от его центра носа попровляем)
        pos.y -= 50.f; //Выше
        pos.x += 12.5f;//Правее                    
        newBullet.setPosition(pos);//После всех поправок кординат, ложим эти кординаты в нашу переменную newBullet (уже с готовыми поправленными кординатами)
        bulletsPlayer.push_back(newBullet);//И возвращем это в нашь вектор пуль, чтобы могли по пробелу стрелять ими.
    }

    void move(Direction &direction)
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

    Sprite &getPlayerSprite()
    {
        return player;
    }

private:
   Texture bulletTexture;
   Texture playerTexture;
   Sprite player;
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

    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 700 }), "SFML window");

    // Load a texture
    const sf::Texture enemyTexture("enemy.png");

    Player player;
    vector <Sprite> bulletsPlayer; // Создаём вектор картинок (пулек)
    Direction direction = Direction::stop;

    vector <Sprite> enemiesPlayer; // Создаём вектор картинок (врагов)
    for (int i = 0; i < enemiesPositions[level].size(); ++i) 
    {
        Sprite enemyTmp1(enemyTexture);
        enemyTmp1.setPosition(enemiesPositions[level][i]);
        enemiesPlayer.push_back(enemyTmp1);
    }

    Clock clock; // Создаём часы (начало таймера)
    chrono::milliseconds tick(500); // После какого времени таймер начал делать всё сначала или сброс.


    const Font font("ARIAL.TTF");

    Text text(font, L"Очки 0"); //L - чтоб были русские буквы вместо крякозябры.
    text.setCharacterSize(30); //Размер текста
    text.setStyle(Text::Bold);//Стиль текста
    text.setFillColor(Color::Blue);//Цвет текста
    text.setPosition(convertColStrToPosition(13, 1));

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
        }

        if (enemiesPlayer.empty()) 
        {
            ++level;
            for (int i = 0; i < enemiesPositions[level].size(); ++i) 
            {
                Sprite enemyTmp1(enemyTexture);
                enemyTmp1.setPosition(enemiesPositions[level][i]);
                enemiesPlayer.push_back(enemyTmp1);
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
            window.draw(enemiesPlayer[i]);
        }

        for (int i = 0; i < bulletsPlayer.size(); i++)
        {
            window.draw(bulletsPlayer[i]);
        }

        // Update the window
        window.display();
    }
}