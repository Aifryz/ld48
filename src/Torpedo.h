#pragma once
#include "SFML/Graphics.hpp"

class Torpedo : public sf::Drawable
{
public:
	Torpedo();
	Torpedo(sf::Texture* txt, sf::Texture* expl);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	sf::Vector2f getPosition() const;
	sf::FloatRect getBBox() const;
	void update(float dt);
	void setPosition(sf::Vector2f pos);
	void launch(sf::Vector2f position, sf::Vector2f dir, sf::Vector2f base_speed, bool firendly);

	bool isTriggered();
	bool isDead();

	//0 - timer, 1->wall, 2->enemy, 3->player
	void trigger(int cause);

	bool hasExploded();
	int explode();
	bool isFriendly();
private:
	sf::Texture* texture;
	sf::Vector2f m_position;
	sf::Vector2f m_speed;
	sf::Sprite m_sprite;
	sf::Sprite m_explosion_sprite;
	float m_timer;
	float m_expl_timer;
	bool m_active;
	bool m_friendly;
	bool m_triggered;
	bool m_dead;
	bool m_exploded;

	int explosion_phase;

	int m_trigger_cause;
};