#pragma once

#include "SFML/Graphics.hpp"

class Player: public sf::Drawable
{
public:
	Player(const sf::Texture& tex);
	Player();
	void update(float dt);
	bool handleInput(const sf::Event& evt);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Vector2f getPosition() const;
	sf::FloatRect getBBox() const;

	void handleTileColision(sf::Vector2f colision_vector);

	sf::Vector2f getSpeed();

	int getHealth();

	bool isAccelerating();

	void setPosition(sf::Vector2f pos);

	bool isDead() const;

	void damage(int damage);

	void repair();
	void addArmor(int amount);
	void addtorpedoes(int amount);
	bool takeTorpedo();

	int getNumOftorpedoes() const;
	int getMaxHealth() const;


private:
	sf::Sprite m_sprite;
	sf::Vector2f m_speed;
	sf::Vector2f m_position;
	sf::Vector2f m_acc;
	bool active;
	bool facing_left;
	int m_health;
	int m_max_healt;
	int m_torpedoes;
};