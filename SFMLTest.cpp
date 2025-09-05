#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "src/commons/PerformanceAnalyze.h"
#include "GameLoop.h"


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML 2 Window");
	entt::registry registry;

	GameInit(window, registry);

	LARGE_INTEGER freq;
	LARGE_INTEGER start;
	LARGE_INTEGER end;

	QueryPerformanceFrequency(&freq);
	// after you spawn entities:

	while (window.isOpen())
	{
		window.clear();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		QueryPerformanceCounter(&start);
		GameMain(window, registry);
		QueryPerformanceCounter(&end);
		double elapsed = static_cast<double>(end.QuadPart - start.QuadPart) / freq.QuadPart;
		printf("Elapsed time: %f seconds\n", elapsed);

		window.display();
	}

	return 0;
}
