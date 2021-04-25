#include "Treasure.h"
Treasure::Treasure()
{
}

Treasure::Treasure(sf::Texture* txt, Treasure::TreasureType type)
{
	sf::IntRect select = sf::IntRect(0, 0, 32, 32);
	if (type == Treasure::COINS)
	{
		select = sf::IntRect(96, 0, 32, 32);
	}
	else if (type == Treasure::CHEST)
	{
		select = sf::IntRect(0, 32, 32, 32);
	}
	else if (type == Treasure::ARMOR)
	{
		select = sf::IntRect(64, 0, 32, 32);
	}
	else if (type == Treasure::REPAIR)
	{
		select = sf::IntRect(32, 0, 32, 32);
	}
	else if (type == Treasure::torpedoes)
	{
		select = sf::IntRect(0, 0, 32, 32);
	}
	else if (type == Treasure::TROPHY)
	{
		select = sf::IntRect(32, 32, 32, 32);
	}
	m_sprite = sf::Sprite(*txt, select);
	m_sprite.setOrigin({ 16.f, 16.f });
	m_collected = false;
	m_type = type;
}

void Treasure::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (!m_collected)
	{
		target.draw(m_sprite);
	}
}

sf::Vector2f Treasure::getPosition() const
{
	return m_position;
}

sf::FloatRect Treasure::getBBox() const
{
	sf::Vector2f sz = sf::Vector2f(32.f, 32.f);
	sf::Vector2f tl = m_position - sz* 0.5f;
	return sf::FloatRect(tl, sz);
	
}

void Treasure::update(float dt)
{

}

void Treasure::setPosition(sf::Vector2f pos)
{
	m_position = pos;
	m_sprite.setPosition(pos);
}

bool Treasure::isCollected()
{
	return m_collected;
}
Treasure::TreasureType Treasure::getType()
{
	return m_type;
}
Treasure::TreasureType Treasure::collect()
{
	m_collected = true;
	return m_type;
}