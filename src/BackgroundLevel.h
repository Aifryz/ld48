#pragma once

#include "Level.h"
#include<unordered_map>

class BackgroundLevel: public Level
{
public:
	BackgroundLevel(std::unordered_map<std::string, sf::Texture>& textures);

	virtual void handleEvent(sf::Event e);
	virtual void update(float dt);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
private:
	std::unordered_map<std::string, sf::Texture>& m_textures;
	sf::Sprite m_grad;
};