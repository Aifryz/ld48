#include "Player.h"
#include <iostream>

Player::Player(const sf::Texture& tex)
{
	m_sprite = sf::Sprite(tex);
	m_position = sf::Vector2f(0.f, 0.f);
	m_speed = sf::Vector2f(0.f, 0.f);

	m_sprite.setOrigin(m_sprite.getLocalBounds().width/2.f, m_sprite.getLocalBounds().height / 2.f);

	m_health = 1000;

	m_max_healt = 1000;
	m_torpedoes = 20;
}

Player::Player()
{
	m_position = sf::Vector2f(0.f, 0.f);
	m_speed = sf::Vector2f(0.f, 0.f);
	m_health = 1000;
}

void Player::setPosition(sf::Vector2f pos)
{
	m_position = pos;
}

bool Player::handleInput(const sf::Event& evt)
{
	return false;
}

void Player::damage(int damage)
{
	m_health -= damage;
}

void Player::update(float dt)
{
	if (isDead())
	{
		return;
	}
	sf::Vector2f new_acc = { 0.f, 0.f };

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		new_acc.y = -1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		new_acc.y = 1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		new_acc.x = -1.f;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		new_acc.x = 1.f;
	}
	m_acc = new_acc*100.f;
	m_speed += m_acc * dt;

	//Clip speed
	std::clamp(m_speed.x, -100.f, 100.f);
	std::clamp(m_speed.y, -100.f, 100.f);
	
	//Simulate drag
	auto rev = dt*m_speed * -0.01f;
	m_speed += rev;

	if (m_speed.x > 0)
	{
		m_sprite.setScale(-1.f, 1.f);
	}
	else
	{
		m_sprite.setScale(1.f, 1.f);
	}

	
	m_position = m_position + m_speed * dt;
	m_sprite.setPosition(m_position);

}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (!isDead())
	{
		target.draw(m_sprite, states);
	}
}

sf::Vector2f Player::getPosition() const
{
	return m_position;
}

sf::FloatRect Player::getBBox() const
{
	sf::Vector2f sz{ 64.f, 16.f };
	sf::Vector2f tl = m_position - sz *0.5f;

	return sf::FloatRect(tl, sz); // HACK, smaller collision
}

void Player::handleTileColision(sf::Vector2f colision_vector)
{
	const float BOUNCE_FACTOR = 0.3f;
	if (colision_vector.x != 0) // Hit vertical wall
	{
		if (m_speed.x > 0)
		{
			m_position.x -= colision_vector.x;
		}
		else
		{
			m_position.x += colision_vector.x;
		}
		float damage = fabs(m_speed.x);
		m_health -= damage;
		std::cout << m_health<<"\n";
		m_speed.x = -BOUNCE_FACTOR * m_speed.x;
	}
	else // Hit horizontal 
	{
		if (m_speed.y > 0)
		{
			m_position.y -= colision_vector.y;
		}
		else
		{
			m_position.y += colision_vector.y;
		}
		float damage = fabs(m_speed.y)*0.3f;
		m_health -= damage;
		std::cout << m_health << "\n";
		m_speed.y = -BOUNCE_FACTOR * m_speed.y;
	}
}

sf::Vector2f Player::getSpeed()
{
	return m_speed;
}

int Player::getHealth()
{
	return m_health;
}

bool Player::isAccelerating()
{
	return m_acc != sf::Vector2f{0.f, 0.f};
}

bool Player::isDead() const
{
	return (m_health <= 0);
}

void Player::repair()
{
	m_health = m_max_healt;
}
void Player::addArmor(int amount)
{
	m_max_healt += amount;
}
void Player::addtorpedoes(int amount)
{
	m_torpedoes += amount;
}
bool Player::takeTorpedo()
{
	if (m_torpedoes > 0)
	{
		m_torpedoes -= 1;
		return true;
	}
	return false;
}

int Player::getNumOftorpedoes() const
{
	return m_torpedoes;
}
int Player::getMaxHealth() const
{
	return m_max_healt;
}