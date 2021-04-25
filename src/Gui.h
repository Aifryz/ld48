#pragma once
#include "SFML/Graphics.hpp"

class Gui : public sf::Drawable
{
public:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
	virtual int handleEvent(sf::Event e) = 0;
private:

};