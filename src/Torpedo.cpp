#include "Torpedo.h"
#include <iostream>
Torpedo::Torpedo()
{
}

Torpedo::Torpedo(sf::Texture* txt, sf::Texture* exp_texture)
{
	m_sprite = sf::Sprite(*txt, sf::IntRect(0, 0, 32, 32));
	m_sprite.setOrigin({ 16.f, 16.f });

	m_explosion_sprite = sf::Sprite(*exp_texture, sf::IntRect(0, 0, 32, 32));
	m_explosion_sprite.setOrigin({ 16.f, 16.f });
	m_dead = false;
}

void Torpedo::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
		if (!m_triggered)
		{
			target.draw(m_sprite);
		}
		else
		{
			target.draw(m_explosion_sprite);
		}
}

sf::Vector2f Torpedo::getPosition() const
{
	return m_position;
}

sf::FloatRect Torpedo::getBBox() const
{
	sf::Vector2f sz = sf::Vector2f(32.f, 32.f);
	sf::Vector2f tl = m_position - sz* 0.5f;
	return sf::FloatRect(tl, sz);
	
}

void Torpedo::update(float dt)
{
	m_position += m_speed * dt;
	m_sprite.setPosition(m_position);
	m_explosion_sprite.setPosition(m_position);
	m_timer -= dt;
	
	if (m_triggered)
	{
		m_expl_timer += dt;

		float part = 0.07f;
		if (m_expl_timer < part)
		{
			sf::Vector2f scale = { 50.f, 50.f };
			m_explosion_sprite.setScale(scale*m_expl_timer);
			std::cout << "Start\n";
			
		}
		else
		{
			sf::Vector2f scale = { 20.f, 20.f };
			float delta = 0.3f - m_expl_timer;
			if (delta < 0.f)
			{
				std::cout << "Done\n";
				delta = 0.f;
				m_dead = true;
			}
			m_explosion_sprite.setScale(scale*delta);
		}
		
		
	}

	if (m_timer < 0)
	{
		trigger(0);
	}
}

void Torpedo::setPosition(sf::Vector2f pos)
{
	m_position = pos;
	m_sprite.setPosition(pos);
	
}

void Torpedo::launch(sf::Vector2f position, sf::Vector2f dir, sf::Vector2f base_speed, bool firendly)
{
	m_speed = dir * 200.f;// +base_speed;
	m_position = position;
	m_sprite.setPosition(m_position);
	m_sprite.setOrigin(16.f, 8.f);

	float norm = std::sqrtf(m_speed.x*m_speed.x + m_speed.y*m_speed.y);


	float ang = 180.f / 3.1415f*std::atan2f(m_speed.y/norm, m_speed.x / norm);
	m_sprite.setRotation(ang+180.f);

	m_friendly = firendly;
	m_active = true;
	m_timer = 5.f;
	m_triggered = false;
	m_exploded = false;
}

bool Torpedo::isTriggered()
{
	return m_triggered;
}

void Torpedo::trigger(int cause)
{
	if (!m_triggered)
	{
		m_trigger_cause = cause;
		m_triggered = true;
		m_expl_timer = 0.f;
		m_speed = sf::Vector2f(0.f, 0.f);
	}
}
bool Torpedo::isDead()
{
	return m_dead;
}

bool Torpedo::hasExploded()
{
	return m_exploded;
}
int Torpedo::explode()
{
	//m_expl_timer = 0.f;
	m_exploded = true;
	return m_trigger_cause;
}

bool Torpedo::isFriendly()
{
	return m_friendly;
}