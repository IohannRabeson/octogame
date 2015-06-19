#include "Tile.hpp"
#include "ConvexShape.hpp"
#include <cassert>

Tile::Tile(void) :
	mf_noiseValue(0.f),
	mb_isEmpty(true),
	me_transition(e_transition_none),
	mp_upLeft(nullptr),
	m_startColor(sf::Color::White)
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
}
