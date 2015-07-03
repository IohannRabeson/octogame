#include "Moon.hpp"
#include "ABiome.hpp"

Moon::Moon(void) :
	m_animator(1.f, 0.f, 4.f, 0.1f),
	m_animation(1.f)
{
}

void Moon::createMoon(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	float unit = size.x / 3.f;

	sf::Vector2f upLeft(-unit * 2.f, -size.y);
	sf::Vector2f upRight(-unit, -size.y);
	sf::Vector2f quarterUpLeft(-size.x, -size.y + unit);
	sf::Vector2f quarterUpRight(-unit * 2.f, -size.y + unit);
	sf::Vector2f upMidLeft(-unit, -unit);
	sf::Vector2f upMidRight(unit, -unit);
	sf::Vector2f downMidLeft(-unit, unit);
	sf::Vector2f downMidRight(unit, unit);
	sf::Vector2f quarterDownLeft(-size.x, size.y - unit);
	sf::Vector2f quarterDownRight(-unit * 2.f, size.y - unit);
	sf::Vector2f downLeft(-unit * 2.f, size.y);
	sf::Vector2f downRight(-unit, size.y);

	upLeft += origin;
	upRight += origin;
	quarterUpLeft += origin;
	quarterUpRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downMidLeft += origin;
	downMidRight += origin;
	quarterDownLeft += origin;
	quarterDownRight += origin;
	downLeft += origin;
	downRight += origin;

	builder.createQuad(upLeft, upRight, quarterUpRight, quarterUpLeft, color);
	builder.createQuad(upRight, upMidRight, upMidLeft, quarterUpRight, color);
	builder.createQuad(upMidRight, downMidRight, downMidLeft, upMidLeft, color);
	builder.createQuad(downLeft, downRight, quarterDownRight, quarterDownLeft, color);
	builder.createQuad(downRight, downMidRight, downMidLeft, quarterDownRight, color);
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
