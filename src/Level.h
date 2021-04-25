#pragma once
#include "SFML/Graphics.hpp"



class Level : public sf::Drawable
{
public:
	enum LevelState
	{
		RUNNING,
		OVER,
		RESET
	};
	virtual void handleEvent(sf::Event e) = 0;
	virtual void update(float dt) = 0;
	void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
	virtual LevelState getState();
protected:
	LevelState m_state;
private:
	
};