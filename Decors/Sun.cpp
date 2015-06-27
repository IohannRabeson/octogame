#include "Sun.hpp"
#include "ABiome.hpp"

Sun::Sun(void) :
	m_partCount(1u),
	m_animator(1.f, 0.f, 4.f, 0.1f),
	m_animation(1.f),
	m_glowingTimer(sf::Time::Zero),
	m_glowingTimerMax(sf::seconds(3.f))
{
}

void Sun::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f upLeft(-size.x + sizeCorner.x, -size.y);
	sf::Vector2f upRight(size.x - sizeCorner.x, -size.y);
	sf::Vector2f cornerUpLeft(-size.x + sizeCorner.x, -size.y + sizeCorner.y);
	sf::Vector2f cornerUpRight(size.x - sizeCorner.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidLeft(-size.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidRight(size.x, -size.y + sizeCorner.y);
	sf::Vector2f downLeft(-size.x + sizeCorner.x, size.y);
	sf::Vector2f downRight(size.x - sizeCorner.x, size.y);
	sf::Vector2f cornerDownLeft(-size.x + sizeCorner.x, size.y - sizeCorner.y);
	sf::Vector2f cornerDownRight(size.x - sizeCorner.x, size.y - sizeCorner.y);
	sf::Vector2f downMidLeft(-size.x, size.y - sizeCorner.y);
	sf::Vector2f downMidRight(size.x, size.y - sizeCorner.y);

	builder.createTriangle(upLeft + origin, cornerUpLeft + origin, upMidLeft + origin, color);
	builder.createTriangle(upRight + origin, cornerUpRight + origin, upMidRight + origin, color);
	builder.createTriangle(downLeft + origin, cornerDownLeft + origin, downMidLeft + origin, color);
	builder.createTriangle(downRight + origin, cornerDownRight + origin, downMidRight + origin, color);

	builder.createQuad(upLeft + origin, upRight + origin, cornerUpRight + origin, cornerUpLeft + origin, color);
	builder.createQuad(cornerUpLeft + origin, cornerUpRight + origin, cornerDownRight + origin, cornerDownLeft + origin, color);
	builder.createQuad(cornerDownLeft + origin, cornerDownRight + origin, downRight + origin, downLeft + origin, color);
	builder.createQuad(upMidLeft + origin, cornerUpLeft + origin, cornerDownLeft + origin, downMidLeft + origin, color);
	builder.createQuad(cornerUpRight + origin, upMidRight + origin, downMidRight + origin, cornerDownRight + origin, color);
}

void Sun::createSun(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, std::size_t partCount, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = 105;
	sf::Vector2f tmpSize = size;
	sf::Vector2f tmpSizeCorner = sizeCorner;
	partCount = partCount > 0 ? partCount : 1;
	float deltaAlpha = (255 - 105) / partCount;

	for (std::size_t i = 0; i < partCount; i++)
	{
		createOctogon(tmpSize, tmpSizeCorner, origin, color, builder);
		tmpSize = tmpSize - size / 10.f;
		tmpSizeCorner = tmpSizeCorner - sizeCorner / 10.f;
		color.a += deltaAlpha;
	}

	// Create glowing effect
	float glowingCoef = m_glowingTimer.asSeconds() / m_glowingTimerMax.asSeconds();
	color.a = 255 * (1 - glowingCoef);
	// x2 size for the glowing sun
	float glowingCoefSize = glowingCoef * 2.f;
	createOctogon(size * glowingCoefSize, sizeCorner * glowingCoefSize, origin, color, builder);
}

void Sun::setup(ABiome& biome)
{
	m_size = biome.getSunSize();
	m_sizeCorner = m_size / 2.f;
	m_color = biome.getSunColor();
	m_partCount = biome.getSunPartCount();
	m_animator.setup();
}

void Sun::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	m_glowingTimer += frameTime;
	if (m_glowingTimer > m_glowingTimerMax)
		m_glowingTimer -= m_glowingTimerMax;

	sf::Vector2f const & position = getPosition();
	createSun(m_size * m_animation, m_sizeCorner * m_animation, position, m_partCount, m_color, builder);
}
