#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;


/*
* поле 16 колонок на 14 строк (1 клетка 50 на 50)
* игрок
* управление
* стрелять игрока
* враг 1 и научить ее умирать
* добавить счетчик очков
* генерация врагов
* движение врагов на игрока
* добавить жизни игрока
*/
/**/
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
    const sf::Texture texture("igrok.png");

    // Create a sprite
    sf::Sprite igrok(texture);
    igrok.setPosition(getPosition(7, 12));

    Direction direction = Direction::stop;

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
            }
        }


        if (direction != Direction::stop)
        {
            Vector2f pos = igrok.getPosition();

            if (direction == Direction::left)
            {
                pos.x -= 50.f;
            }
            if (direction == Direction::right)
            {
                pos.x += 50.f;
            }

            igrok.setPosition(pos);
            direction = Direction::stop;
        }

        // Clear screen
        window.clear();

        // Draw it
        window.draw(igrok);

        // Update the window
        window.display();
    }
}