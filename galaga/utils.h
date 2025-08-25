#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

sf::Vector2f convertColStrToPosition(int col, int str);
sf::Vector2f convertPosition(sf::Vector2f pos);


//-------------------------------------------------------------------------
enum class Direction//перечисление, хранит варианты направления движения
{
    stop,

    left,
    right
};
//-------------------------------------------------------------------------
