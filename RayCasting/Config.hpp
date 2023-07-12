#pragma once

// Singleton config
class GameConfig
{	
	GameConfig() = default;
	GameConfig(const GameConfig&) = delete;
	void operator=(const GameConfig&) = delete;
	~GameConfig() = default;
public:
	static GameConfig& instance() {
		static GameConfig config;
		return config;
	}
	unsigned frameRate = 144;
	int windowWidth = 1280;
	int widowHeight = 720;
};



