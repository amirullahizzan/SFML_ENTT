#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>


void GameInit(sf::RenderWindow& window, entt::registry& registry);
void GameMain(sf::RenderWindow& window, entt::registry& registry);
