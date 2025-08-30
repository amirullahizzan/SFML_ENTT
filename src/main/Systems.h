struct RenderData
{
	float radius;
	sf::Color color;
};

struct TestData
{
	float a;
};

struct Position
{
	float x, y;
};

struct Velocity
{
	float x, y;
};


void SystemClickCircle(sf::RenderWindow& window, entt::registry& registry)
{
	/*for (auto [ent, circle, pos] : registry.view<sf::CircleShape, sf::Vector2f>().each())
	{

	}*/
}

void SystemCirclesMover(sf::RenderWindow& window, entt::registry& registry)
{
	//registry.group<Position, Velocity, RenderData>().  //cannot call three components directly
	registry.group<Position>(entt::get<Velocity, TestData>).
		each([](auto& pos, auto& velocity, auto& renderDat)
			{
				pos.x += velocity.x;
			});
}

void SystemVelocity(sf::RenderWindow& window, entt::registry& registry)
{
	constexpr float friction = 0.97f;
	//SystemVelocity
	registry.view<Velocity>().each([friction](auto& velocity)
			{
				velocity.x *= friction;
				velocity.y *= friction;
			});
}

sf::Color  GetRandomColorSF(float _alpha = 255)
{
	float r = Randomizer::GetRandI(0, 255);
	float g = Randomizer::GetRandI(0, 255);
	float b = Randomizer::GetRandI(0, 255);
	return sf::Color(r, g, b, _alpha);
}

void SystemRandomizeColor(sf::RenderWindow& window, entt::registry& registry)
{
	auto view = registry.view<RenderData>().each(); //use view because only one component
	for (auto [ent, renderData] : view)
	{
		renderData.color = GetRandomColorSF();
	}
}

void SystemCircleLooper(sf::RenderWindow& window, entt::registry& registry)
{
	//sf::Vector2f windowPos = static_cast<sf::Vector2f>(window.getSize());
	//auto group = registry.group<Position, RenderData>().each(); //this creates a compact new group of it and store it permanently
	//for (auto [ent, pos, _renderData] : group)
	//{
	//	if (pos.x >= windowPos.x)
	//	{
	//		pos.x = 0.0f;
	//	}
	//}
}

void SystemRender(sf::RenderWindow& window, entt::registry& registry)
{
	auto group = registry.group<Position, RenderData>().each();
	for (auto [entity, position, renderData] : group)
	{
		sf::CircleShape circle;
		circle.setRadius(renderData.radius);
		circle.setFillColor(renderData.color);
		circle.setPosition(sf::Vector2f(position.x, position.y));
		window.draw(circle);
	}

}

