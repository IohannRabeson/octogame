#include "NanoEffect.hpp"

NanoEffect::NanoEffect(void) :
	m_vertices(new sf::Vertex[100]),
	m_count(100),
	m_used(0u),
	m_size(80.f, 80.f),
	m_color(255, 255, 255, 200),
	m_isActive(true),
	m_isTransfer(false),
	m_glowingTimerMax(sf::seconds(2.f))
{
	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
}

void NanoEffect::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f up(0.f, -size.y);
	sf::Vector2f down(0.f, size.y);
	sf::Vector2f left(-size.x, 0.f);
	sf::Vector2f right(size.x, 0.f);

	up += origin;
	down += origin;
	left += origin;
	right += origin;

	builder.createTriangle(left, up, right, color);
	builder.createTriangle(left, down, right, color);
}

void NanoEffect::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color color, octo::VertexBuilder& builder)
{
	float glowingCoef = m_glowingTimer / m_glowingTimerMax;

	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
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
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
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
