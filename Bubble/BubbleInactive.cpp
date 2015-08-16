#include "BubbleInactive.hpp"

#include <Application.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

BubbleInactive::BubbleInactive(void)
{
}

void BubbleInactive::createExtension(sf::Vector2f const & position, octo::VertexBuilder & builder)
{
	sf::Vector2f rightUp(m_sizeCorner, 0.f);
	sf::Vector2f down(m_sizeCorner, m_sizeCorner);

	rightUp = rightUp + position;
	down = down + position;

	builder.createTriangle(position, rightUp, down, ABubble::getColor());
}

void BubbleInactive::setup(std::string const & phrase, sf::Color const & color)
{
	(void)phrase;
	ABubble::setColor(color);
	m_size = sf::Vector2f(m_sizeCorner, m_sizeCorner);
}

void BubbleInactive::createQuotePart(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder & builder)
{
	sf::Vector2f leftUp(-size.x, -size.y);
	sf::Vector2f rightUp(size.x, -size.y);
	sf::Vector2f leftDown(-size.x, size.y);
	sf::Vector2f rightDown(size.x, size.y);
	sf::Vector2f leftUpTriangle(0.f, size.y);
	sf::Vector2f downTriangle(0.f, size.y + size.y);

	leftUp += origin;
	rightUp += origin;
	leftDown += origin;
	rightDown += origin;
	leftUpTriangle += origin;
	downTriangle += origin;

	builder.createQuad(leftUp, rightUp, rightDown, leftDown, color);
	builder.createTriangle(rightDown, leftUpTriangle, downTriangle, color);
}

void BubbleInactive::update(sf::Time, octo::VertexBuilder& builder)
{
	sf::Vector2f position = ABubble::getPosition();
	position.y -= (m_size.y / 2.f + m_sizeCorner * 2.f);
	ABubble::createOctogon(m_size / 2.f, m_sizeCorner, position, ABubble::getColor(), true, builder);
	createQuotePart(m_size / 4.f, position - sf::Vector2f(m_size.x, 0.f), sf::Color(0, 0, 0), builder);
	createQuotePart(m_size / 4.f, position + sf::Vector2f(0.f, 0.f), sf::Color(0, 0, 0), builder);
}

sf::Text BubbleInactive::getText(void) const
{
	return m_text;
}

