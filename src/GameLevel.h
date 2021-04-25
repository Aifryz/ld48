#pragma once

#include"Level.h"

#include "SFML/Graphics.hpp"
#include <unordered_map>
#include "Player.h"
#include "Map.h"
#include "Treasure.h"
#include "Torpedo.h"
#include "ParticlePack.h"
#include "Enemy.h"

class GameLevel : public Level
{
public:
	GameLevel(std::unordered_map<std::string, sf::Texture>& textures);

	virtual void handleEvent(sf::Event e);
	virtual void update(float dt);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void generateTreasures();

	void handlePlayerTileCollisions();
	void handlePlayerTreasureCollisions();
	void handleTorpedoTileCollisions();
	void handleEnemyTileCollisions();

	void spawnTorpedo(sf::Vector2f origin, sf::Vector2f dir, bool friendly);
	void explodeTorpedo(sf::Vector2f pos, int cause);

	void spawnEnemy();

	void updateUI();

private:
	std::unordered_map<std::string, sf::Texture>& m_textures;
	Player m_player;
	sf::Sprite m_grad;
	Map m_map;
	std::vector<Treasure> m_treasures;
	std::vector<Torpedo> m_torpedoes;
	std::vector<Enemy> m_enemies;
	
	sf::View m_camera;
	sf::Font m_font;

	sf::Text m_torpedoes_text;
	sf::Text m_depth_text;
	sf::Text m_score_text;
	sf::Text m_life_text;

	int m_score;

	float m_player_bubble_timer;

	float m_enemy_spawn_timer;

	ParticlePack m_particles;
};