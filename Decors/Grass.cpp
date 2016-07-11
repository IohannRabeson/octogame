#include "Grass.hpp"
#include "ABiome.hpp"
#include "Tile.hpp"
#include "Progress.hpp"
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <Math.hpp>

Grass::Grass(bool) :
	m_animator(0.5f, 14.f, 35.f, 0.1f),
	m_animation(1.f),
	m_animationSpeed(1.f)
{
}

void Grass::createGrass(sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(0.f, 0.f);
	sf::Vector2f upRight(Tile::TileSize, 0.f);
	sf::Vector2f downLeft(0.f, Tile::TileSize * 2.f);
	sf::Vector2f downRight(Tile::TileSize, Tile::TileSize * 2.f);
	sf::Vector2f up = octo::linearInterpolation(upLeft, upRight, m_animation);

	up += origin;
	downLeft += origin;
	downRight += origin;

	builder.createTriangle(up, downRight, downLeft, color);
}

void Grass::setup(ABiome& biome)
{
	m_color = biome.getTileStartColor();
	m_animator.setup(biome.getMushroomLifeTime());
}

void Grass::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	sf::Vector2f position = getPosition();
	sf::Vector2f const & octoPosition = Progress::getInstance().getOctoPos();
	float dist = std::sqrt(std::pow(position.x - octoPosition.x, 2u) + std::pow(position.y - octoPosition.y, 2u));
	if (dist <= 100.f)
		m_animationSpeed = 1.f + (1.f - dist / 100.f) * 10.f;
	else if (m_animationSpeed >= 1.f)
		m_animationSpeed -= frameTime.asSeconds();


	m_animator.update(frameTime * m_animationSpeed);
	m_animation = m_animator.getAnimation();
	position.y -= Tile::TileSize * 2.f;
	//TODO:: Add to biome
	if (Progress::getInstance().getCurrentDestination() != Level::IceC)
		createGrass(position, m_color, builder);
}
