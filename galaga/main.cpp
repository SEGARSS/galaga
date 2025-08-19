#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

#include <string>

using namespace sf;
using namespace std;

Texture enemyTexture("enemy.png");
Texture bulletTexture("bullet.png");
Texture playerTexture("player.png");

int ochki = 0;

std::string playerName;

//------------------------------------------------------------------------------------------------------------------------------------------
namespace Game
{
    enum class State
    {
        NameInput,
        Play,
        GameEnd,
    };
};
//------------------------------------------------------------------------------------------------------------------------------------------
Game::State currentState = Game::State::NameInput;
//------------------------------------------------------------------------------------------------------------------------------------------
struct LeaderboardEntry
{
    std::string name;
    int score = 0;

    bool operator<(LeaderboardEntry other) const
    {
        return score < other.score;
    }
    //  bool operator>(LeaderboardEntry other) const
    //  {
      //	return score > other.score;
    //  }
    //  bool operator==(LeaderboardEntry other) const
    //  {
    //      return score == other.score;
    //  }
};
//------------------------------------------------------------------------------------------------------------------------------------------
std::vector<LeaderboardEntry> leaderboard;
//------------------------------------------------------------------------------------------------------------------------------------------
void loadLeaderboard() {
    std::string filename = "leaderboard.txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        file.close();
        return;
    }

    string name;
    int score;

    while (file >> name >> score) {
        LeaderboardEntry entry;
        entry.name = name;
        entry.score = score;
        leaderboard.push_back(entry);
    }
    file.close();
}

void saveLeaderboard() 
{
    std::string filename = "leaderboard.txt";
    std::ofstream file(filename);

    for (int i = 0; i < leaderboard.size(); ++i) 
    {
        file << leaderboard[i].name << " " << leaderboard[i].score << "\n";
    }
    file.close();
}
//---------------------------------------------------------------------------------------------------------------
void addPlayerToLeaderboard() 
{
    LeaderboardEntry entry;
    entry.name = playerName;
    entry.score = ochki;
    leaderboard.push_back(entry);

    std::sort(leaderboard.begin(), leaderboard.end());
    saveLeaderboard();
}
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

    void resetPosition()
    {
        player.setPosition(convertColStrToPosition(7, 12));
    }

    Sprite& getPlayerSprite()
    {
        return player;
    }

    int getLife()
    {
        return life;
    }

    void setLife(int l)
    {
        life = l;
    }

    void livesReduce()
    {
        life --;
    }

private:
    Sprite player;
    int life = 3;
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

    void move()
    {
        Vector2f pos = enemy.getPosition();
        pos.y += 50;
        enemy.setPosition(pos);
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

    Clock clockEnemyMovement; // Создаём часы (начало таймера)
    chrono::milliseconds tickEnemyMovement(3000); // После какого времени таймер начал делать всё сначала или сброс.

    random_device rd;
    mt19937 gen(rd());

    const Font font("ARIAL.TTF");

    Text text(font, L"Очки 0"); //L - чтоб были русские буквы вместо крякозябры.
    text.setCharacterSize(30); //Размер текста
    text.setStyle(Text::Bold);//Стиль текста
    text.setFillColor(Color::Blue);//Цвет текста
    text.setPosition(convertColStrToPosition(13, 1));

    Text nic(font, L"Ник "); //L - чтоб были русские буквы вместо крякозябры.
    nic.setCharacterSize(30); //Размер текста
    nic.setStyle(Text::Bold);//Стиль текста
    nic.setFillColor(Color::Red);//Цвет текста
    nic.setPosition(sf::Vector2f(250.0f, 200.0f));

    Text gameWinText(font, L"Подзравляю, Вы выиграли Игру!"); //L - чтоб были русские буквы вместо крякозябры.
    gameWinText.setCharacterSize(40); //Размер текста
    gameWinText.setStyle(Text::Bold);//Стиль текста
    gameWinText.setFillColor(Color::Blue);//Цвет текста
    gameWinText.setPosition(sf::Vector2f(50.0f - 10, 200.0f - 60));

    Text gameOverText(font, L"Ты лузер, проиграл!"); //L - чтоб были русские буквы вместо крякозябры.
    gameOverText.setCharacterSize(60); //Размер текста
    gameOverText.setStyle(Text::Bold);//Стиль текста
    gameOverText.setFillColor(Color::Red);//Цвет текста
    gameOverText.setPosition(sf::Vector2f(120.0f - 60, 200.0f - 60));

    Text lifesText(font, L":Жизни 3"); //L - чтоб были русские буквы вместо крякозябры.
    lifesText.setCharacterSize(30); //Размер текста
    lifesText.setStyle(Text::Bold);//Стиль текста
    lifesText.setFillColor(Color::Blue);//Цвет текста
    lifesText.setPosition(convertColStrToPosition(13, 3));

    bool gameWin = false;
    bool gameOver = false;    

    Text textLeaderboard(font, ""); //L - чтоб были русские буквы вместо крякозябры.
    textLeaderboard.setCharacterSize(20); //Размер текста
    textLeaderboard.setStyle(Text::Bold);//Стиль текста
    textLeaderboard.setFillColor(Color::Red);//Цвет текста
    textLeaderboard.setPosition(sf::Vector2f(120.0f - 60, 300.0f - 60));

    Text textEntered(font, "Имя"); //L - чтоб были русские буквы вместо крякозябры.
    textEntered.setCharacterSize(60); //Размер текста
    textEntered.setStyle(Text::Bold);//Стиль текста
    textEntered.setFillColor(Color::Red);//Цвет текста
    textEntered.setPosition(sf::Vector2f(120.0f - 60, 300.0f - 60));    

    // Start the game loop
    while (window.isOpen())
    {
        switch (currentState)
        {
        case Game::State::NameInput:
        {
            // События процесса
            while (const std::optional event = window.pollEvent())
            {
                // Закрыть окно: выход
                if (event->is<sf::Event::Closed>())
                    window.close();

                //Ввод текста
                if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
                {
                    if (textEntered->unicode < 128)
                    {
                        if (textEntered->unicode == '\b' && playerName != "")
                        {
                            playerName.pop_back();
                        }
                        else if (textEntered->unicode >= 33)
                        {
                            playerName += static_cast<char>(textEntered->unicode);
                        }
                    }

                    std::cout << playerName << std::endl;
                }
            }

            // Очистка окна.
            window.clear();

            nic.setString((playerName));
            window.draw(nic);

            // Очистка окна.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                currentState = Game::State::Play;
                nic.setPosition(sf::Vector2f(600.0f, 0.0f));
            }

            break;
        }
        case Game::State::Play:
        {
            // Process events
            while (const std::optional event = window.pollEvent())
            {
                // Close window: exit
                if (event->is<sf::Event::Closed>())
                    window.close();

                if (event->is<Event::KeyPressed>())
                {
                    if (gameOver == false)
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

                //Попала ли пуля в игрока
                for (int i = 0; i < bulletsEnemies.size(); )
                {
                    Vector2f pos = bulletsEnemies[i].getPosition(); // Запрашиваем позицию пули.
                    pos.y += 50.f; //Назначаем ей направление, куда ей лететь.
                    bulletsEnemies[i].setPosition(pos); // И уже ставим эту позию для пули.

                    FloatRect playerRect = player.getPlayerSprite().getGlobalBounds();
                    FloatRect bulletRect = bulletsEnemies[i].getGlobalBounds();

                    std::optional <FloatRect> intersect = bulletRect.findIntersection(playerRect);
                    if (intersect.has_value())
                    {
                        bulletsEnemies.erase(bulletsEnemies.begin() + i);
                        //player.getLife();
                        player.livesReduce();
                        cout << "Hit \n";
                        if (player.getLife() == 0)
                        {
                            gameOver = true;
                            currentState = Game::State::GameEnd;
                            addPlayerToLeaderboard();
                        }
                    }
                    else
                    {
                        ++i;
                    }
                }
            }

            if (gameOver == false)
            {
                if (clockEnemyShoot.getElapsedTime() > Time(tickEnemyShoot)) //Начальное время 0 > 1500.
                {
                    clockEnemyShoot.restart();
                    if (!enemiesPlayer.empty())
                    {
                        uniform_int_distribution<> distrib(0, enemiesPlayer.size() - 1);
                        enemiesPlayer[distrib(gen)].shoot(bulletsEnemies);
                    }
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
                    currentState = Game::State::GameEnd;
                    addPlayerToLeaderboard();
                }
            }

            //Движение врагов
            if (gameOver == false)
            {
                if (clockEnemyMovement.getElapsedTime() > Time(tickEnemyMovement))
                {
                    clockEnemyMovement.restart();
                    for (int i = 0; i < enemiesPlayer.size(); i++)
                    {
                        enemiesPlayer[i].move();
                    }
                }
            }

            // Clear screen
            window.clear();

            // Draw it
            window.draw(player.getPlayerSprite());

            //Рисуем вывод очков на доске
            text.setString(L"очки " + std::to_string(ochki));//Конвертируем int в string.(to_string)
            window.draw(text);

            lifesText.setString(L"Жизни " + std::to_string(player.getLife()));//Конвертируем int в string.(to_string)
            window.draw(lifesText);

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

            nic.setString(L"Ник " + (playerName));
            window.draw(nic);            

            break;
        }
        case Game::State::GameEnd:
        {
            while (const std::optional event = window.pollEvent())
            {
                // Close window: exit
                if (event->is<sf::Event::Closed>())
                    window.close();


                if (event->is<Event::KeyPressed>())
                {
                    if (gameOver == false)
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
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    {
                        if (gameOver == true || gameWin == true)
                        {
                            level = -1;
                            enemiesPlayer.clear();
                            player.setLife(3);
                            ochki = 0;
                            player.resetPosition();
                            gameOver = false;
                            gameWin = false;
                            clock.restart();
                            clockEnemyShoot.restart();
                            clockEnemyMovement.restart();
                            bulletsEnemies.clear();
                            bulletsPlayer.clear();
                            currentState = Game::State::Play;
                        }
                    }
                }
            }

            // Очистка окна.
            window.clear();

            if (gameWin)
            {
                window.draw(gameWinText);
            }

            if (gameOver)
            {
                window.draw(gameOverText);
            }

            std::string leaderboardText = "Leaderboard:\n";
            for (int i = leaderboard.size() - 1; i >= 0; --i) {
                leaderboardText += leaderboard[i].name + " " + std::to_string(leaderboard[i].score) + "\n";
            }
            textLeaderboard.setString(leaderboardText);
            window.draw(textLeaderboard);

            break;
        }
        default:
            break;
        }
        

        // Update the window
        window.display();
    }
}