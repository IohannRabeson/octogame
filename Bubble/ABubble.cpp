#include "ABubble.hpp"

ABubble::ABubble(void) :
	m_color(255, 255, 255, 200),
	m_isActive(false)
{
}

void ABubble::createOctogon(sf::Vector2f const & size, float sizeCorner, sf::Vector2f const & origin, sf::Color const & color, bool isExtension, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x, -size.y - sizeCorner);
	sf::Vector2f upRight(size.x, -size.y - sizeCorner);
	sf::Vector2f upMidLeft(-size.x - sizeCorner, -size.y);
	sf::Vector2f upMidRight(size.x + sizeCorner, -size.y);
	sf::Vector2f downLeft(-size.x, size.y + sizeCorner);
	sf::Vector2f downRight(size.x, size.y + sizeCorner);
	sf::Vector2f downMidLeft(-size.x - sizeCorner, size.y);
	sf::Vector2f downMidRight(size.x + sizeCorner, size.y);

	if (isExtension)
	{
		m_textUpLeft = sf::Vector2f(-size.x, -size.y);
		sf::Vector2f extensionPos = origin + sf::Vector2f(-sizeCorner, size.y + sizeCorner);
		createExtension(extensionPos, builder);
		if (extensionPos.x <= m_relativePos.x + downLeft.x)
			m_relativePos.x = extensionPos.x + downRight.x;
		else if (extensionPos.x >= m_relativePos.x + downRight.x - sizeCorner)
			m_relativePos.x = extensionPos.x + downLeft.x + sizeCorner;
		m_relativePos.y = origin.y;
	}
	else
		m_relativePos = origin;

	upLeft += m_relativePos;
	upRight += m_relativePos;
	upMidLeft += m_relativePos;
	upMidRight += m_relativePos;
	downLeft += m_relativePos;
	downRight += m_relativePos;
	downMidLeft += m_relativePos;
	downMidRight += m_relativePos;
	m_textUpLeft += m_relativePos;

	builder.createTriangle(m_relativePos, upLeft, upRight, color);
	builder.createTriangle(m_relativePos, upRight, upMidRight, color);
	builder.createTriangle(m_relativePos, upMidRight, downMidRight, color);
	builder.createTriangle(m_relativePos, downMidRight, downRight, color);
	builder.createTriangle(m_relativePos, downRight, downLeft, color);
	builder.createTriangle(m_relativePos, downLeft, downMidLeft, color);
	builder.createTriangle(m_relativePos, downMidLeft, upMidLeft, color);
	builder.createTriangle(m_relativePos, upMidLeft, upLeft, color);

}

sf::Vector2f ABubble::getTextUpLeft(void) const
{
	return m_textUpLeft;
}

sf::Vector2f ABubble::getPosition(void) const
{
	return m_position;
}

sf::Color ABubble::getColor(void) const
{
	return m_color;
}

bool ABubble::isActive(void) const
{
	return m_isActive;
}

void ABubble::setPosition(sf::Vector2f const & position)
{
	m_position = position;
}

void ABubble::setColor(sf::Color const & color)
{
	m_color = color;
}

void ABubble::setActive(bool isActive)
{
	m_isActive = isActive;
}
