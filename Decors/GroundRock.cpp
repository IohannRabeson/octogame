#include "GroundRock.hpp"
#include "ABiome.hpp"
#include "Tile.hpp"
#include "Progress.hpp"
#include <Camera.hpp>

GroundRock::GroundRock(bool onInstance) :
	m_type(0u),
	m_deep(0.f),
	m_onInstance(onInstance)
{
}

void GroundRock::createGroundRock(std::size_t type, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(0.f, 0.f);
	sf::Vector2f upRight(Tile::TileSize, 0.f);
	sf::Vector2f downLeft(0.f, Tile::TileSize);
	sf::Vector2f downRight(Tile::TileSize, Tile::TileSize);

	upLeft += origin;
	upRight += origin;
	downLeft += origin;
	downRight += origin;

	if (type == 0u)
		builder.createTriangle(upLeft, upRight, downLeft, color);
	else if (type == 1u)
		builder.createTriangle(upLeft, upRight, downRight, color);
	else if (type == 2u)
		builder.createTriangle(upLeft, downLeft, downRight, color);
	else if (type == 3u)
		builder.createTriangle(upRight, downRight, downLeft, color);

}

void GroundRock::setup(ABiome& biome)
{
	m_type = biome.randomInt(0u, 4u);
	if (m_onInstance == false)
	{
		//TODO: Add to biome
		if (biome.getId() != Level::Final)
			m_deep = biome.randomFloat(16.f, octo::Camera().getSize().y);
		else
			m_deep = biome.randomFloat(16.f, 16 * 15.f);
	}
	else
		m_deep = 0.f;
	m_color = biome.getRockColor();
}

void GroundRock::update(sf::Time, octo::VertexBuilder& builder, ABiome& biome)
{
	sf::Vector2f position = getPosition();
	position.y -= Tile::TileSize;
	//TODO:: Add to biome
	if (biome.getId() != Level::IceC)
		createGroundRock(m_type, position + sf::Vector2f(0.f, m_deep), m_color, builder);
}
