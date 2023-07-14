#pragma once
#include "Scene.hpp"
#include "SFML/Graphics.hpp"
#include "Vec2.hpp"
#include <iostream>
#include <algorithm>
class GameSystem
{
	sf::RenderWindow m_window;
	std::unique_ptr<Scene> m_curScene;
	std::pair<bool, Vec2> calculateRay(const Vec2& rayFrom, const Vec2& rayTo);
	void drawRay(const sf::Vector2f& from, const sf::Vector2f& to);
	void drawTriangle(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3);
	std::vector<Vec2> calculateVertexRay(const Vec2& rayFrom, float rayLength);
	void render();
	void physics();
	void update();
	void setScene(std::unique_ptr<Scene>&& scene);
	void handleUserInput();
public:
	GameSystem();
	void run();

	
};

