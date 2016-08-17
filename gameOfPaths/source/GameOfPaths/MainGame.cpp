////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "pch.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "PathObstacle.h"
#include <list>
#include <functional>
#include <stdlib.h>
#include <crtdbg.h>
#include "GameHelper.h"
#include "Game.h"

#define _CRTDBG_MAP_ALLOC
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

int main()
{
#if defined(DEBUG) || defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc = 277;
#endif
    //set the working directory to the directory where the executable exists, so that the relative paths work
    SetCurrentDirectory(GameHelper::ExecutableDirectory().c_str());

    // create the window	
    sf::Vector2f viewCenter(960.f, 540.f);
    sf::Vector2f viewSize(1920, 1080);
    sf::View view(viewCenter, viewSize);
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game of Paths");
    window.setView(view);
    auto getWindow = [&]()
    {
        return &window;
    };

    sf::Clock clock;
    Game game(getWindow);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // clear the window with black color
        window.clear(sf::Color(100, 100, 100));

        float deltaTime = clock.restart().asSeconds();

        game.Update(deltaTime);

        game.Draw();

        // end the current frame
        window.display();

        end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(std::chrono::microseconds(16000) - (end - start));

    }

    return 0;
}