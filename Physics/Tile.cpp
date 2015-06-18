#include "Tile.hpp"
#include "ConvexShape.hpp"
#include <cassert>

Tile::Tile(void) :
	mf_noiseValue(0.f),
	mb_isEmpty(true),
	me_transition(e_transition_none),
	mp_upLeft(nullptr),
	m_startColor(sf::Color::White),
	m_shape(nullptr)
{}

//TODO: use an array
int getIndex(int i)
{
	switch (i)
	{
		case 1:
			return 4;
		case 2:
			return 5;
		case 3:
			return 2;
	}
	return i;
}

void Tile::setPolygonVertices(sf::Vertex * vertex)
{
	mp_upLeft = vertex;
	assert(m_shape != nullptr && m_shape->getVertexCount() == 4u);
	m_shape->setVertex(0u, vertex[getIndex(0u)].position);
	m_shape->setVertex(1u, vertex[getIndex(1u)].position);
	m_shape->setVertex(2u, vertex[getIndex(2u)].position);
	m_shape->setVertex(3u, vertex[getIndex(3u)].position);
}
