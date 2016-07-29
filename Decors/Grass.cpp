#include "Grass.hpp"
#include "ABiome.hpp"
#include "Tile.hpp"
#include "Progress.hpp"
#include <Camera.hpp>
#include <Interpolations.hpp>
#include <Math.hpp>

Grass::Grass(bool) :
	m_isDeadlyGrass(false),
	m_animator(1.f, 5.f, 1.f, 0.3f, 1.f),
	m_animation(0.f),
	m_animationSpeed(1.f),
	m_movementTimerMax(sf::seconds(0.5f)),
	m_numberOfTargets(10u),
	m_sideTarget(false),
	m_indexLeftTarget(0u),
	m_indexRightTarget(0u)
{
}

void Grass::createGrass(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f downLeft(0.f, 0.f);
	sf::Vector2f downRight(size.x, 0.f);
	sf::Vector2f downMid(size.x / 2.f, 0.f);
	sf::Vector2f upLeft(-size.x, -size.y);
	sf::Vector2f upRight(size.x * 2.f, -size.y);

	if (!m_sideTarget)
		m_up = octo::cosinusInterpolation(m_leftTargets[m_indexLeftTarget], m_rightTargets[m_indexRightTarget], m_movementTimer / m_movementTimerMax);
	else
		m_up = octo::cosinusInterpolation(m_rightTargets[m_indexRightTarget], m_leftTargets[m_indexLeftTarget], m_movementTimer / m_movementTimerMax);

	downLeft += origin;
	downRight += origin;
	downMid += origin;
	m_up.y *= m_animation;
	m_up += origin;

	if (!m_isDeadlyGrass)
		builder.createTriangle(m_up, downRight, downLeft, color);
	else
	{
		builder.createTriangle(m_up, downLeft, downMid, color + sf::Color(10, 10, 10, 0));
		builder.createTriangle(m_up, downRight, downMid, color);
	}
}

void Grass::setup(ABiome& biome)
{
	m_size = sf::Vector2f(Tile::TileSize, biome.getGrassSizeY());
	m_color = biome.getGrassColor();
	m_colorNormal = biome.getGrassColor();
	m_colorDeadly = biome.getSkyDayColor();
	m_isDeadlyGrass = biome.isDeadlyGrass();
	if (!m_isDeadlyGrass)
		m_animator.setup(biome.getMushroomLifeTime());
	else
		//TODO: Find a better way to do that
		m_animator.setup(sf::seconds(100000.f));

	m_leftTargets.resize(m_numberOfTargets);
	m_rightTargets.resize(m_numberOfTargets);
	for (std::size_t i = 0u; i < m_numberOfTargets; i++)
	{
		m_leftTargets[i] = sf::Vector2f(-m_size.x * ((i + 1) / m_numberOfTargets), -m_size.y);
		m_rightTargets[i] = sf::Vector2f(m_size.x + m_size.x * ((i + 1) / m_numberOfTargets), -m_size.y);
	}

	if (m_isDeadlyGrass)
	{
		m_shine.setSize(biome.getShineEffectSize() / 4.f);
		m_shine.setup(biome);
	}
}

void Grass::computeMovement(sf::Time frameTime)
{
	Progress & progress = Progress::getInstance();
	sf::Vector2f const & octoPosition = progress.getOctoPos();
	float dist = std::sqrt(std::pow(m_up.x - octoPosition.x, 2u) + std::pow(m_up.y - octoPosition.y, 2u));

	if (m_isDeadlyGrass && dist < 600.f)
		m_color = octo::cosinusInterpolation(m_colorDeadly, m_colorNormal, dist / 600.f);

	if (dist <= 60.f && m_lastOctoPosition.x != octoPosition.x)
	{
		if (m_isDeadlyGrass && (m_up.x - octoPosition.x > -16.f && m_up.x - octoPosition.x < 16.f))
			progress.setKillOcto(true);
		m_animationSpeed = 1.f + (dist / 60.f);
	}
	else if (m_animationSpeed >= 0.2f)
		m_animationSpeed -= frameTime.asSeconds();

	if (m_animation != 0.f)
		m_movementTimer += frameTime * m_animationSpeed;

	if (m_movementTimer > m_movementTimerMax)
	{
		m_sideTarget = !m_sideTarget;
		if (dist <= 60.f && m_lastOctoPosition.x != octoPosition.x)
		{
			if (octoPosition.x < m_lastOctoPosition.x)
				m_indexLeftTarget = m_numberOfTargets - 1;
			else if (octoPosition.x > m_lastOctoPosition.x)
				m_indexRightTarget = m_numberOfTargets - 1;
		}
		else
		{
			if (m_sideTarget && m_indexLeftTarget > 0u)
				m_indexLeftTarget--;
			else if (!m_sideTarget && m_indexRightTarget > 0u)
				m_indexRightTarget--;
		}
		m_movementTimer = sf::Time::Zero;
	}
	m_lastOctoPosition = octoPosition;
}

void Grass::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome & biome)
{
	sf::Vector2f const & position = getPosition();

	computeMovement(frameTime);
	m_animator.update(frameTime * m_animationSpeed);
	m_animation = m_animator.getAnimation();

	createGrass(m_size, position, m_color, builder);

	if (m_animation > 0.f && m_isDeadlyGrass)
	{
		m_shine.setPosition(m_up);
		m_shine.update(frameTime, builder, biome);
	}
}
