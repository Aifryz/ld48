#include "Game.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include "Map.h"

GameLevel::GameLevel(std::unordered_map<std::string, sf::Texture>& tex):
	m_textures(tex)
{
	std::cout << "Creating level \n";
	m_player = Player(m_textures["sub.png"]);
	m_player.setPosition(sf::Vector2f{ 128.f*32.f, 0.f });
	m_grad = sf::Sprite(m_textures["grad.png"]);

	std::cout << "Creating map \n";
	m_map = Map();
	m_map.create(&m_textures["tiles.png"]);

	std::cout << "Map created \n";

	m_particles = ParticlePack(&m_textures["particles.png"]);

	//m_particles.addRandomParticle({ 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f }, { 0.f, 0.f }, 1.f);

	//Generate treasures

	generateTreasures();


	m_font.loadFromFile("res/fonts/type_wrong/typwrng.ttf");

	m_torpedoes_text = sf::Text("torpedoes: 45", m_font, 15);
	m_torpedoes_text.setPosition(0, 0);

	m_depth_text = sf::Text("Depth: 45", m_font, 15);
	m_depth_text.setPosition(0, 20);

	//m_life_text = sf::Text("Max Depth: 45", m_font);
	m_score_text = sf::Text("Score: 45", m_font, 15);
	m_score_text.setPosition(0, 40);

	m_life_text = sf::Text("Life: 45", m_font, 15);
	m_life_text.setPosition(0, 60);

	m_player_bubble_timer = 0.f;

	m_camera = sf::View();

	m_enemy_spawn_timer = 0.f;

	m_score = 0;


}

void GameLevel::update(float dt)
{
	m_player.update(dt);

	if (m_player.isDead() && m_state != Level::OVER)
	{
		m_state = Level::OVER;
		//explodeTorpedo(m_player.getPosition(), 3);
		spawnTorpedo(m_player.getPosition(), sf::Vector2f{}, true);
		m_torpedoes.back().trigger(3);
	}

	float depth = m_player.getPosition().y/3.2f;

	if (depth > 1000)
	{
		m_enemy_spawn_timer += dt;
		
	}

	if (depth > 5000 && m_enemy_spawn_timer > 2.f)
	{
		m_enemy_spawn_timer = 0.f;
		spawnEnemy();
	}
	else if (depth > 3000 && m_enemy_spawn_timer > 5.f)
	{
		m_enemy_spawn_timer = 0.f;
		spawnEnemy();
	}
	else if (depth > 1000 && m_enemy_spawn_timer > 10.f)
	{
		m_enemy_spawn_timer = 0.f;
		spawnEnemy();
	}



	auto target = m_player.getPosition();
	for (Enemy& e : m_enemies)
	{
		e.setTarget(target);
		e.update(dt);
		if (e.getHealth() < 0)
		{
			spawnTorpedo(e.getPosition(), sf::Vector2f{}, true);
			m_torpedoes.back().trigger(2);
		}

		if (e.canShoot())
		{
			spawnTorpedo(e.getPosition(), e.getTorpedoDir()*2.f, false); // Spawn not friendly torpedo
		}
		
	}

	for (Torpedo& t : m_torpedoes)
	{
		t.update(dt);

		if (t.isTriggered() && !t.hasExploded()) // Explode and remove
		{
			int cause = t.explode();
			sf::Vector2f pos = t.getPosition();
			explodeTorpedo(pos, cause);
		}

		if (t.isFriendly())
		{
			for (Enemy& e : m_enemies)
			{
				sf::Vector2f pos = e.getPosition();
				auto diff = pos - t.getPosition();
				auto norm = std::sqrtf(diff.x*diff.x + diff.y*diff.y);
				if (norm < 30.f)
				{
					t.trigger(2);//Triger from enemy
				}

			}
		}
		else // Check player colision
		{
			sf::Vector2f pos = m_player.getPosition();
			auto diff = pos - t.getPosition();
			auto norm = std::sqrtf(diff.x*diff.x + diff.y*diff.y);
			if (norm < 30.f)
			{
				t.trigger(0);//Trigger without fragments
			}
		}
	}

	//Remove dead torpedoes
	auto tit = std::remove_if(m_torpedoes.begin(), m_torpedoes.end(), [](auto t) {return t.isDead(); });
	m_torpedoes.erase(tit, m_torpedoes.end());
	//Remove dead enemies
	auto eit = std::remove_if(m_enemies.begin(), m_enemies.end(), [](auto e) {return e.isDead(); });
	m_enemies.erase(eit, m_enemies.end());

	m_particles.update(dt);

	//Handle player colision with rocks

	handlePlayerTileCollisions();
	handleEnemyTileCollisions();
	handlePlayerTreasureCollisions();
	handleTorpedoTileCollisions();

	m_player_bubble_timer += dt;
	if (m_player_bubble_timer > 0.2f)
	{
		m_player_bubble_timer = 0.f;
		if (m_player.isAccelerating() && !m_player.isDead())
		{
			m_particles.addRandomParticle(m_player.getPosition(), { 0.f, -100.f }, { 0.f, 0.f }, { 15.f, 15.f }, 0.25f);
		}
	}


		m_camera.setCenter(m_player.getPosition());
		m_camera.setSize(800, 600);
		

		updateUI();


}
void GameLevel::handleEnemyTileCollisions()
{
	for (Enemy& e : m_enemies)
	{
		auto enemy_bbox = e.getBBox();

		sf::Vector2f total;

		sf::Vector2f player_pos = m_player.getPosition();

		float maxx = 0.f;
		float maxy = 0.f;
		for (auto tile : m_map.getCollideableTiles(enemy_bbox))
		{
			auto rect = tile.getBBox();
			sf::FloatRect intersect{};
			if (enemy_bbox.intersects(rect, intersect))
			{
				maxx = std::max(maxx, fabs(intersect.width));
				maxy = std::max(maxy, fabs(intersect.height));
				sf::Vector2f x = sf::Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 2) - player_pos;
				total += x;
			}
		}

		if (maxx != 0.f && maxy != 0.f)
		{
			if (maxx < maxy) // Vertical wall hit
			{
				uint32_t sdads = 0;
				e.handleTileColision(sf::Vector2f(maxx, 0));
			}
			else // Horizontal wall hit
			{
				e.handleTileColision(sf::Vector2f(0, maxy));
			}
		}
	}
}
void GameLevel::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		int mx = event.mouseButton.x - 400;
		int my = event.mouseButton.y - 300;

		float dx = mx;
		float dy = my;

		float norm = sqrtf(dx * dx + dy * dy);

		sf::Vector2f dir{ dx / norm, dy / norm };

		if (m_player.takeTorpedo())
		{
			spawnTorpedo(m_player.getPosition(), dir, true); // Spawn friendly torpedo
		}
	}
}

void GameLevel::explodeTorpedo(sf::Vector2f pos, int cause)
{
	const float RADIUS = 90.f;
	const float SQRAD = RADIUS * RADIUS;
	sf::Vector2f sz = { RADIUS , RADIUS };
	sf::FloatRect bbox = sf::FloatRect(pos - sz * 0.5f, sz);

	for (auto t : m_map.getCollideableTiles(bbox))
	{
		auto tile_pos = t.getCenter();

		auto diff = pos - tile_pos;
		float r2 = diff.x*diff.x + diff.y*diff.y;
		if (r2 < SQRAD)
		{
			t.destroy();
		}

	}

	//Spawn some bubbles

	for (int i = 0; i < 5; i++)
	{
		m_particles.addRandomParticle(pos, { 0.f, -100.f }, { 0.f, 0.f }, { 15.f, 15.f }, 0.5f);
	}

	//Spawn some rubble
	if(cause == 1)
	for (int i = 0; i < 5; i++)
	{
		m_particles.addRandomParticle(pos, { 0.f, 100.f }, { 0.f, 0.f }, { 15.f, 15.f }, 0.5f, 1);
	}

	//Spawn enemy fragments
	if (cause == 2)
		for (int i = 0; i < 5; i++)
		{
			m_particles.addRandomParticle(pos, { 0.f, 100.f }, { 0.f, 0.f }, { 15.f, 15.f }, 0.f, 2);
		}
	//Spawn friendly fragments
	if (cause == 3)
		for (int i = 0; i < 5; i++)
		{
			m_particles.addRandomParticle(pos, { 0.f, 100.f }, { 0.f, 0.f }, { 15.f, 15.f }, 0.f, 3);
		}

	//Do damage to players and enemies in range

	auto ppos = m_player.getPosition();
	auto pt_dist = ppos - pos;
	if (pt_dist.x*pt_dist.x + pt_dist.y*pt_dist.y < SQRAD)
	{
		m_player.damage(100);
	}

	for (Enemy& e : m_enemies)
	{
		auto epos = e.getPosition();
		auto pt_dist = epos - pos;
		if (pt_dist.x*pt_dist.x + pt_dist.y*pt_dist.y < SQRAD)
		{
			e.damage(10000);
		}
	}
}

void GameLevel::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Draw map?
	//Draw player?

	target.clear(sf::Color(5, 80, 120));

	target.setView(m_camera);
	target.draw(m_map);
	target.draw(m_player);

	for (const Enemy& e : m_enemies)
	{
		target.draw(e);
	}

	for (const Treasure& t : m_treasures)
	{
		target.draw(t);
	}
	for (const Torpedo& t : m_torpedoes)
	{
		target.draw(t);
	}

	target.draw(m_particles);

	/*sf::RectangleShape rect{ {16.f, 16.f} };
	sf::FloatRect bbox = m_player.getBBox();
	rect.setPosition(bbox.left, bbox.top);
	rect.setFillColor(sf::Color::Red);
	rect.setSize({ bbox.width, bbox.height });
	target.draw(rect);
	*/

	sf::View overlay_view;
	overlay_view.setCenter(400, 300);
	overlay_view.setSize(800, 600);
	target.setView(overlay_view);
	target.draw(m_grad, sf::BlendMultiply);

	target.draw(m_torpedoes_text);
	target.draw(m_depth_text);
	target.draw(m_score_text);
	target.draw(m_life_text);


}

void GameLevel::generateTreasures()
{
	int start_y = 10;

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> coin_depth_step(1, 3);
	std::uniform_int_distribution<> torp_pack_depth_step(6, 8);
	std::uniform_int_distribution<> pack_depth_step(12, 16);
	std::uniform_int_distribution<> armor_depth_step(24, 48);
	std::uniform_int_distribution<> crate_depth_step(9, 12);
	std::uniform_int_distribution<> trophy_depth_step(24, 48);

	std::uniform_int_distribution<> xpos(2, 253);
	int xcoord = 0;
	//512 coins
	for (int i = 0; i < 512; i++)
	{
		start_y += coin_depth_step(gen);
		
		xcoord = xpos(gen);
		int ynew = start_y;
		while (true)
		{
			if (ynew > 1010)
			{
				std::cout << "[WARN] Generated " << i << " coins \n";
				break;
			}
			if (!m_map.isTileCollideable(xcoord, ynew) && m_map.isTileCollideable(xcoord, ynew + 1))
			{
				//Generate 
				
				m_treasures.push_back(Treasure(&m_textures["pickups.png"], Treasure::COINS));
				m_treasures.back().setPosition(sf::Vector2f{ xcoord*32.f, ynew*32.f }+sf::Vector2f{ 16.f, 16.f });
				break;
			}
			ynew += 1;
		}

	}
	std::cout << "[WARN] Last coin at " << start_y << " coins \n";
	//128  torpedo packs, form top to bottom
	start_y = 10;
	for (int i = 0; i < 128; i++)
	{
		start_y += torp_pack_depth_step(gen);
		
		xcoord = xpos(gen);
		int ynew = start_y;
		while (true)
		{
			if (ynew > 1010)
			{
				std::cout << "[WARN] Generated " << i << " torpedoes \n";
				break;
			}
			if (!m_map.isTileCollideable(xcoord, ynew) && m_map.isTileCollideable(xcoord, ynew + 1))
			{
				//Generate 
				m_treasures.push_back(Treasure(&m_textures["pickups.png"], Treasure::torpedoes));
				m_treasures.back().setPosition(sf::Vector2f{ xcoord*32.f, ynew*32.f }+sf::Vector2f{ 16.f, 16.f });
				break;
			}
			ynew += 1;
		}

	}
	std::cout << "[WARN] Last torp at " << start_y << " coins \n";
	//64 health packs
	start_y = 10;
	for (int i = 0; i < 64; i++)
	{
		start_y += pack_depth_step(gen);
		xcoord = xpos(gen);
		int ynew = start_y;
		while (true)
		{
			if (ynew > 1010)
			{
				std::cout << "[WARN] Generated " << i << " repairs \n";
				break;
			}
			if (!m_map.isTileCollideable(xcoord, ynew) && m_map.isTileCollideable(xcoord, ynew + 1))
			{
				//Generate 
				m_treasures.push_back(Treasure(&m_textures["pickups.png"], Treasure::REPAIR));
				m_treasures.back().setPosition(sf::Vector2f{ xcoord*32.f, ynew*32.f }+sf::Vector2f{ 16.f, 16.f });
				break;
			}
			ynew += 1;
		}

	}
	std::cout << "[WARN] Last health at " << start_y << " \n";

	//16 armor packs
	start_y = 256;
	for (int i = 0; i < 16; i++)
	{
		start_y += armor_depth_step(gen);
		
		xcoord = xpos(gen);
		int ynew = start_y;
		while (true)
		{
			if (ynew > 1010)
			{
				std::cout << "[WARN] Generated " << i << " armors \n";
				break;
			}
			if (!m_map.isTileCollideable(xcoord, ynew) && m_map.isTileCollideable(xcoord, ynew + 1))
			{
				//Generate 
				m_treasures.push_back(Treasure(&m_textures["pickups.png"], Treasure::ARMOR));
				m_treasures.back().setPosition(sf::Vector2f{ xcoord*32.f, ynew*32.f }+sf::Vector2f{ 16.f, 16.f });
				break;
			}
			ynew += 1;
		}

	}
	std::cout << "[WARN] Last armor at " << start_y << " \n";

	//64 chests
	start_y = 256;
	for (int i = 0; i < 64; i++)
	{
		start_y += crate_depth_step(gen);
		xcoord = xpos(gen);
		int ynew = start_y;
		while (true)
		{
			if (ynew > 1010)
			{
				std::cout << "[WARN] Generated " << i << " chests \n";
				break;
			}
			if (!m_map.isTileCollideable(xcoord, ynew) && m_map.isTileCollideable(xcoord, ynew + 1))
			{
				//Generate 
				m_treasures.push_back(Treasure(&m_textures["pickups.png"], Treasure::CHEST));
				m_treasures.back().setPosition(sf::Vector2f{ xcoord*32.f, ynew*32.f }+sf::Vector2f{ 16.f, 16.f });
				break;
			}
			ynew += 1;
		}
	}

	//8 or so treasures, fake that there's only one to make finding it easier
	start_y = 512;
	for (int i = 0; i < 8; i++)
	{
		start_y += trophy_depth_step(gen);
		xcoord = xpos(gen);
		int ynew = start_y;
		while (true)
		{
			if (ynew > 1010)
			{
				std::cout << "[WARN] Generated " << i << " trophies \n";
				break;
			}
			if (!m_map.isTileCollideable(xcoord, ynew) && m_map.isTileCollideable(xcoord, ynew + 1))
			{
				//Generate 
				m_treasures.push_back(Treasure(&m_textures["pickups.png"], Treasure::TROPHY));
				m_treasures.back().setPosition(sf::Vector2f{ xcoord*32.f, ynew*32.f }+sf::Vector2f{ 16.f, 16.f });
				break;
			}
			ynew += 1;
		}
	}

	std::cout << "[WARN] Last trophy at " << start_y << "  \n";






}

void GameLevel::handlePlayerTileCollisions()
{
	auto player_bbox = m_player.getBBox();

	sf::Vector2f total;

	sf::Vector2f player_pos = m_player.getPosition();

	float maxx = 0.f;
	float maxy = 0.f;
	for (auto tile : m_map.getCollideableTiles(player_bbox))
	{
		auto rect = tile.getBBox();
		sf::FloatRect intersect{};
		if (player_bbox.intersects(rect, intersect))
		{
			maxx = std::max(maxx, fabs(intersect.width));
			maxy = std::max(maxy, fabs(intersect.height));
			sf::Vector2f x = sf::Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 2) - player_pos;
			total += x;
		}
	}

	if (maxx != 0.f && maxy != 0.f)
	{
		if (maxx < maxy) // Vertical wall hit
		{
			uint32_t sdads = 0;
			m_player.handleTileColision(sf::Vector2f(maxx, 0));
		}
		else // Horizontal wall hit
		{
			m_player.handleTileColision(sf::Vector2f(0, maxy));
		}
	}
}
void GameLevel::handlePlayerTreasureCollisions()
{
	auto player_bbox = m_player.getBBox();

	for (Treasure& t : m_treasures)
	{
		auto treasure_bbox = t.getBBox();
		if (player_bbox.intersects(treasure_bbox) && !t.isCollected())
		{
			auto tt = t.collect();
			if (tt == Treasure::COINS)
			{
				m_score += 5;
			}
			else if (tt == Treasure::CHEST)
			{
				m_score += 50;
			}
			else if (tt == Treasure::ARMOR)
			{
				m_player.addArmor(1000);
				m_player.repair();
			}
			else if (tt == Treasure::REPAIR)
			{
				m_player.repair();
			}
			else if (tt == Treasure::torpedoes)
			{
				m_player.addtorpedoes(10);
			}
			else if (tt == Treasure::TROPHY)
			{
				if(m_state != Level::OVER)
				{
					m_state = Level::OVER;
				}
			}
		}
	}
}

void GameLevel::spawnTorpedo(sf::Vector2f origin, sf::Vector2f dir, bool friendly)
{
	m_torpedoes.emplace_back(&m_textures["torpedo.png"], &m_textures["explosion.png"]);
	m_torpedoes.back().launch(origin, dir, m_player.getSpeed(), friendly);
}

void GameLevel::handleTorpedoTileCollisions()
{
	for (Torpedo& t : m_torpedoes)
	{
		auto torpedo_bbox = t.getBBox();

		for (auto tile : m_map.getCollideableTiles(torpedo_bbox))
		{
			auto rect = tile.getBBox();
			sf::FloatRect intersect{};
			if (torpedo_bbox.intersects(rect, intersect))
			{
				t.trigger(1);
			}
		}
	}
}

void GameLevel::updateUI()
{
	float depth = m_player.getPosition().y;
	int d = (int)(depth / 3.2f);
	std::string str = "Depth: ";
	str += std::to_string(d);
	m_depth_text.setString(str);

	//Health
	int health = m_player.getHealth() / 10;
	int max_health = m_player.getMaxHealth() / 10;
	str = "Health: ";
	str += std::to_string(health);
	str += " / ";
	str += std::to_string(max_health);
	m_life_text.setString(str);

	//Score

	str = "Score: ";
	str += std::to_string(m_score);
	m_score_text.setString(str);


	//torpedoes
	str = "torpedoes: ";
	str += std::to_string(m_player.getNumOftorpedoes());
	m_torpedoes_text.setString(str);


}


void GameLevel::spawnEnemy()
{
	uint32_t x = 0;
	sf::Vector2f ppos = m_player.getPosition();
	sf::Vector2f vs = sf::Vector2f(800.f, 600.f);
	//DFS to find first free tile,
	sf::Vector2f tl = ppos + sf::Vector2f{ -vs.x / 2.f,-vs.y / 2.f };
	sf::Vector2f tr = ppos + sf::Vector2f{  vs.x / 2.f,-vs.y / 2.f };
	sf::Vector2f br = ppos + sf::Vector2f{  vs.x / 2.f, vs.y / 2.f };
	sf::Vector2f bl = ppos + sf::Vector2f{ -vs.x / 2.f, vs.y / 2.f };

	//Skip first 20 tiles (they are in player vision)
	int range = 10;

	int tx = ppos.x / 32.f;
	int ty = ppos.y / 32.f;

	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dir(0, 1);
	int xdir = dir(gen);
	int ydir = dir(gen);

	if (xdir == 0)
		xdir = -1;
	if (ydir == 0)
		ydir = -1;

	for (int r = range; r < 50; r++)
	{
		int iy_begin = ty - range;
		int iy_end = ty + range;

		if (iy_begin < 1)
		{
			iy_begin = 1;
		}

		if (iy_end > MAP_HEIGHT)
		{
			iy_end = MAP_HEIGHT - 1;
		}

		if (ydir < 0)
		{
			int tmp = iy_end;
			iy_end = iy_begin;
			iy_begin = tmp;
		}
		
		for (int iy = iy_begin; iy != iy_end; iy += ydir)
		{
			int ix_begin = tx - range;
			int ix_end = tx + range;

			if (ix_begin < 1)
			{
				ix_begin = 1;
			}

			if (ix_end > MAP_WIDTH)
			{
				ix_end = MAP_WIDTH - 1;
			}

			if (xdir < 0)
			{
				int tmp = ix_end;
				ix_end = ix_begin;
				ix_begin = tmp;
			}

			for (int ix = ix_begin; ix != ix_end; ix++)
			{
				if (!m_map.isTileCollideable(ix, iy))
				{
					//Valid pos, spawn
					m_enemies.emplace_back<Enemy>(m_textures["bad_sub.png"]);
					m_enemies.back().setPosition(sf::Vector2f{ ix*32.f, iy*32.f });
					return;
				}
			}
		}
		

	}







}