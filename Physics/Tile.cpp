#include "Tile.hpp"

Tile::Tile(void) :
	Polygon(4u, CollideType::e_tile),
	mf_noiseValue(0.f),
	mb_isEmpty(true),
	me_transition(e_transition_none),
	mp_upLeft(nullptr),
	m_startColor(sf::Color::White)
{}

Tile::~Tile(void) {}

//TODO: optimize
int getIndex(int i)
{
	switch (i)
	{
		case 1:
			return 4;
		case 2:
			return 5;
		case 4:
			return 2;
	}
	return i;
}

void Tile::computeEdges(void)
{
	if (!m_recompute)
		return;
	m_recompute = false;
	for (std::size_t i = 0u; i < 4u; i++)
	{
		m_edges[i].x = mp_upLeft[getIndex(i)].position.x - mp_upLeft[getIndex((i + 1u) % 4u)].position.x;
		m_edges[i].y = mp_upLeft[getIndex(i)].position.y - mp_upLeft[getIndex((i + 1u) % 4u)].position.y;
		m_normals[i].x = -m_edges[i].y;
		m_normals[i].y = m_edges[i].x;
	}
}

void Tile::computeCenter(void)
{
	if (!m_recomputeCenter)
		return;
	m_recomputeCenter = false;
	m_center.x = 0.f;
	m_center.y = 0.f;
	std::size_t nbVertice = 0u;
	for (std::size_t i = 0u; i < 4u; i++)
	{
		if (mp_upLeft[i].position == mp_upLeft[getIndex((i + 1u) % 4u)].position)
			continue;
		m_center.x += mp_upLeft[getIndex(i)].position.x;
		m_center.y += mp_upLeft[getIndex(i)].position.y;
		nbVertice += 1u;
	}
	m_center.x /= nbVertice;
	m_center.y /= nbVertice;
}

sf::Vector2f const & Tile::getVertex(std::size_t index) const
{
	return mp_upLeft[getIndex(index)].position;
}
