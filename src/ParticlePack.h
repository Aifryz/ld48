#pragma once

#include "SFML/Graphics.hpp"

class ParticlePack : public sf::Drawable
{
public:
	ParticlePack();
	ParticlePack(sf::Texture* tex);
	//void addParticle(sf::Vector2f pos);
	void addRandomParticle(sf::Vector2f base_pos, sf::Vector2f base_speed, sf::Vector2f pos_variance, sf::Vector2f speed_variance, float size_variance, int type = 0);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void update(float dt);
private:
	int m_put;
	sf::Texture* m_texture;
	std::vector<sf::Vector2f> m_positions;
	std::vector<sf::Vector2f> m_scales;
	std::vector<sf::Vector2f> m_speeds;
	sf::VertexArray m_vertices;
};