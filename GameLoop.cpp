#include "GameLoop.h"

#include <print>

struct Position { float x, y; };
struct Velocity { float x, y; };
struct CircleData { float radius; };


struct Object //FOR OOP
{

};

struct Component
{
	Component() = default;
	virtual ~Component() = default;
};

struct PositionComp : public Component
{
	PositionComp(float x, float y) : x(x), y(y) {};
	float x, y;

	void Move(float velX)
	{
		x += velX;
	}


};

struct VelocityComp : public Component
{
	VelocityComp(float x, float y) : x(x), y(y) {};
	float x, y;
	void AddForce(float x, float y)
	{
		x += x;
		y += y;
	}
	void ApplyFriction()
	{
		constexpr float friction = 0.98f;
		x *= friction;
		y *= friction;
	}
};

struct Circle : public Object //FOR OOP
{
	Circle()
	{
		VelocityComp* initialVelComp = new VelocityComp(3.0f, 0.0f);

		components.emplace_back(new PositionComp{ 0.0f,0.0f });
		components.emplace_back(initialVelComp);
	};

	std::vector<Component*> components; //oop

	template <typename T>
	T* GetComponent()
	{
		for (auto* component : components)
		{
			T* cast = dynamic_cast<T*>(component);
			if (cast) return cast;
		}
		return nullptr;
	}

	bool CheckNeighbors(float thisRadius, const std::vector<Circle*>& circles)
	{
		const auto* thisPosition = GetComponent<PositionComp>();
		for (const auto& circle : circles)
		{
			if (circle != this)
			{
				const auto* position = circle->GetComponent<PositionComp>();
				float dx = thisPosition->x - position->x;
				float dy = thisPosition->y - position->y;
				float distance = std::sqrt(dx * dx + dy * dy);

				if (distance < (thisRadius + circle->radius))
				{
					return true;
				}
			}
		}

		return false;
	}


	bool isHit = false;
	float radius = 5.0f;

};

std::vector<Circle*> circles; //oop

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
		circles.emplace_back(circle);
	}

	Velocity initialVel = Velocity(3.0f, 0.0f);
	//ENTT STYLE
	for (int i = 0; i < ENTITY_SPAWN_COUNT; i++)
	{
		auto entity = registry.create();

		registry.emplace<Position>(entity, Position(0.0f, 0.0f));
		registry.emplace<Velocity>(entity, initialVel);
		registry.emplace<CircleData>(entity, 10.0f);
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

	auto view = registry.view<Velocity>();
	constexpr float friction = 0.98f;
	for (auto [ent, velocity] : view.each())
	{
		velocity.x *= friction;
	}

	auto view = registry.view<CircleData>();
	constexpr float friction = 0.98f;
	for (auto [ent, circle] : view.each())
	{
		//if(circle);
	}
}

void SystemCirclesMover_OOP(std::vector<Circle*>& circles)
{
	for (auto& c : circles)
	{
		auto* posComp = c->GetComponent<PositionComp>();
		posComp->Move(1.0f);
		c->GetComponent<VelocityComp>()->ApplyFriction();
		c->CheckNeighbors(c->radius, circles);
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
