#include "GameLoop.h"

#include "src/commons/Randomizer.h"
#include "src/main/Systems.h"

#include <print>

bool pressedLastFrame = false;



void GameInit(sf::RenderWindow& window, entt::registry& registry)
{
	sf::Vector2u windowSize = window.getSize(); // width, height

	sf::Vector2f topLeft(0.f, 0.f);
	sf::Vector2f topRight(static_cast<float>(windowSize.x), 0.f);
	sf::Vector2f bottomLeft(0.f, static_cast<float>(windowSize.y));
	sf::Vector2f bottomRight(static_cast<float>(windowSize.x), static_cast<float>(windowSize.y));

	constexpr int ENTITY_SPAWN_COUNT = 10000;
	for (int i = 0; i < ENTITY_SPAWN_COUNT; i++)
	{
		// Create a simple entity with a position and a shape
		auto entity = registry.create();

		//auto& circle = registry.emplace<sf::CircleShape>(entity, radius); //dont need this big data. create this on render only
		float radius = 10.0f;
		RenderData renderDat = RenderData(radius, GetRandomColorSF());
		registry.emplace<RenderData>(entity, renderDat);

		float maxHeight = static_cast<float>(windowSize.y);
		registry.emplace<Position>
			(entity, topLeft.x, std::fmod(topLeft.y + (i * 0.1f), maxHeight)); //looping the y position back to 0

		registry.emplace<Velocity>(entity, 0.f, 0.f);
	}

	auto group = registry.group<entt::entity>();
	printf("Entity count : %zu \n", group.size());
}

void GameMain(sf::RenderWindow& window, entt::registry& registry)
{
	bool isKeyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	if (isKeyDown && !pressedLastFrame)
	{
		float force = 3.f;
		for (auto [ent, vel, renderDat] : registry.view<Velocity, RenderData>().each())
		{
			int randomTest = rand() % 100;
			float randomF = randomTest * 0.1f;
			//int entityId = static_cast<uint32_t>(ent);
			vel.x += force + randomF;
		}
	}
	pressedLastFrame = isKeyDown;

	//SystemClickCircle(window, registry);
	SystemCircleLooper(window, registry);
	SystemRandomizeColor(window, registry);
	SystemVelocity(window, registry);
	SystemCirclesMover(window, registry);
	SystemRender(window, registry);
}

//Result:
// With Circle (With View, not Group)
//Elapsed time : 0.032607 seconds
//Elapsed time : 0.031879 seconds
//Elapsed time : 0.031519 seconds


//Without Circle (With view, not Group)
//Elapsed time: 0.032731 seconds
//Elapsed time : 0.032812 seconds
//Elapsed time : 0.032683 seconds
//Elapsed time : 0.033412 seconds
//Elapsed time : 0.033078 seconds