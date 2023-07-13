#pragma once
#include "Scene.hpp"
#include "SFML/Graphics.hpp"
#include "Vec2.hpp"
#include <iostream>
class GameSystem
{
	sf::RenderWindow m_window;
	std::unique_ptr<Scene> m_curScene;
	

	std::pair<bool, Vec2> checkAllIntersection(const Vec2& a, const Vec2& b) {
		bool intersectionExists = false;
		float minDist = 100000.f;
		Vec2 intersection;
		auto manager = m_curScene->getManager();
		auto rederables = manager->getEntities(ComponentType::RENDER);
		for (auto& entity : rederables) {
			if (entity->hasComponent<CRayCast>()) {
				continue;
			}
			auto cRender = entity->getComponent<CRender>();
			const auto& transform = cRender->states.transform;
			size_t size = cRender->vertexArray.getVertexCount();

			for (size_t i = 0; i < size; ++i) {
				auto p1 = transform * cRender->vertexArray[i].position;
				auto p2 = transform * cRender->vertexArray[(i + 1) % size].position;
				auto result = checkIntersection(a, b, p1, p2);
				if (result.valid && minDist > result.t) {
					intersectionExists = true;
					minDist = result.t;
					intersection = result.intersection;
				}
			}
		}
		return { intersectionExists, intersection };
	}

	void drawRay(const Vec2& a, const Vec2& b) {
		auto [found, intersection] = checkAllIntersection(a, b);
		if (found) {
			sf::VertexArray line(sf::Lines, 2);
			line[0].position = a.toVec2f();
			line[1].position = intersection.toVec2f();
			line[0].color = sf::Color::Red;
			line[1].color = sf::Color::Red;
			sf::CircleShape circle(8.f);
			circle.setOrigin(8.f, 8.f);
			circle.setPosition(intersection.toVec2f());
			circle.setFillColor(sf::Color::Red);
			m_window.draw(line);
			m_window.draw(circle);
		}
	}

	void render() {
		m_window.clear();
		// Handler Renderable entities
		auto manager = m_curScene->getManager();
		auto rederables = manager->getEntities(ComponentType::RENDER);
		for (auto& entity : rederables) {
			auto cRender = entity->getComponent<CRender>();
			m_window.draw(cRender->vertexArray, cRender->states);
		}
		// Handle Ray Casting
		auto raycasters = manager->getEntities(ComponentType::RAYCAST);
		for (auto& entity : raycasters) {
			auto cRayCast = entity->getComponent<CRayCast>();
			auto cRender = entity->getComponent<CRender>();
			if (cRayCast->toMouse) {
				auto& config = GameConfig::instance();
				auto entityPos = Vec2(cRender->states.transform * sf::Vector2f(0, 0));
				auto mousePos = Vec2(sf::Mouse::getPosition(m_window));
				auto directionVec = (mousePos - entityPos).resize(cRayCast->rayLength);
				drawRay(entityPos, entityPos + directionVec);
			}
			else {
				size_t numRays = cRayCast->numRays;
				auto& config = GameConfig::instance();
				auto entityPos = Vec2(cRender->states.transform * sf::Vector2f(0, 0));
				float unitTheta =  2 * config.pi / numRays;
				float rayLength = cRayCast->rayLength;
				float theta = 0;
				for (size_t i = 0; i < numRays; ++i, theta+=unitTheta) {
					auto directionVec = Vec2(rayLength * std::cosf(theta), rayLength * std::sinf(theta));
					drawRay(entityPos, entityPos + directionVec);
				}
			}
		}

		m_window.display();
	}

public:
	GameSystem() : m_window(sf::VideoMode(GameConfig::instance().windowWidth, GameConfig::instance().widowHeight), "SFML works!")
	{	
		m_window.setFramerateLimit(GameConfig::instance().frameRate);
		setScene(std::make_unique<Scene1>());
	}
	void run() {
		while (m_window.isOpen())
		{	
			update();
			handleUserInput();
			physics();
			render();
		}
	}

	void physics() {
		auto manager = m_curScene->getManager();
		// Handle mouse following entities
		auto followers = manager->getEntities(ComponentType::FOLLOWCURSOR);	
		for (auto& entity : followers) {
			auto cFlollow = entity->getComponent<CFlollowCursor>();
			if (cFlollow->follow) {
				auto cRender = entity->getComponent<CRender>();
				cRender->states.transform = sf::Transform::Identity;
				cRender->states.transform.translate(sf::Vector2f(sf::Mouse::getPosition(m_window)));
			}
		}
	}
	void update() {
		auto manager = m_curScene->getManager();
		manager->update();
	}
	void setScene(std::unique_ptr<Scene>&& scene) {
		m_curScene = std::move(scene);
		m_curScene->init();
	}
	void handleUserInput() {
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				m_window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.scancode == sf::Keyboard::Scan::Num1) {
					setScene(std::make_unique<Scene1>());
				}
				else if (event.key.scancode == sf::Keyboard::Scan::Num2) {
					setScene(std::make_unique<Scene2>());
				}
			}
		}
	}
};

