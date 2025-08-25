#include "gameTexts.h"
#include "utils.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

extern Text ochkiText;
extern Text nic;
extern Text gameWinText;
extern Text gameOverText;
extern Text lifesText;
extern Text textLeaderboard;


//--------------------------------------------------------------------------------------
void initTexts()
{
    ochkiText.setString(L"Очки 0");
    ochkiText.setCharacterSize(30); //Размер текста
    ochkiText.setStyle(Text::Bold);//Стиль текста
    ochkiText.setFillColor(Color::Blue);//Цвет текста
    ochkiText.setPosition(convertColStrToPosition(13, 1));

    nic.setString(L"Ник ");
    nic.setCharacterSize(30); //Размер текста
    nic.setStyle(Text::Bold);//Стиль текста
    nic.setFillColor(Color::Red);//Цвет текста
    nic.setPosition(sf::Vector2f(250.0f, 200.0f));

    gameWinText.setString(L"Подзравляю, Вы выиграли Игру!");
    gameWinText.setCharacterSize(40); //Размер текста
    gameWinText.setStyle(Text::Bold);//Стиль текста
    gameWinText.setFillColor(Color::Blue);//Цвет текста
    gameWinText.setPosition(sf::Vector2f(50.0f - 10, 200.0f - 60));

    gameOverText.setString(L"Ты лузер, проиграл!");
    gameOverText.setCharacterSize(60); //Размер текста
    gameOverText.setStyle(Text::Bold);//Стиль текста
    gameOverText.setFillColor(Color::Red);//Цвет текста
    gameOverText.setPosition(sf::Vector2f(120.0f - 60, 200.0f - 60));

    lifesText.setString(L":Жизни 3");
    lifesText.setCharacterSize(30); //Размер текста
    lifesText.setStyle(Text::Bold);//Стиль текста
    lifesText.setFillColor(Color::Blue);//Цвет текста
    lifesText.setPosition(convertColStrToPosition(13, 3));

    textLeaderboard.setString("");
    textLeaderboard.setCharacterSize(20); //Размер текста
    textLeaderboard.setStyle(Text::Bold);//Стиль текста
    textLeaderboard.setFillColor(Color::Red);//Цвет текста
    textLeaderboard.setPosition(sf::Vector2f(120.0f - 60, 300.0f - 60));
}
//--------------------------------------------------------------------------------------