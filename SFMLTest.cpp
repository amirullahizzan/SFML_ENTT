#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <print>

struct Velocity
{
	float x, y;
};

void SystemCirclesMover(sf::RenderWindow& window, entt::registry& registry)
{
	registry.view<sf::CircleShape, sf::Vector2f, Velocity>().
		each([](auto& circle, auto& pos, auto& velocity)
			{
				pos.x += velocity.x;
			});
}

void SystemVelocity(sf::RenderWindow& window, entt::registry& registry)
{
	constexpr float friction = 0.93f;
	//SystemVelocity
	registry.view<Velocity>().
		each([friction](auto& velocity)
			{
				velocity.x *= friction;
				velocity.y *= friction;
			});
}

void SystemRender(sf::RenderWindow& window, entt::registry& registry)
{
	registry.view<sf::CircleShape, sf::Vector2f>().
		each([&window](auto& circle, auto& position)
			{
				circle.setPosition(position);
				window.draw(circle);
			});
}

void GameInit(sf::RenderWindow& window, entt::registry& registry)
{
	sf::Vector2u windowSize = window.getSize(); // width, height

	sf::Vector2f topLeft(0.f, 0.f);
	sf::Vector2f topRight(static_cast<float>(windowSize.x), 0.f);
	sf::Vector2f bottomLeft(0.f, static_cast<float>(windowSize.y));
	sf::Vector2f bottomRight(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

	for (int i = 0; i < 20000;i++)
	{
		// Create a simple entity with a position and a shape
		auto entity = registry.create();
		auto& circle = registry.emplace<sf::CircleShape>(entity, 10.f);
		circle.setFillColor(sf::Color::Green);

		float maxHeight = static_cast<float>(windowSize.y);
		registry.emplace<sf::Vector2f>
			(entity, topLeft.x, std::fmod(topLeft.y + (i * 0.1f), maxHeight)); //looping the y position to 0

		registry.emplace<Velocity>(entity, 0.f, 0.f);
	}

	auto view = registry.view<entt::entity>();
	std::cout << "Entity count : " << view.size() << "\n";
}

bool pressedLastFrame = false;

void GameMain(sf::RenderWindow& window, entt::registry& registry)
{
	bool isKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	if (isKeyDown && !pressedLastFrame)
	{
		float force = 3.f;
		for (auto [ent, circle, vel] : registry.view< sf::CircleShape, Velocity>().each())
		{
			int randomTest = rand() % 100;
			//int entityId = static_cast<uint32_t>(ent);
			vel.x += force + randomTest;
		}

	}

	pressedLastFrame = isKeyDown;

	SystemVelocity(window, registry);
	SystemCirclesMover(window, registry);
	SystemRender(window, registry);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML 2 Window");
	entt::registry registry;

	GameInit(window, registry);

	while (window.isOpen())
	{
		window.clear();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


		GameMain(window, registry);

		window.display();
	}


	return 0;
}
