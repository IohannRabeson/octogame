#include "Sky.hpp"
#include "ABiome.hpp"
#include "GameClock.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <Camera.hpp>

Sky::Sky(void) :
	m_clock(nullptr)
{
}

Sky::Sky(GameClock * clock) :
	m_clock(clock)
{
}

void Sky::createSky(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & colorUp, sf::Color const & colorDown, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x, -size.y);
	sf::Vector2f upRight(size.x, -size.y);
	sf::Vector2f downLeft(-size.x, size.y);
	sf::Vector2f downRight(size.x, size.y);

	upLeft += origin;
	upRight += origin;
	downLeft += origin;
	downRight += origin;

	builder.createVertex(upLeft, colorUp);
	builder.createVertex(upRight, colorUp);
	builder.createVertex(downRight, colorDown);
	builder.createVertex(upLeft, colorUp);
	builder.createVertex(downLeft, colorDown);
	builder.createVertex(downRight, colorDown);
}

void Sky::setup(ABiome& biome)
{
	m_colorUpDay = biome.getSkyDayColor();
	m_colorUpNight = biome.getSkyNightColor();
	m_colorDownDay = sf::Color(255, 255, 255);
	m_colorDownNight = sf::Color(50, 50, 50);
}

void Sky::update(sf::Time, octo::VertexBuilder& builder, ABiome&)
{
	sf::Vector2f const & position = getPosition();
	sf::Vector2f cameraSize = octo::Application::getCamera().getSize();
	float interpolateValue = m_clock->getNightValue() * 2.f >= 1.f ? 1.f : m_clock->getNightValue() * 2.f;
	sf::Color colorUp = octo::linearInterpolation(m_colorUpDay, m_colorUpNight, interpolateValue);
	sf::Color colorDown = octo::linearInterpolation(m_colorDownDay, m_colorDownNight, interpolateValue);
	createSky(cameraSize, position, colorUp, colorDown, builder);
}
