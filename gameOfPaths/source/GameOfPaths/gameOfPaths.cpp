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
#include "Helper.h"
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
	//_crtBreakAlloc = 647;
#endif
    //set the working directory to the directory where the executable exists, so that the relative paths work
    SetCurrentDirectory(Helper::ExecutableDirectory().c_str());

	// create the window	
	sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(Helper::ScreenDimensions.x), static_cast<unsigned int>(Helper::ScreenDimensions.y)), "Game of Paths");
	window.setFramerateLimit(60);
	auto getWindow = [&]()
	{
		return &window;
	};

	sf::Clock clock;
	Game game(getWindow, static_cast<unsigned int>(Helper::ScreenDimensions.x), static_cast<unsigned int>(Helper::ScreenDimensions.y));
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// clear the window with black color
		window.clear(sf::Color(100, 100,100));

		float deltaTime = clock.restart().asSeconds();

		game.Update(deltaTime);

        game.Draw();

		// end the current frame
		window.display();
	}

	return 0;
}