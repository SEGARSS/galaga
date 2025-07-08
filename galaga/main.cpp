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

    vector <Sprite> bulletsPlayer; // Создаём вектор картинок (пулек)
    vector <Sprite> enemiesPlayer; // Создаём вектор картинок (врагов)

    sf::Sprite enemyTmp1(enemyTexture);
    sf::Sprite enemyTmp2(enemyTexture);
    sf::Sprite enemyTmp3(enemyTexture);
    enemyTmp1.setPosition(getPosition(5, 1));
    enemyTmp2.setPosition(getPosition(7, 1));
    enemyTmp3.setPosition(getPosition(9, 1));
    enemiesPlayer.push_back(enemyTmp1);
    enemiesPlayer.push_back(enemyTmp2);
    enemiesPlayer.push_back(enemyTmp3);

    Clock clock; // Создаём часы (начало таймера)
    chrono::milliseconds tick(500); // После какого времени таймер начал делать всё сначала или сброс.

    Direction direction = Direction::stop;

    const Font font("ARIAL.TTF");

    Text text(font, L"Очки 0"); //L - чтоб были русские буквы вместо крякозябры.
    text.setCharacterSize(30); //Размер текста
    text.setStyle(Text::Bold);//Стиль текста
    text.setFillColor(Color::Blue);//Цвет текста
    text.setPosition(getPosition(13, 1));

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

        //Уничтожение пулек если они коснулись края(верхнего)
        for (int i = 0; i < bulletsPlayer.size(); )
        {
            Vector2f posSt = bulletsPlayer[i].getPosition();
            if (posSt.y < 40)
            {
                bulletsPlayer.erase(bulletsPlayer.begin() + i);
            }
            else
            {
                ++i;
            }
        }

        if (clock.getElapsedTime() > Time(tick)) //Начальное время 0 > 500.
        {
            clock.restart();// и как только это произошло (по условию стало больше 500, делаем рестарт.

            for (int i = 0; i < bulletsPlayer.size(); ++i) // Проходим по всему вектору пулек
            {
                Vector2f pos = bulletsPlayer[i].getPosition(); // Запрашиваем позицию пули.
                pos.y -= 50.f; //Назначаем ей направление, куда ей лететь.
                bulletsPlayer[i].setPosition(pos); // И уже ставим эту позию для пули.

                //пробегаемся по врагам и проверяем попала ли пуля в врага
                for (int e = 0; e < enemiesPlayer.size(); )
                {
                    /*
                    почему столкновение проверяем через пересечение прямоугольников?
                    а не через радиусы, как в агарио
                    - потому что у нас не круглые картинки, и круг плохо описывает картинку
                    почему не через сравнение координат?
                    можно же было сделать setOrigin, чтобы Origin был в центре картинки
                    и тогда pos == enemy.getPosition() сработал бы отлично
                    - потому что нам нужно учитывать размеры и частичное пересечение,
                      когда пуля попадает в картинку врага, но не в центр

                    |------|
                    |      |
                    |     ^|   - вот это тоже пересечение
                    |-----||
                          |
                    */               

                    
                    FloatRect enemyRect = enemiesPlayer[e].getGlobalBounds(); //получаем прямоугольник врага, в который вписывается спрайт врага
                    FloatRect bulletRect = bulletsPlayer[i].getGlobalBounds();//получаем прямоугольник пули, в который вписывается спрайт пули
                    //проверяем пересекаются ли эти два прямоугольника
                    // findIntersection возвращает std::optional, который может быть пустым, если пересечения нет
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
                    }
                    else
                    {
                        // поскольку erase удаляет элемент, мы можем пропустить проверку следующего врага
                        // поэтому увеличиваем индекс e только если не удалили врага
                        // например
                        //    индексы 0  1  2  3
                        //          [ 1, 2, 3, 4 ]
                        // удалили врага с индексом 1
                        //                      0  1  2
                        // теперь вектор стал [ 1, 3, 4 ]
                        // если при этом индекс увеличить на 1, то мы перейдем к элементу с индексом 2 ( значение 4)
                        // а элемент с индексом 1 (значение 3) пропустим
                        ++e;

                        /*
                        поэтому важно, если в цикле надо удалять элементы из вектора,
                        то из for надо убирать ++i, иначе будем пропускать элементы
                        т е вместо for (int i = 0; i < v.size(); i++)
                        пишем      for (int i = 0; i < v.size(); )
                        а ++i ставим в тела цикла, после всех проверок и удалений, там где это надо
                        */
                    }
                }
                
            }            
        }

        // Clear screen
        window.clear();

        // Draw it
        window.draw(player);

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

        //window.draw(text);

        // Update the window
        window.display();
    }
}