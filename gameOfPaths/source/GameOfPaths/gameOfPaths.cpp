////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "pch.h"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include "../Library/PathObstacle.h"
#include <list>
#include <functional>
#include "Game.h"


void DrawObstacle(Vector2D position, Vector2D location, float rotation)
{
 	sf::CircleShape shape(50);
 	shape.setPosition((float)position.x, (float)position.y);
 	// set the shape color to green
 	shape.setFillColor(sf::Color::Green);


}
////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

int main()
{
	// create the window	
	sf::RenderWindow window(sf::VideoMode(800, 600), "Game of Paths");
	auto getWindow = [&]()
	{
		return &window;
	};
	sf::Clock clock;

	Game game(getWindow);
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

