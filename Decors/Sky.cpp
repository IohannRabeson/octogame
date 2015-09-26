#include "Sky.hpp"
#include "ABiome.hpp"
#include "SkyCycle.hpp"
#include <Application.hpp>
#include <Camera.hpp>

Sky::Sky(void) :
	m_cycle(nullptr)
{
}

Sky::Sky(SkyCycle * cycle) :
	m_cycle(cycle)
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

void Sky::setup(ABiome&)
{
	m_colorUp = m_cycle->getSkyColorUp();
	m_colorDown = m_cycle->getSkyColorDown();
}

void Sky::update(sf::Time, octo::VertexBuilder& builder, ABiome&)
{
	sf::Vector2f const & position = getPosition();
	sf::Vector2f const & cameraSize = octo::Application::getCamera().getSize();
	m_colorUp = m_cycle->getSkyColorUp();
	m_colorDown = m_cycle->getSkyColorDown();
	createSky(cameraSize, position, m_colorUp, m_colorDown, builder);
}
