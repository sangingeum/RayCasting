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
	
	std::pair<bool, Vec2> calculateRay(const Vec2& rayFrom, const Vec2& rayTo) {
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
				auto p1 = Vec2(transform * cRender->vertexArray[i].position);
				auto p2 = Vec2(transform * cRender->vertexArray[(i + 1) % size].position);
				auto result = checkIntersection(rayFrom, rayTo, p1, p2);
				if (result.valid && minDist > result.t) {
					intersectionExists = true;
					minDist = result.t;
					intersection = result.intersection;
				}
			}
		}
		return { intersectionExists, intersection };
	}

	void drawRay(const sf::Vector2f& from, const sf::Vector2f& to) {
		sf::VertexArray line(sf::Lines, 2);
		line[0].position = from;
		line[1].position = to;
		line[0].color = sf::Color::Red;
		line[1].color = sf::Color::Red;
		sf::CircleShape circle(8.f);
		circle.setOrigin(8.f, 8.f);
		circle.setPosition(to);
		circle.setFillColor(sf::Color::Red);
		m_window.draw(line);
		m_window.draw(circle);
	}

	void drawTriangle(const sf::Vector2f& v1, const sf::Vector2f& v2, const sf::Vector2f& v3){
		sf::VertexArray triangle(sf::Triangles, 3);
		triangle[0].position = v1;
		triangle[1].position = v2;
		triangle[2].position = v3;
		triangle[0].color = sf::Color(255, 0, 0, 120);
		triangle[1].color = sf::Color(255, 0, 0, 120);
		triangle[2].color = sf::Color(255, 0, 0, 120);
		m_window.draw(triangle);
	}

	std::vector<Vec2> calculateVertexRay(const Vec2& rayFrom, float rayLength) {
		std::vector<Vec2> results;
		auto& renderables = m_curScene->getManager()->getEntities(ComponentType::RENDER);
		for (auto& entity : renderables) {
			if (entity->hasComponent<CRayCast>()) {
				continue;
			}
			auto cRender = entity->getComponent<CRender>();
			auto& vertexArray = cRender->vertexArray;
			auto& transform = cRender->states.transform;
			std::vector<std::pair<bool, Vec2>> tempVec;
			size_t size = vertexArray.getVertexCount();
			for (size_t i = 0; i < size; ++i) {
				auto toEdgeVec = (Vec2(transform * vertexArray[i].position) - rayFrom).resize(rayLength);
				//tempVec.push_back(calculateRay(rayFrom, toEdgeVec + rayFrom));
				tempVec.push_back(calculateRay(rayFrom, toEdgeVec.rotate_(rayFrom, 0.001) + rayFrom));
				tempVec.push_back(calculateRay(rayFrom, toEdgeVec.rotate_(rayFrom, -0.001) + rayFrom));
				for (auto& result : tempVec) {
					if (result.first) {
						results.push_back(result.second);
					}
				}
			}
		}
		return results;
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
				auto [found, intersection] = calculateRay(entityPos, entityPos + directionVec);
				if (found) {
					drawRay(entityPos.toVec2f(), intersection.toVec2f());
				}
			}
			else {
				auto entityPos = Vec2(cRender->states.transform * sf::Vector2f(0, 0));
				float rayLength = cRayCast->rayLength;
				std::vector<Vec2> intersections;
				if (cRayCast->sweep) {
					// Cast 3 rays to all vertices in the scene
					intersections = calculateVertexRay(entityPos, rayLength);
					// Sort intersections radially
					std::sort(intersections.begin(), intersections.end(),
						[entityPos](const Vec2& a, const Vec2& b) {
							auto centeredA = a - entityPos;
							auto centeredB = b - entityPos;
							return std::atan2(centeredA.y, centeredA.x) > std::atan2(centeredB.y, centeredB.x);
						});
				}
				else {
					// Emit equally spaced radial rays
					size_t numRays = cRayCast->numRays;
					auto& config = GameConfig::instance();
					float unitTheta = 2 * config.pi / numRays;
					float theta = 0;
					for (size_t i = 0; i < numRays; ++i, theta += unitTheta) {
						auto directionVec = Vec2(rayLength * std::cosf(theta), rayLength * std::sinf(theta));
						auto [found, intersection] = calculateRay(entityPos, entityPos + directionVec);
						if (found) {
							intersections.push_back(intersection);
						}
					}
				}
				// Draw rays & Draw triangles given the entity position and intersection postions
				size_t size = intersections.size();
				auto from = entityPos.toVec2f();
				for (size_t i = 0; i < size; ++i) {
					auto to = intersections[i].toVec2f();
					if (cRayCast->drawTriangle) {
						drawTriangle(from, to, intersections[(i + 1) % size].toVec2f());
					}
					if (cRayCast->drawRay) {
						drawRay(from, to);
					}
				}
				
			}
		}

		m_window.display();
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
				else if (event.key.scancode == sf::Keyboard::Scan::Num3) {
					setScene(std::make_unique<Scene3>());
				}
				else if (event.key.scancode == sf::Keyboard::Scan::Num4) {
					setScene(std::make_unique<Scene4>());
				}
				else if (event.key.scancode == sf::Keyboard::Scan::Num5) {
					setScene(std::make_unique<Scene5>());
				}
			}
		}
	}
public:
	GameSystem() : m_window(sf::VideoMode(GameConfig::instance().windowWidth, GameConfig::instance().widowHeight), GameConfig::instance().windowName.c_str())
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

	
};

