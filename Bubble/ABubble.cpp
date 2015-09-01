#include "ABubble.hpp"

ABubble::ABubble(void) :
	m_vertices(new sf::Vertex[100]),
	m_count(100),
	m_used(0u),
	m_currentType(Type::None),
	m_color(255, 255, 255, 200)
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

void ABubble::createExtension(sf::Vector2f const & position, sf::Color const & color, Type type,  octo::VertexBuilder& builder)
{
	if (type == Type::Speak)
	{
		sf::Vector2f rightUp(m_sizeCorner * 1.5f, 0.f);
		sf::Vector2f down(m_sizeCorner * 1.5f, m_sizeCorner * 1.5f);

		rightUp = rightUp + position;
		down = down + position;
		builder.createTriangle(position, rightUp, down, color);
	}
	else if (type == Type::Think || type == Type::Inactive)
	{
		sf::Vector2f size(m_sizeCorner / 3.f, m_sizeCorner / 3.f);
		createBubble(size, m_sizeCorner / 3.f, position + sf::Vector2f(m_sizeCorner, m_sizeCorner), color, Type::None, builder);
	}
}

void ABubble::createBubble(sf::Vector2f const & size, float sizeCorner, sf::Vector2f origin, sf::Color const & color, Type type, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x, -size.y - sizeCorner);
	sf::Vector2f upRight(size.x, -size.y - sizeCorner);
	sf::Vector2f upMidLeft(-size.x - sizeCorner, -size.y);
	sf::Vector2f upMidRight(size.x + sizeCorner, -size.y);
	sf::Vector2f downLeft(-size.x, size.y + sizeCorner);
	sf::Vector2f downRight(size.x, size.y + sizeCorner);
	sf::Vector2f downMidLeft(-size.x - sizeCorner, size.y);
	sf::Vector2f downMidRight(size.x + sizeCorner, size.y);

	if (type == Type::Think || type == Type::Speak || type == Type::Inactive)
	{
		m_contentUpLeft = sf::Vector2f(-size.x, -size.y);
		sf::Vector2f extensionPos = origin + sf::Vector2f(-sizeCorner, size.y + sizeCorner);
		createExtension(extensionPos, color, type, m_builder);
		if (extensionPos.x <= m_relativePos.x + downLeft.x)
			m_relativePos.x = extensionPos.x + downRight.x;
		else if (extensionPos.x >= m_relativePos.x + downRight.x - sizeCorner)
			m_relativePos.x = extensionPos.x + downLeft.x + sizeCorner;

		origin.x = m_relativePos.x;
		m_contentUpLeft += origin;
	}

	if (type != Type::Inactive)
	{
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
	}
}


void ABubble::update(sf::Time frameTime)
{
	m_builder.clear();
	if (m_currentType != Type::None)
	{
		m_size = getContentSize();
		m_position.y -= (m_size.y / 2.f + m_sizeCorner * 2.f);
		createBubble(m_size / 2.f, m_sizeCorner, m_position, m_color, m_currentType, m_builder);
		updateContent(frameTime, m_contentUpLeft);
	}
	m_used = m_builder.getUsed();
}

void ABubble::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (m_currentType != Type::None)
	{
		render.draw(m_vertices.get(), m_used, sf::Triangles, states);
		drawContent(render, states);
	}
}

sf::Vector2f ABubble::getContentUpLeft(void) const
{
	return m_contentUpLeft;
}

sf::Vector2f ABubble::getPosition(void) const
{
	return m_position;
}

sf::Color ABubble::getColor(void) const
{
	return m_color;
}

ABubble::Type ABubble::getType(void) const
{
	return m_currentType;
}

float ABubble::getSizeCorner(void) const
{
	return m_sizeCorner;
}

void ABubble::setPosition(sf::Vector2f const & position)
{
	m_position = position;
}

void ABubble::setColor(sf::Color const & color)
{
	m_color = color;
}

void ABubble::setType(Type type)
{
	m_currentType = type;
}
