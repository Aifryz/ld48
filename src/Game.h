#pragma once

#include "SFML/Graphics.hpp"
#include "GameLevel.h"
#include "BackgroundLevel.h"
#include "Gui.h"


class Game
{
public:
	enum GameState
	{
		START_MENU,
		PLAYING
	};
	Game();
	void run();

private:
	std::unordered_map<std::string, sf::Texture> m_textures;
	std::unique_ptr<Level> m_level;
	std::unique_ptr<Gui> m_gui;
	sf::Font m_font;
};