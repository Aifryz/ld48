#include "Map.h"
#include <iostream>

static int countWalls(std::vector<Map::TileState> &states,int x, int y)
{
	const int N8X[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	const int N8Y[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int w = 0;
	for (int j = 0; j < 8; j++)
	{
		int nx = x + N8X[j];
		int ny = y + N8Y[j];
		if (nx >= MAP_WIDTH || nx < 0)
		{
			continue;
		}
		if (ny >= MAP_HEIGHT || ny < 0)
		{
			continue;
		}

		int i = ny * MAP_WIDTH + nx;
		if (states[i] == Map::WALL || states[i] == Map::NEW_SPACE)
		{
			w += 1;
		}
	}
	return w;
}


Tile::Tile(Map& parent, int x, int y):
	m_map(parent),
	m_x(x),
	m_y(y)
{

}
void Tile::destroy()
{
	m_map.destroyTile(m_x, m_y);
}
sf::FloatRect Tile::getBBox()
{
	sf::Vector2f size = { 32.f, 32.f };
	sf::Vector2f pos = sf::Vector2f(m_x, m_y)*32.f;
	return sf::FloatRect(pos, size);
}

sf::Vector2f Tile::getCenter()
{
	sf::Vector2f size = { 32.f, 32.f };
	sf::Vector2f pos = sf::Vector2f(m_x, m_y)*32.f;
	return pos + size * 0.5f;
}

void Map::create(sf::Texture* tiles)
{
	const int WALL_COEFF = 52;
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, 100);
	m_tiles = tiles;

	m_tile_status.resize(MAP_WIDTH * MAP_HEIGHT);


	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int i = y * MAP_WIDTH + x;
				auto num = distrib(gen);
			if (num > WALL_COEFF)
			{
				m_tile_status[i] = WALL;
			}
			else
			{
				m_tile_status[i] = SPACE;
			}
		}
	}


	for (int step = 0; step < 10; step++)
	{
		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				int i = y * MAP_WIDTH + x;
				int w = countWalls(m_tile_status, x, y);
				if (w > 4 && m_tile_status[i] == Map::SPACE)
				{
					m_tile_status[i] = Map::NEW_WALL;
				}

				if (w <= 3 && m_tile_status[i] == Map::WALL)
				{
					m_tile_status[i] = Map::NEW_SPACE;
				}
			}
		}

		for (int y = 0; y < MAP_HEIGHT; y++)
		{
			for (int x = 0; x < MAP_WIDTH; x++)
			{
				int i = y * MAP_WIDTH + x;
					if (m_tile_status[i] == NEW_WALL)
					{
						m_tile_status[i] = WALL;

				}
					if (m_tile_status[i] == NEW_SPACE)
					{
						m_tile_status[i] = SPACE;

					}
			}
		}
		
	}
	

	//Create vertex buffer with stuff
	m_vbuf.create(MAP_WIDTH * MAP_HEIGHT * 4);
	m_vbuf.setPrimitiveType(sf::Quads);
	std::vector<sf::Vertex> verts(MAP_WIDTH * MAP_HEIGHT * 4);

	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			int i = (y * MAP_WIDTH + x);
			int i4 = i * 4;
			sf::Vector2f tile_off;
			if (m_tile_status[i] == Map::WALL)
			{
				tile_off = { 32.f, 0.f };
			}
			sf::Vector2f vpos = sf::Vector2f(x * 32.f, y * 32.f);
			//Top left
			verts[i4 + 0].texCoords = sf::Vector2f(0.f,0.f)+tile_off;
			verts[i4 + 0].position = sf::Vector2f(0,0)+vpos;
			//Top right
			verts[i4 + 1].texCoords = sf::Vector2f(32.f, 0.f) + tile_off;
			verts[i4 + 1].position = sf::Vector2f(32.f, 0) + vpos;
			//Bottom right
			verts[i4 + 2].texCoords = sf::Vector2f(32.f, 32.f) + tile_off;
			verts[i4 + 2].position = sf::Vector2f(32.f, 32.f) + vpos;
			//Bottom left
			verts[i4 + 3].texCoords = sf::Vector2f(0.f, 32.f) + tile_off;
			verts[i4 + 3].position = sf::Vector2f(0, 32.f) + vpos;
		}
	}
	m_vbuf.update(verts.data());

}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = m_tiles;
	target.draw(m_vbuf, states);
}


CollideableTiles Map::getCollideableTiles(sf::FloatRect rect)
{
	//Determine topleft tile index
	//Determine bottomright tile index,
	//Create iterator

	//Assume no negative coords for tiles

	if (rect.left < 0 || rect.top < 0)
	{
		return CollideableTiles(*this, sf::IntRect(0, 0, 1, 1));
	}

	float top = rect.top / 32.f;
	float left = rect.left / 32.f;

	float bot = (rect.top + rect.height) / 32.f;
	float right = (rect.left + rect.width) / 32.f;

	//Simple downcast is ok
	int top_idx = top;
	int left_idx = left;
	int bot_idx = bot+1;
	int right_idx = right+1;

	return CollideableTiles(*this, sf::IntRect(left_idx, top_idx, right_idx -left_idx,  bot_idx-top_idx));

	
}


bool CollideableTilesIterator::operator ==(const CollideableTilesIterator& other)
{
	return m_next_x == other.m_next_x && m_next_y == other.m_next_y;
	}
bool CollideableTilesIterator::operator !=(const CollideableTilesIterator& other)
{
	return m_next_x != other.m_next_x || m_next_y != other.m_next_y;
	}
CollideableTilesIterator CollideableTilesIterator::operator ++()
{
	//Increment iterator, unitl collideable tile is found
	while (true)
	{
		m_next_x += 1;

		if (m_next_x >= m_range.left + m_range.width)
		{
			m_next_y += 1;
			if (m_next_y >= m_range.top + m_range.height)
			{
				break;
			}
			else
			{
				m_next_x = m_range.left;
			}
		}

		//If tile is collideable
		if (m_map.isTileCollideable(m_next_x, m_next_y))
		{
			break;
		}
	}


	return *this;
	}
Tile CollideableTilesIterator::operator*()
{
	return Tile(m_map, m_next_x, m_next_y);
}

CollideableTiles::CollideableTiles(Map& parent, sf::IntRect range):
	m_map(parent),
	m_range(range)
{

	}
CollideableTilesIterator CollideableTiles::begin()
{
	return CollideableTilesIterator(m_map, m_range, m_range.left, m_range.top);
	}
CollideableTilesIterator CollideableTiles::end()
{
	return CollideableTilesIterator(m_map, m_range, m_range.left+m_range.width, m_range.top + m_range.height);

	}

CollideableTilesIterator::CollideableTilesIterator(Map& parent, sf::IntRect range, int x, int y):
	m_map(parent),
	m_range(range),
	m_next_x(x),
	m_next_y(y)
{
	if (m_map.isTileCollideable(m_next_x, m_next_y))
	{

	}
	else if(x != m_range.left+m_range.width && y != m_range.top + m_range.height) // If not end iterator

	{
		++*this; // Increment iterator to get next collideable tile
	}
}

bool Map::isTileCollideable(int x, int y) const
{
	int i = y * MAP_WIDTH + x;
	if (m_tile_status[i] == Map::WALL)
	{
		return true;
	}
	return false;
}

void Map::destroyTile(int x, int y)
{
	int i = y * MAP_WIDTH +x;
	if (m_tile_status[i] == Map::WALL)
	{
		m_tile_status[i] = Map::SPACE;
		//Update vertbuf
		sf::Vertex verts[4];
		sf::Vector2f tile_off;
		sf::Vector2f vpos = sf::Vector2f(x * 32.f, y * 32.f);
		//Top left
		verts[0].texCoords = sf::Vector2f(0.f, 0.f) + tile_off;
		verts[0].position = sf::Vector2f(0, 0) + vpos;
		//Top right
		verts[1].texCoords = sf::Vector2f(32.f, 0.f) + tile_off;
		verts[1].position = sf::Vector2f(32.f, 0) + vpos;
		//Bottom right
		verts[2].texCoords = sf::Vector2f(32.f, 32.f) + tile_off;
		verts[2].position = sf::Vector2f(32.f, 32.f) + vpos;
		//Bottom left
		verts[3].texCoords = sf::Vector2f(0.f, 32.f) + tile_off;
		verts[3].position = sf::Vector2f(0, 32.f) + vpos;

		m_vbuf.update(verts, 4, i * 4);

	}
}