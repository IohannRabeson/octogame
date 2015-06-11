#include "Circle.hpp"

Circle::Circle(void) :
	AShape(),
	m_rotatedCenter(),
	m_center(),
	m_globalBounds(),
	m_radius(0.f)
{ }

sf::Rect<float> const & Circle::getGlobalBounds(void)
{
	m_globalBounds.left = getCenter().x - m_radius;
	m_globalBounds.top = getCenter().y - m_radius;
	m_globalBounds.width = 2.f * m_radius;
	m_globalBounds.height = 2.f * m_radius;
	return m_globalBounds;
}

void Circle::rotate(float angle)
{
	float cos = std::cos(angle);
	float sin = std::sin(angle);
	float x;
	float y;
	sf::Vector2f const & origin = getOrigin();
	x = m_rotatedCenter.x - origin.x;
	y = m_rotatedCenter.y - origin.y;
	m_rotatedCenter.x = origin.x + (x * cos - y * sin);
	m_rotatedCenter.y = origin.y + (x * sin + y * cos);
}

sf::Vector2f const & Circle::getCenter(void)
{
	m_center = m_rotatedCenter + getPosition();
	m_center.x += m_radius;
	m_center.y += m_radius;
	return m_center;
}

// TODO: move debug draw Only debug
void drawCrosss(sf::RenderTarget & render, sf::Vector2f const & position, sf::Color const & color)
{
	sf::Vector2f sizeX(10.f, 0.f);
	sf::Vector2f sizeY(0.f, 10.f);

	sf::Vertex line[] =
	{
		sf::Vertex(position - sizeX, color),
		sf::Vertex(position + sizeX, color),
		sf::Vertex(position - sizeY, color),
		sf::Vertex(position + sizeY, color),
	};

	render.draw(line, 4, sf::Lines);
}

void Circle::debugDraw(sf::RenderTarget & render)
{
	sf::CircleShape shape(m_radius);
	shape.setPointCount(100u);
	sf::Vector2f position = getCenter();
	position.x -= m_radius;
	position.y -= m_radius;
	shape.setPosition(position);
	render.draw(shape);

	sf::RectangleShape rect;
	rect.setOutlineColor(sf::Color::Red);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(1);
	rect.setPosition(sf::Vector2f(getGlobalBounds().left, getGlobalBounds().top));
	rect.setSize(sf::Vector2f(getGlobalBounds().width, getGlobalBounds().height));
	render.draw(rect);

	sf::Vector2f ori = getOrigin() + getPosition();
	ori.x += m_radius;
	ori.y += m_radius;
	drawCrosss(render, ori, sf::Color::Red);
	drawCrosss(render, getPosition(), sf::Color::Magenta);
	drawCrosss(render, getCenter(), sf::Color::Green);
}
