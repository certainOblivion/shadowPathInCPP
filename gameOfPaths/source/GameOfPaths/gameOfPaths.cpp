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
#include "Game.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


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
	// create the window	
	const int windowWidth = 1900;
	const int windowHeight = 1200;
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Game of Paths");
	auto getWindow = [&]()
	{
		return &window;
	};

	sf::Clock clock;
	Game game(getWindow, windowWidth,windowHeight);
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
		window.clear(sf::Color(136, 136,136));

		float deltaTime = clock.restart().asSeconds();

		game.Update(deltaTime);

		// end the current frame
		window.display();
	}

	return 0;
}

