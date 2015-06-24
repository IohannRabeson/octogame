#include "Tile.hpp"
#include <cassert>

Tile::Tile(void) :
	m_transitionType(e_transition_none),
	m_startColor(sf::Color::White),
	m_upLeft(nullptr),
	m_noiseValue(0.f),
	m_isEmpty(true)
{}
