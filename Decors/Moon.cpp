#include "Moon.hpp"
#include "ABiome.hpp"

Moon::Moon(void) :
	m_animator(1.f, 0.f, 4.f, 0.2f),
	m_animation(1.f)
{
}

void Moon::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x + sizeCorner.x, -size.y);
	sf::Vector2f upRight(size.x - sizeCorner.x, -size.y);
	sf::Vector2f cornerUpLeft(-size.x + sizeCorner.x, -size.y + sizeCorner.y);
	sf::Vector2f cornerUpRight(size.x - sizeCorner.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidLeft(-size.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidRight(size.x, -size.y + sizeCorner.y);
	sf::Vector2f downLeft(-size.x + sizeCorner.x, size.y);
	sf::Vector2f downRight(size.x - sizeCorner.x, size.y);
	sf::Vector2f cornerDownLeft(-size.x + sizeCorner.x, size.y - sizeCorner.y);
	sf::Vector2f cornerDownRight(size.x - sizeCorner.x, size.y - sizeCorner.y);
	sf::Vector2f downMidLeft(-size.x, size.y - sizeCorner.y);
	sf::Vector2f downMidRight(size.x, size.y - sizeCorner.y);

	upLeft += origin;
	upRight += origin;
	cornerUpLeft += origin;
	cornerUpRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downLeft += origin;
	downRight += origin;
	cornerDownLeft += origin;
	cornerDownRight += origin;
	downMidLeft += origin;
	downMidRight += origin;

	builder.createTriangle(upLeft, cornerUpLeft, upMidLeft, color);
	builder.createTriangle(upRight, cornerUpRight, upMidRight, color);
	builder.createTriangle(downLeft, cornerDownLeft, downMidLeft, color);
	builder.createTriangle(downRight, cornerDownRight, downMidRight, color);

	builder.createQuad(upLeft, upRight, cornerUpRight, cornerUpLeft, color);
	builder.createQuad(cornerUpLeft, cornerUpRight, cornerDownRight, cornerDownLeft, color);
	builder.createQuad(cornerDownLeft, cornerDownRight, downRight, downLeft, color);
	builder.createQuad(upMidLeft, cornerUpLeft, cornerDownLeft, downMidLeft, color);
	builder.createQuad(cornerUpRight, upMidRight, downMidRight, cornerDownRight, color);
}

void Moon::createMoon(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	createOctogon(size, size / 2.f, origin, color, builder);
	sf::Color darkSideColor(0, 0, 0, 150);
	createOctogon(sf::Vector2f(size.x / 2.f, size.y), size / 2.f, origin + sf::Vector2f(size.x / 2.f, 0.f), darkSideColor, builder);
}

void Moon::setup(ABiome& biome)
{
	m_size = biome.getMoonSize();
	m_color = biome.getMoonColor();
	m_animator.setup();
}

void Moon::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();

	createMoon(m_size * m_animation, position, m_color, builder);
}
