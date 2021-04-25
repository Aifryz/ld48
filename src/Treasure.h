#pragma once
#include "SFML/Graphics.hpp"

class Treasure : public sf::Drawable
{
public:
	enum TreasureType
	{
		COINS,
		CHEST,
		torpedoes,
		REPAIR,
		ARMOR,
		TROPHY
	};
	Treasure();
	Treasure(sf::Texture* txt, TreasureType type);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	sf::Vector2f getPosition() const;
	sf::FloatRect getBBox() const;
	void update(float dt);
	void setPosition(sf::Vector2f pos);
	bool isCollected();
	TreasureType getType();
	TreasureType collect();
private:
	bool m_collected;
	sf::Texture* texture;
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	sf::Sprite m_sprite;
	TreasureType m_type;
};