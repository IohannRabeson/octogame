#include "NanoEffect.hpp"

NanoEffect::NanoEffect(void) :
	m_vertices(new sf::Vertex[100]),
	m_count(100),
	m_used(0u),
	m_size(10.f, 10.f),
	m_color(255, 255, 255, 200),
	m_isActive(true),
	m_isTransfer(false),
	m_glowingTimerMax(sf::seconds(2.f))
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

void NanoEffect::setup(octo::AnimatedSprite const & sprite)
{
	m_sprite = sprite;
	m_sprite.setScale(0.f, 0.f);
	m_sprite.setColor(m_color);
}

void NanoEffect::createOctogon(sf::Vector2f const & size, float sizeCorner, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
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

void NanoEffect::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color color, octo::VertexBuilder& builder)
{
	(void)builder;
	float glowingCoef = m_glowingTimer / m_glowingTimerMax;
	//float sizeCorner = size.x;

	m_sprite.setColor(sf::Color::Black);
	color.a = color.a * (1 - glowingCoef);
	m_sprite.setScale(size.x * glowingCoef, size.y * glowingCoef);
	m_sprite.setPosition(origin - m_sprite.getGlobalSize() / 2.f);
	//createOctogon(size * glowingCoef, sizeCorner * glowingCoef, origin, color, builder);
}

void NanoEffect::update(sf::Time frameTime)
{
	m_builder.clear();
	m_glowingTimer += frameTime;
	if (m_glowingTimer >= m_glowingTimerMax)
	{
		if (m_isTransfer == true)
			m_isActive = false;
		m_glowingTimer = sf::Time::Zero;
	}
	if (m_isActive == true)
	{
		createEffect(m_size, m_position, m_color, m_builder);
		if (m_isTransfer == true)
			createEffect(m_size * 2.f, m_position, m_color, m_builder);
	}
	m_used = m_builder.getUsed();
}

void NanoEffect::onTransfer(void)
{
	if (m_isActive == true && m_isTransfer == false)
	{
		m_isTransfer = true;
		m_glowingTimer = sf::Time::Zero;
	}
}

void NanoEffect::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	//render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	(void)states;
	render.draw(m_sprite, sf::BlendAlpha);
}

sf::Vector2f const & NanoEffect::getPosition(void) const
{
	return m_position;
}

sf::Color const & NanoEffect::getColor(void) const
{
	return m_color;
}

bool NanoEffect::isActive(void) const
{
	return m_isActive;
}

void NanoEffect::setPosition(sf::Vector2f const & position)
{
	m_position = position;
}

void NanoEffect::setColor(sf::Color const & color)
{
	m_color = color;
}

void NanoEffect::setActive(bool isActive)
{
	m_isActive = isActive;
}
