#include <SFML/Graphics.hpp>
#include "Game.h"
using namespace sf;

int main()
{
    RenderWindow window(VideoMode(1920, 1080), "Space Shooter", Style::Default);
    Game game(&window);
    while (window.isOpen())
    {
        game.update();
        game.draw();
    }
    return 0;
}