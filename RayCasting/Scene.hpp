#pragma once
#include "EntityManager.hpp"
#include "Config.hpp"
#include <cmath>
class Scene
{
protected:
	std::shared_ptr<EntityManager> m_entityManager;
	sf::Color m_blockColor = sf::Color::White;
	const float pi = 3.14159265358979323846f;
public:
	virtual ~Scene() = default;
	Scene() : m_entityManager(std::make_shared<EntityManager>()) {}
	std::shared_ptr<EntityManager> getManager() {
		return m_entityManager;
	}
	virtual void init() = 0;

	sf::VertexArray createSquareVertices(float width, float height) {
		sf::VertexArray vertexArray(sf::Quads, 4);
		vertexArray[0].position = sf::Vector2f(-width/2.f, -height/2.f);
		vertexArray[1].position = sf::Vector2f(width/2.f, -height/2.f);
		vertexArray[2].position = sf::Vector2f(width/2.f, height/2.f);
		vertexArray[3].position = sf::Vector2f(-width/2.f, height/2.f);
		vertexArray[0].color = m_blockColor;
		vertexArray[1].color = m_blockColor;
		vertexArray[2].color = m_blockColor;
		vertexArray[3].color = m_blockColor;
		return vertexArray;
	}

	sf::VertexArray createCircleVertices(size_t numVertices, float radius) {
		sf::VertexArray vertexArray(sf::TriangleFan, numVertices + 2);
		vertexArray[0].position = sf::Vector2f(0, 0);
		vertexArray[0].color = m_blockColor;
		for (size_t i = 1; i <= numVertices+1; ++i) {
			float theta = i * 2 * pi / numVertices ;
			vertexArray[i].position = sf::Vector2f(radius* std::cosf(theta), radius * std::sinf(theta));
			vertexArray[i].color = m_blockColor;
		}
		return vertexArray;
	}

	sf::VertexArray createBoundary() {
		sf::VertexArray vertexArray(sf::LineStrip, 4);
		auto& config = GameConfig::instance();
		vertexArray[0].position = sf::Vector2f(0, 0);
		vertexArray[1].position = sf::Vector2f(config.windowWidth, 0);
		vertexArray[2].position = sf::Vector2f(config.windowWidth, config.widowHeight);
		vertexArray[3].position = sf::Vector2f(0, config.widowHeight);
		return vertexArray;
	}

};

class Scene1 : public Scene {

	
public:
	void init() override {

		auto vertexArray = createSquareVertices(100.f, 100.f);
		auto entity = m_entityManager->addEntity();
		auto cRender = entity->addComponent<CRender>(vertexArray);
		cRender->states.transform.translate(100.f, 100.f);
		entity = m_entityManager->addEntity();
		cRender = entity->addComponent<CRender>(vertexArray);
		cRender->states.transform.translate(200.f, 300.f);
		entity = m_entityManager->addEntity();
		cRender = entity->addComponent<CRender>(vertexArray);
		cRender->states.transform.translate(400.f, 500.f);
		entity = m_entityManager->addEntity();
		cRender = entity->addComponent<CRender>(vertexArray);
		cRender->states.transform.translate(700.f, 300.f);

		auto boundaryVA = createBoundary();
		entity = m_entityManager->addEntity();
		cRender = entity->addComponent<CRender>(boundaryVA);
		

		entity = m_entityManager->addEntity();
		auto circleVA = createCircleVertices(30, 10.f);
		cRender = entity->addComponent<CRender>(circleVA);
		cRender->states.transform.translate(GameConfig::instance().windowWidth / 2.f, GameConfig::instance().widowHeight / 2.f);
		entity->addComponent<CRayCast>(true);

	}
};



