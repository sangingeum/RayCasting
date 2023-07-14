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
	Scene();
	virtual void init() = 0;
	std::shared_ptr<EntityManager> getManager();
	sf::VertexArray createSquareVertices(float width, float height);
	sf::VertexArray createCircleVertices(size_t numVertices, float radius);
	sf::VertexArray createBoundaryVertices();
	std::shared_ptr<CRender> addBlock(float width, float height, float translateX, float translateY);
	void baseInit();
	void addStationaryRayCaster();
	void addFollowingRayCaster(size_t numRays, bool drawTriangle, bool drawRay, bool sweep);
};

//Concrete Scenes
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
