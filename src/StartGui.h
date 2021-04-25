#pragma once
#include "Gui.h"
#include<unordered_map>

class StartGui : public Gui
{
public:
	StartGui(std::unordered_map<std::string, sf::Texture>& m_textures, const sf::Font& font);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	int handleEvent(sf::Event e);
private:
	std::unordered_map<std::string, sf::Texture> m_textures;
	sf::Sprite m_start_sprite;
	sf::Sprite m_credits_sprite;
	sf::Sprite m_quit_sprite;

	sf::Text m_start_text;
	sf::Text m_credits_text;
	sf::Text m_quit_text;
	sf::Text m_instructions;
	sf::Text m_instr2;
	sf::Text m_cred_text;
	sf::Text m_credits;

	sf::Sprite m_title_sprite;
	int m_mode;
};