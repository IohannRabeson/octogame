#include "Sky.hpp"
#include "ABiome.hpp"
#include "GameClock.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <Camera.hpp>

//TODO: Get gameclock in constructor?
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
	(void)biome;
	//TODO: Get colors from biome
	m_colorUpDay = sf::Color(188, 200, 206);//biome.getSkyColorUp();
	m_colorUpNight = sf::Color(8, 20, 26);
	m_colorDownDay = sf::Color(255, 255, 255);//biome.getSkyColorDown();
	m_colorDownNight = sf::Color(50, 50, 50);//biome.getSkyColorDown();
}

void Sky::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	(void)frameTime;
	sf::Vector2f const & position = getPosition();
	sf::Vector2f cameraSize = octo::Application::getCamera().getSize();
	float interpolateValue = m_clock->getNightValue() * 2.f >= 1.f ? 1.f : m_clock->getNightValue() * 2.f;
	sf::Color colorUp = octo::linearInterpolation(m_colorUpDay, m_colorUpNight, interpolateValue);
	sf::Color colorDown = octo::linearInterpolation(m_colorDownDay, m_colorDownNight, interpolateValue);
	createSky(cameraSize, position, colorUp, colorDown, builder);
}
