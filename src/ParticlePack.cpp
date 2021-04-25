#include "ParticlePack.h"
#include <random>

ParticlePack::ParticlePack() :
	m_put(0)
{
	m_positions.resize(256);
	m_scales.resize(256);
	m_vertices = sf::VertexArray(sf::Quads, 256 * 4);
}

ParticlePack::ParticlePack(sf::Texture* tex) :
	m_put(0),
	m_texture(tex)
{
	m_positions.resize(256);
	m_scales.resize(256);
	m_speeds.resize(256);
	m_vertices = sf::VertexArray(sf::Quads, 256 * 4);
}



//void addParticle(sf::Vector2f pos);
void ParticlePack::addRandomParticle(sf::Vector2f base_pos, sf::Vector2f base_speed, sf::Vector2f pos_variance, sf::Vector2f speed_variance, float size_variance, int type)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::normal_distribution distrib(0.f, 1.f);

	std::uniform_int_distribution select(0, 3);

	float rscale = distrib(gen);
	float rspeedx = distrib(gen);
	float rspeedy = distrib(gen);
	float rpos = distrib(gen);

	sf::Vector2f pos = base_pos + rpos * pos_variance;
	sf::Vector2f speed = base_speed + sf::Vector2f{ rspeedx*speed_variance.x,rspeedy*speed_variance.y };
	sf::Vector2f scale = sf::Vector2f{1.f, 1.f} + rscale * size_variance*sf::Vector2f(1.f, 1.f);

	m_scales[m_put] = scale;
	m_positions[m_put] = pos;
	m_speeds[m_put] = speed;

	int sel = select(gen);
	sf::Vector2f rowpos = sf::Vector2f{ 0.f, 0.f };

	if (type == 0) // bubbles
	{
		rowpos += sf::Vector2f{ 0.f, 0.f };
		rowpos += sf::Vector2f{ 16.f, 0.f }*((float)sel);
	}
	else if(type == 1) // rocks
	{
		rowpos += sf::Vector2f{ 0.f, 16.f };
		rowpos += sf::Vector2f{ 16.f, 0.f }*((float)sel);
	}
	else if (type == 2) // enemy fragmens
	{
		rowpos += sf::Vector2f{ 0.f, 32.f };
		rowpos += sf::Vector2f{ 16.f, 0.f }*((float)sel);
	}
	else // player fragments
	{
		rowpos += sf::Vector2f{ 0.f, 48.f };
		rowpos += sf::Vector2f{ 16.f, 0.f }*((float)sel);
	}

	sf::Vector2f sz = sf::Vector2f{ 16.f, 16.f }*rscale*size_variance;

	m_vertices[m_put * 4 + 0].position = pos + sf::Vector2f(-sz.x / 2.f, -sz.y / 2.f);//tl
	m_vertices[m_put * 4 + 1].position = pos + sf::Vector2f(sz.x / 2.f, -sz.y / 2.f);//tr
	m_vertices[m_put * 4 + 2].position = pos + sf::Vector2f(sz.x / 2.f, sz.y / 2.f);//br
	m_vertices[m_put * 4 + 3].position = pos + sf::Vector2f(-sz.x / 2.f, sz.y / 2.f);//bl

	m_vertices[m_put * 4 + 0].texCoords = rowpos + sf::Vector2f{ 0.f, 0.f };
	m_vertices[m_put * 4 + 1].texCoords = rowpos + sf::Vector2f{ 16.f, 0.f };
	m_vertices[m_put * 4 + 2].texCoords = rowpos + sf::Vector2f{ 16.f, 16.f };
	m_vertices[m_put * 4 + 3].texCoords = rowpos + sf::Vector2f{ 0.f, 16.f };

	m_put += 1;
	if (m_put >= 256)
	{
		m_put = 0;
	}
}

void ParticlePack::update(float dt)
{
	for (int i = 0; i < 256; i++)
	{
		sf::Vector2f pos = m_positions[i] + m_speeds[i] * dt;
		m_positions[i] = pos;

		//Update vertice positions

		sf::Vector2f sz = sf::Vector2f{ 16.f, 16.f }*m_scales[i].x;

		m_vertices[i * 4 + 0].position = pos + sf::Vector2f(-sz.x / 2.f, -sz.y / 2.f);//tl
		m_vertices[i * 4 + 1].position = pos + sf::Vector2f(sz.x / 2.f, -sz.y / 2.f);//tr
		m_vertices[i * 4 + 2].position = pos + sf::Vector2f(sz.x / 2.f, sz.y / 2.f);//br
		m_vertices[i * 4 + 3].position = pos + sf::Vector2f(-sz.x / 2.f, sz.y / 2.f);//bl
	}
}

void ParticlePack::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.texture = m_texture;
	target.draw(m_vertices, states);
}