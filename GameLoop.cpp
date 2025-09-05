#include "GameLoop.h"

#include <print>

struct Position{	float x, y;};
struct Velocity{	float x, y;};
struct Circle //FOR OOP
{
	Position pos;
	Velocity vel;

	void Move() 
	{ pos.x += vel.x; }
};

std::vector<Circle*> circles;

void GameInit(sf::RenderWindow& window, entt::registry& registry)
{
	sf::Vector2u windowSize = window.getSize(); // width, height
	sf::Vector2f topLeft(0.f, 0.f);

	float maxHeight = static_cast<float>(windowSize.y);
	constexpr int ENTITY_SPAWN_COUNT = 100000;

	// OOP STYLE
	for (int i = 0; i < ENTITY_SPAWN_COUNT; i++)
	{
		Circle* circle = new Circle();
		circle->pos = Position(0.0f, 0.0f);
		circles.emplace_back(circle);
	}

	//ENTT STYLE
	for (int i = 0; i < ENTITY_SPAWN_COUNT; i++)
	{
		auto entity = registry.create();

		registry.emplace<Position>(entity, Position(0.0f, 0.0f));
		registry.emplace<Velocity>(entity, 0.f, 0.f);
	}
}

void SystemCirclesMover_ENTT(sf::RenderWindow& window, entt::registry& registry)
{
	//auto view = registry.view<Position, Velocity>();
	//for (auto [ent, pos, velocity] : view.each()) //SLOWER

	auto group = registry.group<Position, Velocity>();
	for (auto [ent, pos, velocity] : group.each())
	{
		pos.x += velocity.x;
	}
}

void SystemCirclesMover_OOP(std::vector<Circle*>& circles)
{
	for (auto& c : circles) 
	{
		c->Move();
	}
}


void GameMain(sf::RenderWindow& window, entt::registry& registry)
{
	SystemCirclesMover_ENTT(window, registry);
	//SystemCirclesMover_OOP(circles);
}

//Results:

//DEBUG With OOP 
//Elapsed time : 0.000069 seconds
//Elapsed time : 0.000020 seconds
//Elapsed time : 0.000029 seconds
//Elapsed time : 0.000019 seconds
// OOP FASTER

//DEBUG With ENTT (Group)
//Elapsed time : 0.003572 seconds
//Elapsed time : 0.003583 seconds
//Elapsed time : 0.003515 seconds
//Elapsed time : 0.003663 seconds
// ENTT SLOWER

//NOT DEBUG With OOP 
//Elapsed time : 0.000016 seconds
//Elapsed time : 0.000013 seconds
//Elapsed time : 0.000013 seconds
//Elapsed time : 0.000013 seconds
// OOP FASTER

//NOT DEBUG With ENTT
//Elapsed time : 0.001579 seconds
//Elapsed time : 0.001643 seconds
//Elapsed time : 0.001630 seconds
//Elapsed time : 0.001657 seconds
// ENTT SLOWER

//OOP 100.000 (Release)
//Elapsed time : 0.000268 seconds
//Elapsed time : 0.000305 seconds
//Elapsed time : 0.000188 seconds
//Elapsed time : 0.000244 seconds

//ENTT 100.000 (Release)
//Elapsed time : 0.017884 seconds
//Elapsed time : 0.018441 seconds
//Elapsed time : 0.018790 seconds
//Elapsed time : 0.018355 seconds
