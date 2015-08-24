#include "CircleShape.hpp"
#include <cmath>

CircleShape::CircleShape(void) :
	AShape(),
	m_baryCenter(),
	m_initialBaryCenter(),
	m_globalBounds(),
	m_radius(0.f)
{}

void CircleShape::computeShape(void)
{
	m_initialBaryCenter.x = m_radius;
	m_initialBaryCenter.y = m_radius;

	float cos = std::cos(getRotation());
	float sin = std::sin(getRotation());
	float x;
	float y;
	sf::Vector2f const & origin = getOrigin();
	x = m_initialBaryCenter.x - origin.x;
	y = m_initialBaryCenter.y - origin.y;
	m_baryCenter.x = origin.x + (x * cos - y * sin) + getPosition().x;
	m_baryCenter.y = origin.y + (x * sin + y * cos) + getPosition().y;

	m_globalBounds.left = m_baryCenter.x - m_radius;
	m_globalBounds.top = m_baryCenter.y - m_radius;
	m_globalBounds.width = 2.f * m_radius;
	m_globalBounds.height = 2.f * m_radius;
}

void CircleShape::setRadius(float radius)
{
	m_radius = radius;
	computeShape();
}

void CircleShape::debugDraw(sf::RenderTarget & render)
{
	sf::CircleShape shape(m_radius);
	shape.setPointCount(100u);
	sf::Vector2f position = getBaryCenter();
	position.x -= m_radius;
	position.y -= m_radius;
	shape.setPosition(position);
	render.draw(shape);

	AShape::debugDraw(render);
}
