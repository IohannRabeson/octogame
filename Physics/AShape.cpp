#include "AShape.hpp"
#include <limits>

AShape::AShape(void) :
	m_velocity(),
	m_origin(),
	m_position(),
	m_rotation(0.f),
	m_sleep(false),
	m_applyGravity(true),
	m_outOfScreen(false),
	m_type(AShape::Type::e_dynamic),
	m_collisionType(0u),
	m_collisionMask(std::numeric_limits<std::uint32_t>::max())
{ }

void AShape::update(void)
{
	move(m_velocity);
	m_velocity.x = 0.f;
	m_velocity.y = 0.f;
	computeShape();
}

void AShape::drawCross(sf::RenderTarget & render, sf::Vector2f const & position, sf::Color const & color)
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

void AShape::debugDraw(sf::RenderTarget & render)
{
	sf::Rect<float> const & bounds = getGlobalBounds();
	sf::RectangleShape rect;
	rect.setOutlineColor(sf::Color::Red);
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(1);
	rect.setPosition(sf::Vector2f(bounds.left, bounds.top));
	rect.setSize(sf::Vector2f(bounds.width, bounds.height));
	render.draw(rect);

	sf::Vector2f ori = getOrigin() + getPosition();
	drawCross(render, ori, sf::Color::Red);
	drawCross(render, getPosition(), sf::Color::Magenta);
	drawCross(render, getBaryCenter(), sf::Color::Green);
}
