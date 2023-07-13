#pragma once
#include "EntityManager.hpp"
#include "Config.hpp"
#include <cmath>
class Scene
{
protected:
	std::shared_ptr<EntityManager> m_entityManager;
	sf::Color m_blockColor = sf::Color::White;
	
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
		float pi = GameConfig::instance().pi;
		for (size_t i = 1; i <= numVertices+1; ++i) {
			float theta = i * 2 * pi / numVertices ;
			vertexArray[i].position = sf::Vector2f(radius* std::cosf(theta), radius * std::sinf(theta));
			vertexArray[i].color = m_blockColor;
		}
		return vertexArray;
	}

	sf::VertexArray createBoundaryVertices() {
		sf::VertexArray vertexArray(sf::LineStrip, 4);
		auto& config = GameConfig::instance();
		vertexArray[0].position = sf::Vector2f(0, 0);
		vertexArray[1].position = sf::Vector2f(config.windowWidth+1, 0);
		vertexArray[2].position = sf::Vector2f(config.windowWidth+1, config.widowHeight);
		vertexArray[3].position = sf::Vector2f(0, config.widowHeight);
		return vertexArray;
	}

	std::shared_ptr<CRender> addBlock(float width, float height, float translateX, float translateY) {
		auto vertexArray = createSquareVertices(width, height);
		auto entity = m_entityManager->addEntity();
		auto cRender = entity->addComponent<CRender>(vertexArray);
		cRender->states.transform.translate(translateX, translateY);
		return cRender;
	}

	void baseInit() {
		// Make quad blocks
		auto cRender = addBlock(100.f, 100.f, 100.f, 100.f);
		cRender->vertexArray[2] = sf::Vector2f(-20, -20);

		cRender = addBlock(100.f, 100.f, 400.f, 500.f);
		cRender->vertexArray[1] = sf::Vector2f(90, 0);

		cRender = addBlock(100.f, 100.f, 200.f, 300.f);
		cRender->vertexArray[0] = sf::Vector2f(-90, -90);

		cRender = addBlock(100.f, 100.f, 900.f, 300.f);
		cRender->vertexArray[2] = sf::Vector2f(90, 20);

		cRender = addBlock(100.f, 100.f, 300.f, 50.f);
		cRender->vertexArray[3] = sf::Vector2f(-0, 90);
		cRender->vertexArray[1] = sf::Vector2f(60, 0);

		cRender = addBlock(100.f, 100.f, 600.f, 150.f);
		cRender->vertexArray[0] = sf::Vector2f(-90, -90);
		cRender->vertexArray[3] = sf::Vector2f(90, 120);
		cRender->states.transform.rotate(110);


		// Make a circle block
		auto entity = m_entityManager->addEntity();
		auto circleVA = createCircleVertices(30, 50.f);
		cRender = entity->addComponent<CRender>(circleVA);
		cRender->states.transform.translate(800, 500);

		// Make the boundary
		auto boundaryVA = createBoundaryVertices();
		entity = m_entityManager->addEntity();
		cRender = entity->addComponent<CRender>(boundaryVA);
	}

	void addStationaryRayCaster() {
		// Make a raycaster
		auto entity = m_entityManager->addEntity();
		auto circleVA = createCircleVertices(30, 10.f);
		auto cRender = entity->addComponent<CRender>(circleVA);
		cRender->states.transform.translate(GameConfig::instance().windowWidth / 2.f, GameConfig::instance().widowHeight / 2.f);
		entity->addComponent<CRayCast>();
	}

	void addFollowingRayCaster(size_t numRays, bool drawTriangle, bool drawRay, bool sweep) {
		// Make a raycaster
		auto entity = m_entityManager->addEntity();
		auto circleVA = createCircleVertices(30, 10.f);
		auto cRender = entity->addComponent<CRender>(circleVA);
		cRender->states.transform.translate(GameConfig::instance().windowWidth / 2.f, GameConfig::instance().widowHeight / 2.f);
		entity->addComponent<CRayCast>(numRays, drawTriangle, drawRay, sweep);
		entity->addComponent<CFlollowCursor>(true);
	}

};
//size_t numRays_, bool drawTriangle_, bool drawRay_, bool sweep_
class Scene1 : public Scene {

public:
	void init() override {
		baseInit();
		addStationaryRayCaster();
	}
};


class Scene2 : public Scene {

public:
	void init() override {
		baseInit();
		addFollowingRayCaster(30, false, true, false);
	}
};


class Scene3 : public Scene {

public:
	void init() override {
		baseInit();
		addFollowingRayCaster(30, true, true, false);
	}
};

class Scene4 : public Scene {

public:
	void init() override {
		baseInit();
		addFollowingRayCaster(30, true, true, true);
	}
};

class Scene5 : public Scene {

public:
	void init() override {
		baseInit();
		addFollowingRayCaster(30, true, false, true);
	}
};
