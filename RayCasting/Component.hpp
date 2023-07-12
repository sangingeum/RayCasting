#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
enum class ComponentType {
	RENDER,
	RAYCAST,
	FOLLOWCURSOR
};

class CRender
{
public:
	sf::VertexArray vertexArray;
	sf::RenderStates states = sf::RenderStates::Default;
	CRender(const sf::VertexArray& vertexArray_) 
		: vertexArray(vertexArray_){}
	CRender(const sf::VertexArray& vertexArray_, const sf::RenderStates& states_)
		: vertexArray(vertexArray_), states(states_) {}
};

class CRayCast
{
public:
	bool toMouse = false;
	size_t numRays = 10;
	float rayLength = 4000;
	CRayCast(size_t numRays_) : numRays(numRays_) {}
	CRayCast(bool toMouse_) : toMouse(toMouse_) {}
};

class CFlollowCursor
{
public:
	bool follow = true;
	CFlollowCursor() = default;
	CFlollowCursor(bool follow_) : follow(follow_) {}
};

