#include "Rainbow.hpp"
#include "ABiome.hpp"
#include <Math.hpp>

Rainbow::Rainbow(void) :
	m_partCount(1u),
	m_animator(1.f, 0.f, 4.f, 0.1f),
	m_animation(1.f)
{
}

void Rainbow::createRainbowPart(sf::Vector2f const & size, sf::Vector2f const & origin, std::vector<sf::Vector2f> & start, std::vector<sf::Vector2f> & end, std::size_t stripeCount, octo::VertexBuilder& builder)
{
	float stripeSize = size.y / stripeCount;

	for (std::size_t i = 0; i < stripeCount; i++)
	{
		start[i] = sf::Vector2f(0.f, stripeSize * (i + 1));
		end[i] = start[i];
		rotateVec(end[i], m_cos, m_sin);

	}
	for (std::size_t i = 0; i < stripeCount - 1; i++)
	{
		builder.createQuad(start[i] + origin, start[i + 1] + origin, end[i + 1] + origin, end[i] + origin, sf::Color(i*20 + 100,i*10 + 15,i*10 + 2));
	}
}

/*
void Rainbow::createRainbow(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, std::size_t partCount, sf::Color color, octo::VertexBuilder& builder)
{
}
*/
void Rainbow::setup(ABiome& biome)
{
	m_size = biome.getRainbowSize();
	m_partCount = biome.getRainbowPartCount();
	m_stripeCount = 8u;
	m_cos = std::cos(-90.f * octo::Deg2Rad);
	m_sin = std::sin(-90.f * octo::Deg2Rad);

	m_rainbow.resize(m_partCount);
	for (std::size_t i = 0; i < m_partCount; i++)
		m_rainbow[i].resize(m_stripeCount);

	m_animator.setup();
}

void Rainbow::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();
	createRainbowPart(m_size, position, m_rainbow[0], m_rainbow[1], m_stripeCount, builder);
	//createRainbow(m_size * m_animation, m_sizeCorner * m_animation, position, m_partCount, m_color, builder);
}

void Rainbow::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}

