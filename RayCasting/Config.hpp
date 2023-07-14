#pragma once
#include <string>
// Singleton config
class GameConfig
{	
	GameConfig() = default;
	GameConfig(const GameConfig&) = delete;
	void operator=(const GameConfig&) = delete;
	~GameConfig() = default;
public:
	static GameConfig& instance();
	unsigned frameRate = 144;
	int windowWidth = 1280;
	int widowHeight = 720;
	std::string windowName = "RayCasting Examples";
	const float pi = 3.14159265358979323846f;
};



