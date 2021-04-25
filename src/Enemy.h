#pragma once
#include "SFML/Graphics.hpp"
class Enemy: public sf::Drawable
{
public:
	Enemy(const sf::Texture& tex);
	Enemy();
	void update(float dt);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::Vector2f getPosition() const;
	sf::FloatRect getBBox() const;

	void handleTileColision(sf::Vector2f colision_vector);

	sf::Vector2f getSpeed();
	int getHealth();
	bool isAccelerating();
	bool isDead();
	void setPosition(sf::Vector2f pos);
	void damage(int damage);

	void setTarget(sf::Vector2f target);

	bool canShoot() const;
	sf::Vector2f getTorpedoDir() const;

private:
	sf::Sprite m_sprite;
	sf::Vector2f m_speed;
	sf::Vector2f m_position;
	sf::Vector2f m_acc;
	sf::Vector2f m_target;
	sf::Vector2f m_torpedo_dir;
	int m_health;
	float m_torpedo_timer;
	bool m_dead;
	bool m_target_ranged;
};