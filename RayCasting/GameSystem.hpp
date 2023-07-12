#pragma once
#include "Scene.hpp"
#include "SFML/Graphics.hpp"
#include "Vec2.hpp"
#include <iostream>
class GameSystem
{
	sf::RenderWindow m_window;
	std::unique_ptr<Scene> m_curScene;
	
	std::pair<bool, Vec2> intersectionCheckAll(const Vec2& a, const Vec2& b, float minDist) {
		bool intersectionExists = false;
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
				auto result = intersectionCheck(a, b, p1, p2);;
				if (result.valid && minDist > result.t) {
					intersectionExists = true;
					minDist = result.t;
					intersection = result.intersection;
				}
			}
		}
		return { intersectionExists, intersection };
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
			if (cRayCast->toMouse) {
				auto& config = GameConfig::instance();
				auto centerPos = Vec2(config.windowWidth / 2.f, config.widowHeight / 2.f);
				auto mousePos = Vec2(sf::Mouse::getPosition(m_window));
				auto direction = (mousePos - centerPos).resize(cRayCast->rayLength);
				auto [found, intersection] = intersectionCheckAll(centerPos, centerPos + direction, cRayCast->rayLength);
				sf::VertexArray line(sf::Lines, 2);
				line[0].position = centerPos.toVec2f();
				line[1].position = intersection.toVec2f();
				line[0].color = sf::Color::Red;
				line[1].color = sf::Color::Red;
				m_window.draw(line);
			
			}
			else {

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
			sf::Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_window.close();
			}
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

};

