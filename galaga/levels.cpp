#include "levels.h"

#include "utils.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

extern std::vector<std::vector<Vector2f>> enemiesPositions;
extern int levelCount;


//----------------------------------------------------------------------------------------------
void initLevels()
{
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

    levelCount = enemiesPositions.size() - 1;

}
//----------------------------------------------------------------------------------------------