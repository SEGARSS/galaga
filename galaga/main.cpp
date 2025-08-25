#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "utils.h"
#include "levels.h"
#include "gameTexts.h"
#include "Player.h"
#include "Enemy.h"
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

class Enemy;

vector<Enemy> enemiesPlayer; // Создаём вектор врагов
vector<Sprite> bulletsEnemies; // Создаём вектор картинок (пулек)
vector <Sprite> bulletsPlayer; // Создаём вектор картинок (пулек).

vector<vector<Vector2f>> enemiesPositions;

int levelCount = 0;

const Font font("ARIAL.TTF");

Text ochkiText(font);
Text nic(font);
Text gameWinText(font);
Text gameOverText(font);
Text lifesText(font);
Text textLeaderboard(font);

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
void loadLeaderboard() 
{
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
//---------------------------------------------------------------------------------------------------------------
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
int main()
{
    initTexts();
    initLevels();
    loadLeaderboard();

    int level = 0;


    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 700 }), "SFML window");

    Player player;
    Direction direction = Direction::stop;

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

    // Start the game loop
    while (window.isOpen())
    {
        // Очистка окна.
        window.clear();

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
                        player.shoot();
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
                        //FloatRect enemyRect = enemiesPlayer[e].getGlobalBounds(); //получаем прямоугольник врага, в который вписывается спрайт врага
                        FloatRect bulletRect = bulletsPlayer[i].getGlobalBounds();//получаем прямоугольник пули, в который вписывается спрайт пули
                        //// Intersection - это на агл пересечение
                        //std::optional <FloatRect> intersect = bulletRect.findIntersection(enemyRect);
                        //if (intersect.has_value())
                        if (enemiesPlayer[e].isHitted(bulletRect))
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

                    //FloatRect playerRect = player.getGlobalBounds();
                    FloatRect bulletRect = bulletsEnemies[i].getGlobalBounds();

                    //std::optional <FloatRect> intersect = bulletRect.findIntersection(playerRect);
                    //if (intersect.has_value())
                    if (player.isHitted(bulletRect))
                    {
                        bulletsEnemies.erase(bulletsEnemies.begin() + i);
                        //player.getLife();
                        player.livesReduce();
                        cout << "Hit \n";
                        if (player.getLife() == 0)
                        {
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

            if (clockEnemyShoot.getElapsedTime() > Time(tickEnemyShoot)) //Начальное время 0 > 1500.
            {
                clockEnemyShoot.restart();
                if (!enemiesPlayer.empty())
                {
                    uniform_int_distribution<> distrib(0, enemiesPlayer.size() - 1);
                    enemiesPlayer[distrib(gen)].shoot();
                }
            }

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
                currentState = Game::State::GameEnd;
                addPlayerToLeaderboard();
            }

            //Движение врагов
            if (clockEnemyMovement.getElapsedTime() > Time(tickEnemyMovement))
            {
                clockEnemyMovement.restart();
                for (int i = 0; i < enemiesPlayer.size(); i++)
                {
                    enemiesPlayer[i].move();
                }
            }

            // Draw it
            //window.draw(player.getPlayerSprite());
            player.draw(window);

            //Рисуем вывод очков на доске
            ochkiText.setString(L"очки " + std::to_string(ochki));//Конвертируем int в string.(to_string)
            window.draw(ochkiText);

            lifesText.setString(L"Жизни " + std::to_string(player.getLife()));//Конвертируем int в string.(to_string)
            window.draw(lifesText);

            for (int i = 0; i < enemiesPlayer.size(); i++)
            {
                enemiesPlayer[i].draw(window);
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
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    {
                        level = -1;
                        enemiesPlayer.clear();
                        player.setLife(3);
                        ochki = 0;
                        player.resetPosition();
                        clock.restart();
                        clockEnemyShoot.restart();
                        clockEnemyMovement.restart();
                        bulletsEnemies.clear();
                        bulletsPlayer.clear();
                        currentState = Game::State::Play;
                    }
                }
            }

            if (player.getLife() > 0)
            {
                window.draw(gameWinText);
            }
            else
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