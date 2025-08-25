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
    ochkiText.setString(L"���� 0");
    ochkiText.setCharacterSize(30); //������ ������
    ochkiText.setStyle(Text::Bold);//����� ������
    ochkiText.setFillColor(Color::Blue);//���� ������
    ochkiText.setPosition(convertColStrToPosition(13, 1));

    nic.setString(L"��� ");
    nic.setCharacterSize(30); //������ ������
    nic.setStyle(Text::Bold);//����� ������
    nic.setFillColor(Color::Red);//���� ������
    nic.setPosition(sf::Vector2f(250.0f, 200.0f));

    gameWinText.setString(L"����������, �� �������� ����!");
    gameWinText.setCharacterSize(40); //������ ������
    gameWinText.setStyle(Text::Bold);//����� ������
    gameWinText.setFillColor(Color::Blue);//���� ������
    gameWinText.setPosition(sf::Vector2f(50.0f - 10, 200.0f - 60));

    gameOverText.setString(L"�� �����, ��������!");
    gameOverText.setCharacterSize(60); //������ ������
    gameOverText.setStyle(Text::Bold);//����� ������
    gameOverText.setFillColor(Color::Red);//���� ������
    gameOverText.setPosition(sf::Vector2f(120.0f - 60, 200.0f - 60));

    lifesText.setString(L":����� 3");
    lifesText.setCharacterSize(30); //������ ������
    lifesText.setStyle(Text::Bold);//����� ������
    lifesText.setFillColor(Color::Blue);//���� ������
    lifesText.setPosition(convertColStrToPosition(13, 3));

    textLeaderboard.setString("");
    textLeaderboard.setCharacterSize(20); //������ ������
    textLeaderboard.setStyle(Text::Bold);//����� ������
    textLeaderboard.setFillColor(Color::Red);//���� ������
    textLeaderboard.setPosition(sf::Vector2f(120.0f - 60, 300.0f - 60));
}
//--------------------------------------------------------------------------------------