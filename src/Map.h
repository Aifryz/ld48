#pragma once

#include "SFML/Graphics.hpp"
#include <random>


constexpr int MAP_WIDTH = 256;
constexpr int MAP_HEIGHT = 1024;

class Map;

class Tile
{
public:
	Tile(Map& parent, int x, int y);
	void destroy();
	sf::FloatRect getBBox();
	sf::Vector2f getCenter();
private:
	Map& m_map;
	int m_x;
	int m_y;
};

class CollideableTilesIterator
{
public:
	CollideableTilesIterator(Map& parent, sf::IntRect range, int x, int y);
	bool operator ==(const CollideableTilesIterator& other);
	bool operator !=(const CollideableTilesIterator& other);
	CollideableTilesIterator operator ++();
	Tile operator*();
private:
	Map& m_map;
	const sf::IntRect m_range;
	int m_next_x;
	int m_next_y;
};



class CollideableTiles
{
public:
	CollideableTiles(Map& parent, sf::IntRect range);
	CollideableTilesIterator begin();
	CollideableTilesIterator end();
private:
	Map& m_map;
	const sf::IntRect m_range;
};

class Map: public sf::Drawable
{
public:
	enum TileState
	{
		SPACE,
		WALL,
		NEW_SPACE,
		NEW_WALL
	};
	void create(sf::Texture* tiles);

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void destroyTile(int x, int y);

	///Returns iterator to collideable tiles in range
	CollideableTiles getCollideableTiles(sf::FloatRect rect);
	bool isTileCollideable(int x, int y) const;

private:
	std::vector<TileState> m_tile_status;
	sf::VertexBuffer m_vbuf;
	sf::Texture* m_tiles;

};