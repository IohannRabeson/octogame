#include "Tile.hpp"

Tile::Tile(void) :
	m_transitionType(e_transition_none),
	m_startColor(sf::Color::White),
	m_isEmpty(true),
	m_tileType(octo::LevelMap::TileType::Square)
{}
