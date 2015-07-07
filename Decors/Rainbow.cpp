#include "Rainbow.hpp"
#include "ABiome.hpp"
#include <Math.hpp>

std::bernoulli_distribution	Rainbow::m_distribution(0.5);
std::default_random_engine	Rainbow::m_engine;

Rainbow::Rainbow(void) :
	m_partCount(1u),
	m_animator(1.f, 0.f, 4.f, 0.1f),
	m_animation(1.f)
{
}

void Rainbow::computeHorizontalLine(float thickness, sf::Vector2f const & origin, std::size_t stripeCount, std::vector<sf::Vector2f> & points)
{
	float stripeSize = thickness / stripeCount;
	sf::Vector2f relativeOrigin(origin.x - thickness / 2.f, origin.y);

	for (std::size_t i = 0; i < stripeCount; i++)
		points[i] = sf::Vector2f(0.f, stripeSize * (i + 1)) + relativeOrigin;
}

void Rainbow::computeVerticalLine(float thickness, sf::Vector2f const & origin, std::size_t stripeCount, std::vector<sf::Vector2f> & points)
{
	float stripeSize = thickness / stripeCount;
	sf::Vector2f relativeOrigin(origin.x, origin.y - thickness / 2.f);

	for (std::size_t i = 0; i < stripeCount; i++)
		points[i] = sf::Vector2f(stripeSize * (i + 1), 0.f) + relativeOrigin;
}

void Rainbow::createRainbowPart(float thickness, sf::Vector2f const & origin, std::vector<sf::Vector2f> & start, std::vector<sf::Vector2f> & end, std::size_t stripeCount, octo::VertexBuilder& builder)
{
	(void)thickness;
	for (std::size_t i = 0; i < stripeCount - 1; i++)
	{
		builder.createQuad(start[i] + origin, start[i + 1] + origin, end[i + 1] + origin, end[i] + origin, m_stripesColors[i]);
	}
}

void Rainbow::createRainbow(sf::Vector2f const & origin, std::size_t partCount, std::vector<sf::Vector2f> & partLineOrigin, sf::Color const & color, octo::VertexBuilder& builder)
{
	(void)color;
	partLineOrigin[0] = origin;
	for (std::size_t i = 0; i < partCount; i++)
	{
		sf::Vector2f rotateOrigin(partLineOrigin[i].x + m_partSizes[i], partLineOrigin[i].y);
		m_partLineOrigin[i + 1] = m_partLineOrigin[i];
		if (m_partSizes[i] < 0.f )
		{
			if (m_partSides[i])
				rotateVec(partLineOrigin[i + 1], rotateOrigin, -m_cos, -m_sin);
			else
				rotateVec(partLineOrigin[i + 1], rotateOrigin, m_cos, m_sin);
		}
		else
		{
			if (m_partSides[i])
				rotateVec(partLineOrigin[i + 1], rotateOrigin, m_cos, m_sin);
			else
				rotateVec(partLineOrigin[i + 1], rotateOrigin, -m_cos, -m_sin);
		}

		if (i > 0)
		{
			if (m_partSides[i - 1])
				computeVerticalLine(40.f, partLineOrigin[i - 1], m_stripeCount, m_startPart);
			else
				computeHorizontalLine(40.f, partLineOrigin[i - 1], m_stripeCount, m_startPart);
			if (m_partSides[i])
				computeVerticalLine(40.f, partLineOrigin[i], m_stripeCount, m_endPart);
			else
				computeHorizontalLine(40.f, partLineOrigin[i], m_stripeCount, m_endPart);
			createRainbowPart(40.f, origin, m_startPart, m_endPart, m_stripeCount, builder);
		}
		//builder.createLine(partLineOrigin[i], partLineOrigin[i + 1], 5.f, color);
	}
}

void Rainbow::setup(ABiome& biome)
{
	m_size = biome.getRainbowSize();
	m_partCount = 8u;//biome.getRainbowPartCount();
	m_stripeCount = 7u;
	m_stripesColors.resize(m_stripeCount);
	m_stripesColors[0] = sf::Color(255, 0, 0);
	m_stripesColors[1] = sf::Color(255, 127, 0);
	m_stripesColors[2] = sf::Color(255, 255, 0);
	m_stripesColors[3] = sf::Color(0, 255, 0);
	m_stripesColors[4] = sf::Color(0, 0, 255);
	m_stripesColors[5] = sf::Color(75, 0, 130);
	m_stripesColors[6] = sf::Color(143, 0, 255);
	m_cos = 0;//std::cos(90.f * octo::Deg2Rad);
	m_sin = 1;//std::sin(90.f * octo::Deg2Rad);

	m_startPart.resize(m_stripeCount);
	m_endPart.resize(m_stripeCount);
	m_partLineOrigin.resize(m_partCount + 1);
	m_partSizes.resize(m_partCount);
	m_partSides.resize(m_partCount);

	m_partSizes[0] = -100.f;
	m_partSizes[1] = 100.f;
	m_partSizes[2] = -150.f;
	m_partSizes[3] = -200.f;
	m_partSizes[4] = -100.f;
	m_partSizes[5] = 100.f;
	m_partSizes[6] = 200.f;
	m_partSizes[7] = -150.f;

	m_partSides[0] = true;
	m_partSides[1] = true;
	m_partSides[2] = true;
	m_partSides[3] = false;
	m_partSides[4] = true;
	m_partSides[5] = true;
	m_partSides[6] = false;
	m_partSides[7] = false;

	m_animator.setup();
}

void Rainbow::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();
	createRainbow(position, m_partCount, m_partLineOrigin, sf::Color(213, 122, 0), builder);
}

void Rainbow::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}

void Rainbow::rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle)
{
	vector -= origin;
	rotateVec(vector, cosAngle, sinAngle);
	vector += origin;
}

bool Rainbow::getPartAngle(void)
{
	return m_distribution(m_engine);
}
