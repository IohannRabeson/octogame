#include "RectangleShape.hpp"

RectangleShape::RectangleShape(void) :
	PolygonShape(),
	m_size()
{
	setVertexCount(4u);
}

void RectangleShape::setSize(sf::Vector2f const & size)
{
	m_size = size;
	setVertex(0u, sf::Vector2f(0.f, 0.f));
	setVertex(1u, sf::Vector2f(size.x, 0.f));
	setVertex(2u, size);
	setVertex(3u, sf::Vector2f(0.f, size.y));
}
