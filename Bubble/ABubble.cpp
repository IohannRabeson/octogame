#include "ABubble.hpp"

ABubble::ABubble(void)
{
}

void ABubble::createOctogon(sf::Vector2f const & size, float sizeCorner, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x, -size.y - sizeCorner);
	sf::Vector2f upRight(size.x, -size.y - sizeCorner);
	sf::Vector2f upMidLeft(-size.x - sizeCorner, -size.y);
	sf::Vector2f upMidRight(size.x + sizeCorner, -size.y);
	sf::Vector2f downLeft(-size.x, size.y + sizeCorner);
	sf::Vector2f downRight(size.x, size.y + sizeCorner);
	sf::Vector2f downMidLeft(-size.x - sizeCorner, size.y);
	sf::Vector2f downMidRight(size.x + sizeCorner, size.y);

	upLeft += origin;
	upRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downLeft += origin;
	downRight += origin;
	downMidLeft += origin;
	downMidRight += origin;

	builder.createTriangle(origin, upLeft, upRight, color);
	builder.createTriangle(origin, upRight, upMidRight, color);
	builder.createTriangle(origin, upMidRight, downMidRight, color);
	builder.createTriangle(origin, downMidRight, downRight, color);
	builder.createTriangle(origin, downRight, downLeft, color);
	builder.createTriangle(origin, downLeft, downMidLeft, color);
	builder.createTriangle(origin, downMidLeft, upMidLeft, color);
	builder.createTriangle(origin, upMidLeft, upLeft, color);

	m_textUpLeft = sf::Vector2f(-size.x, -size.y) + origin;
	createExtension(downLeft, builder);
}

sf::Vector2f ABubble::getTextUpLeft(void) const
{
	return m_textUpLeft;
}

sf::Vector2f ABubble::getPosition(void) const
{
	return m_position;
}

void ABubble::setPosition(sf::Vector2f const & position)
{
	m_position = position;
}

