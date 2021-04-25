#include "Enemy.h"

Enemy::Enemy(const sf::Texture& tex)
{
	m_sprite = sf::Sprite(tex);
	m_position = sf::Vector2f(0.f, 0.f);
	m_speed = sf::Vector2f(0.f, 0.f);

	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);

	m_health = 1000;
	m_dead = false;
	m_torpedo_timer = 0.f;
	}
	Enemy::Enemy() {
		m_torpedo_timer = 0.f;
	}
	void Enemy::update(float dt) {

		sf::Vector2f new_acc;
		auto diff = m_target-m_position ;
		
		auto norm = std::sqrtf(diff.x*diff.x + diff.y*diff.y);

		if (norm > 2000.f)
		{
			m_dead = true; // Target to far away, despawn
		}

		if (m_health < 0)
		{
			//Play explosion animation, then die
			m_dead = true;
		}

		if (norm > 0.f)
		{
			diff = diff * 1.f / norm;

			//diff.x = diff.x*diff.x*diff.x;
			//diff.y = diff.y*diff.y*diff.y;

			new_acc = diff;
		}
		if (norm > 300)
		{
			m_acc = new_acc * 200.f;
		}
		else if (norm > 100)
		{
			m_acc = new_acc * 100.f;
		}
		else if (norm > 50)
		{
			m_acc = new_acc * 50.f;
		}


		
		m_speed += m_acc * dt;

		//Clip speed
		std::clamp(m_speed.x, -100.f, 100.f);
		std::clamp(m_speed.y, -100.f, 100.f);

		//Simulate drag
		auto rev = dt * m_speed * -0.01f;
		m_speed += rev;

		if (m_speed.x > 0)
		{
			m_sprite.setScale(-1.f, 1.f);
		}
		else
		{
			m_sprite.setScale(1.f, 1.f);
		}

		if (m_torpedo_timer > 1.f)
		{
			m_torpedo_timer = 0.f;
		}

		//Torpedo targetting
		m_torpedo_timer += dt;
		m_torpedo_dir = diff;
		m_target_ranged = false;

		if (norm < 300.f && norm > 100.f)
		{
			m_target_ranged = true;
		}
		

		m_position = m_position + m_speed * dt;
		m_sprite.setPosition(m_position);
	}
	void Enemy::draw(sf::RenderTarget &target, sf::RenderStates states) const {
		target.draw(m_sprite, states);
	}

	sf::Vector2f Enemy::getPosition() const {
		return m_position;
	}
	sf::FloatRect Enemy::getBBox() const {
		sf::Vector2f sz{ 64.f, 16.f };
		sf::Vector2f tl = m_position - sz * 0.5f;

		return sf::FloatRect(tl, sz); // HACK, smaller collision
	}

	void Enemy::handleTileColision(sf::Vector2f colision_vector) {
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
			float damage = fabs(m_speed.y);
			m_health -= damage;
			m_speed.y = -BOUNCE_FACTOR * m_speed.y;
		}
	}

	sf::Vector2f Enemy::getSpeed() {
		return m_speed;
	}
	int Enemy::getHealth() {
		return m_health;
	}
	bool Enemy::isAccelerating() {
		return m_acc != sf::Vector2f{ 0.f, 0.f };
	}
	void Enemy::setPosition(sf::Vector2f pos) {
		m_position = pos;
		m_sprite.setPosition(pos);
	}
	void Enemy::damage(int damage) {
		m_health -= damage;
	}

	void Enemy::setTarget(sf::Vector2f tgt)
	{
		m_target = tgt;
	}

	bool Enemy::isDead()
	{
		return m_dead;
	}

	bool Enemy::canShoot() const
	{
		return (m_torpedo_timer > 1.f && m_target_ranged);
	}
	sf::Vector2f Enemy::getTorpedoDir() const
	{
		return m_torpedo_dir;
	}