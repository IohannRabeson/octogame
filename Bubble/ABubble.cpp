#include "ABubble.hpp"

ABubble::ABubble(void) :
	m_vertices(new sf::Vertex[100]),
	m_count(100),
	m_used(0u),
	m_currentType(Type::None),
	m_lastType(Type::None),
	m_priority(Priority::Bullshit),
	m_color(255, 255, 255, 200),
	m_isActive(false)
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

void ABubble::createExtensionSpeak(sf::Vector2f const & position, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f leftUp(-m_sizeCorner, -m_sizeCorner);
	sf::Vector2f rightUp(0.f, -m_sizeCorner);
	sf::Vector2f down(0.f, 0.f);

	leftUp += position;
	rightUp += position;
	down += position;

	builder.createTriangle(leftUp, rightUp, down, color);
}

void ABubble::createExtensionThink(sf::Vector2f const & position, sf::Color const & color, octo::VertexBuilder& builder)
{
	float tierCorner = m_sizeCorner / 4.f;
	sf::Vector2f size(tierCorner, tierCorner);
	createOctogon(size, tierCorner, position, color, builder);
}

void ABubble::createExtension(sf::Vector2f const & position, sf::Color const & color, Type type, octo::VertexBuilder& builder)
{
	if (type == Type::Speak)
		createExtensionSpeak(position, color, builder);
	else if (type != Type::None && type != Type::Menu && type != Type::MainMenu)
		createExtensionThink(position, color, builder);
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
}

void ABubble::createQuotePart(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder & builder)
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

void ABubble::createSquare(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder & builder)
{
	sf::Vector2f leftUp(-size.x, -size.y);
	sf::Vector2f rightUp(size.x, -size.y);
	sf::Vector2f leftDown(-size.x, size.y);
	sf::Vector2f rightDown(size.x, size.y);

	leftUp += origin;
	rightUp += origin;
	leftDown += origin;
	rightDown += origin;

	builder.createQuad(leftUp, rightUp, rightDown, leftDown, color);
}

void ABubble::createRectangle(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder & builder)
{
	sf::Vector2f leftUp(-size.x, -size.y * 4.f);
	sf::Vector2f rightUp(size.x, -size.y * 4.f);
	sf::Vector2f leftDown(-size.x, size.y);
	sf::Vector2f rightDown(size.x, size.y);

	leftUp += origin;
	rightUp += origin;
	leftDown += origin;
	rightDown += origin;

	builder.createQuad(leftUp, rightUp, rightDown, leftDown, color);
}

void ABubble::createInactiveLogo(sf::Vector2f const & size)
{
	switch (m_priority)
	{
		case Priority::Tips:
		{
			createQuotePart(size / 2.f, m_positionBubble - sf::Vector2f(size.x, 0.f), sf::Color(75, 75, 75), m_builder);
			createQuotePart(size / 2.f, m_positionBubble + sf::Vector2f(size.x, 0.f), sf::Color(75, 75, 75), m_builder);
			break;
		}
		case Priority::Bullshit:
		{
			createSquare(size / 3.f, m_positionBubble, sf::Color(150, 150, 150), m_builder);
			createSquare(size / 3.f, m_positionBubble - sf::Vector2f(size.x, 0.f), sf::Color(150, 150, 150), m_builder);
			createSquare(size / 3.f, m_positionBubble + sf::Vector2f(size.x, 0.f), sf::Color(150, 150, 150), m_builder);
			break;
		}
		case Priority::Important:
		{
			createRectangle(size / 3.f, m_positionBubble, sf::Color(0, 0, 0), m_builder);
			createSquare(size / 3.f, m_positionBubble + sf::Vector2f(0.f, size.y), sf::Color(0, 0, 0), m_builder);
			break;
		}
		default:
			break;
	}
}

void ABubble::computePositionBubble(Type type, sf::Vector2f const & position)
{
	if (type == Type::Speak || type == Type::Think)
	{
		if (m_isActive)
		{
			if (m_positionBubble.x >= position.x + m_size.x / 2.f - m_sizeCorner)
				m_positionBubble.x = position.x - m_sizeCorner + m_size.x / 2.f;
			else if (m_positionBubble.x <= position.x - m_size.x / 2.f)
				m_positionBubble.x = position.x - m_size.x / 2.f;
			m_positionBubble.y = position.y - m_sizeCorner * 2.f - m_size.y / 2.f;
		}
		else
			m_positionBubble = sf::Vector2f(position.x - m_sizeCorner / 2.f, position.y - m_sizeCorner * 2.f - m_sizeCorner / 2.f);
	}
	else if (type == Type::Right)
		m_positionBubble = position + sf::Vector2f(m_sizeCorner * 2.f + m_size.x / 2.f, 0.f);
	else if (type == Type::Left)
		m_positionBubble = position - sf::Vector2f(m_sizeCorner * 2.f + m_size.x / 2.f, 0.f);
	else if (type == Type::Up || type == Type::Menu)
		m_positionBubble = position - sf::Vector2f(m_size.x / 2.f, m_sizeCorner * 2.f + m_size.y / 2.f);
	else if (type == Type::MainMenu)
		m_positionBubble = position + sf::Vector2f(m_size.x / 2.f, m_size.y / 2.f);
}

void ABubble::update(sf::Time frameTime)
{
	m_builder.clear();
	if (m_currentType != Type::None)
	{
		createExtension(m_position, m_color, m_currentType, m_builder);
		m_size = getContentSize();
		computePositionBubble(m_currentType, m_position);
		m_contentUpLeft = m_positionBubble - m_size / 2.f;
		updateContent(frameTime, m_contentUpLeft);
		if (m_isActive)
		{
			createOctogon(m_size / 2.f, m_sizeCorner, m_positionBubble, m_color, m_builder);
		}
		else
		{
			sf::Vector2f sizeInactive(m_sizeCorner / 2.f, m_sizeCorner / 2.f);
			createOctogon(sizeInactive, m_sizeCorner, m_positionBubble, m_color, m_builder);
			createInactiveLogo(sizeInactive);
		}
	}
	m_used = m_builder.getUsed();
	m_lastType = m_currentType;
}

void ABubble::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	//TODO: Find the problem insteas of avoiding it
	if (m_currentType != Type::None && m_lastType == m_currentType)
	{
		render.draw(m_vertices.get(), m_used, sf::Triangles, states);
		if (m_isActive)
			drawContent(render, states);
	}
}

sf::Vector2f const & ABubble::getContentUpLeft(void) const
{
	return m_contentUpLeft;
}

sf::Vector2f const & ABubble::getPosition(void) const
{
	return m_position;
}

sf::Color const & ABubble::getColor(void) const
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

void ABubble::setType(Type type)
{
	m_currentType = type;
}

void ABubble::setPriority(Priority priority)
{
	m_priority = priority;
}

void ABubble::setActive(bool isActive)
{
	m_isActive = isActive;
}
