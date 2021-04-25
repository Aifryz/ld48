#include "Game.h"
#include <filesystem>
#include <iostream>
#include"SFML/Graphics.hpp"

#include "BackgroundLevel.h"
#include "StartGui.h"
#include "OverGui.h"



Game::Game()
{
}

void Game::run()
{
	//Create window, load textures, fonts, sounds
	sf::Clock clk;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Deep Dive");
	// run the program as long as the window is open

	namespace fs = std::filesystem;
	try
	{
		for (auto& p : fs::directory_iterator("res/textures"))
		{
			auto filename = p.path().filename();
			std::string path = p.path().generic_string();
			sf::Texture texture;
			texture.loadFromFile(path);
			m_textures[filename.generic_string()] = texture;
		}
	}
	catch (fs::filesystem_error& e)
	{
		std::cout << e.what() << "\n";
	}

	m_font = sf::Font();
	m_font.loadFromFile("res/fonts/type_wrong/typwrng.ttf");
	//m_level = std::make_unique<GameLevel>(m_textures);
	m_level = std::make_unique<BackgroundLevel>(m_textures);
	m_gui = std::make_unique<StartGui>(m_textures, m_font);


	clk.restart();

	while (window.isOpen())
	{
		sf::Time dt = clk.restart();
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
			else
			{
				if (m_gui)
				{
					int action = m_gui->handleEvent(event);
					if (action == 1)
					{
						m_level = std::make_unique<GameLevel>(m_textures);
						m_gui.reset();
					}
					else if (action == -1)
					{
						window.close();
					}
				}
				else
				{
					m_level->handleEvent(event);
				}
			}
		}

		m_level->update(dt.asSeconds());

		Level::LevelState ls = m_level->getState();
		if (ls == Level::OVER) // Show over gui
		{
			if (!m_gui)
			{
				m_gui = std::make_unique<OverGui>(m_textures, m_font);
			}
		}
		window.clear();
		window.draw(*m_level);
		if (m_gui)
		{
			window.draw(*m_gui);
		}
		window.display();
	}
}

