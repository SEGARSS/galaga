#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "utils.h"

#include <vector>

using namespace std;
using namespace sf;

//------------------------------------------------------------------
class Base
{
public:
	Base(Texture& texture, Vector2f position);

	void draw(RenderWindow& window);
	FloatRect getGlobalBounds();
	bool isHitted(const FloatRect& bulletRect);

	virtual void shoot() = 0;

	Sprite bodu_;

private:	
};