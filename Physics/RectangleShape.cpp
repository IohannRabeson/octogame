#include "RectangleShape.hpp"

RectangleShape::RectangleShape(void) :
	PolygonShape(),
	m_size()
{
	setVertexCount(4u);
}

void RectangleShape::setSize(float width, float height)
{
	m_size.x = width;
	m_size.y = height;
	setVertex(0u, sf::Vector2f(0.f, 0.f));
	setVertex(1u, sf::Vector2f(width, 0.f));
	setVertex(2u, m_size);
	setVertex(3u, sf::Vector2f(0.f, height));
}
