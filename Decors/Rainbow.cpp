#include "Rainbow.hpp"
#include "ABiome.hpp"
#include <Math.hpp>

Rainbow::Rainbow(void) :
	m_animator(1.f, 0.f, 4.f, 0.1f),
	m_animation(1.f),
	m_cos(0),//std::cos(90.f * octo::Deg2Rad);
	m_sin(1),//std::sin(90.f * octo::Deg2Rad);
	m_loopCount(0u),
	m_partCount(0u),
	m_thickness(50.f),
	m_stripeCount(7u)
{
}

void Rainbow::createFirstLine(Line & line, std::size_t stripeCount, float thickness)
{
	float delta = -thickness / stripeCount + 1;
	for (std::size_t i = 0; i < stripeCount + 1; i++)
		line[i] = sf::Vector2f(i * delta, 0.f);
}

void Rainbow::rotateLine(Line const & start, Line & end, std::size_t stripeCount, sf::Vector2f const & origin, float cos, float sin)
{
	for (std::size_t i = 0; i < stripeCount + 1; i++)
		end[i] = rotateVecCopy(start[i], origin, cos, sin);
}

void Rainbow::createPart(Line const & start, Line const & end, std::size_t stripeCount, sf::Vector2f const & origin, std::vector<sf::Color> const & colors, octo::VertexBuilder& builder)
{
	for (std::size_t i = 0; i < stripeCount; i++)
		builder.createQuad(start[i] + origin, start[i + 1] + origin, end[i + 1] + origin, end[i] + origin, colors[i]);
}

void Rainbow::createRainbow(sf::Vector2f const & origin, std::vector<sf::Vector2f> const & sizes, std::size_t stripeCount, float thickness, std::vector<sf::Color> const & colors, octo::VertexBuilder& builder)
{
	sf::Vector2f originRotate;

	for (std::size_t i = 0; i < sizes.size() - 1; i++)
	{
		if (i == 0)
			createFirstLine(m_start, stripeCount, thickness);
		else
			m_start = m_end;
		originRotate = m_start[0] + sizes[i];
		rotateLine(m_start, m_end, stripeCount, originRotate, m_cos, m_sin);
		createPart(m_start, m_end, stripeCount, origin, colors, builder);
	}
	m_start = m_end;
	originRotate = m_start[0] + sf::Vector2f(0.f, -m_start[0].y);
	rotateLine(m_start, m_end, stripeCount, originRotate, m_cos, m_sin);
	createPart(m_start, m_end, stripeCount, origin, colors, builder);
}

void Rainbow::setupSizes(ABiome & biome, std::vector<sf::Vector2f> & sizes, std::size_t loopCount, std::size_t partCount, float thickness)
{
	(void)biome;
	sizes[0] = sf::Vector2f(400.f + thickness, 0.f);//biome.getRainbowPartSize() * 2;

	for (size_t j = 0; j < loopCount; j++)
	{
		for (std::size_t i = 0; i < partCount; i++)
		{
			if (i == 0)
				sizes[(j * 4) + i + 1] = sf::Vector2f(0.f, 200.f);//biome.getRainbowPartSize();
			if (i == 1)
				sizes[(j * 4) + i + 1] = sf::Vector2f(-120.f, 0.f);//biome.getRainbowPartSize();
			if (i == 2)
				sizes[(j * 4) + i + 1] = sf::Vector2f(0.f, -80.f);//biome.getRainbowPartSize();
			if (i == 3)
				sizes[(j * 4) + i + 1] = sf::Vector2f(200.f, 0.f);//biome.getRainbowPartSize();
		}
	}
	sizes[partCount - 1] = sf::Vector2f(0.f, 0.f);
}

void Rainbow::setupColors(std::vector<sf::Color> & colors)
{
	colors[0] = sf::Color(255, 0, 0);
	colors[1] = sf::Color(255, 127, 0);
	colors[2] = sf::Color(255, 255, 0);
	colors[3] = sf::Color(0, 255, 0);
	colors[4] = sf::Color(0, 0, 255);
	colors[5] = sf::Color(75, 0, 130);
	colors[6] = sf::Color(143, 0, 255);
}

void Rainbow::setup(ABiome& biome)
{
	(void)biome;
	m_loopCount = 2u;//biome.getLoopCount();
	m_partCount = 2u + m_loopCount * 4u;
	m_thickness = 100.f;//biome.getRainbowThickness();
	m_sizes.resize(m_partCount);
	setupSizes(biome, m_sizes, m_loopCount, m_partCount, m_thickness);

	m_stripeCount = 7u;//biome.getStripeCount();
	m_start.resize(m_stripeCount + 1);
	m_end.resize(m_stripeCount + 1);
	m_colors.resize(m_stripeCount);
	setupColors(m_colors);

	m_animator.setup();
}

void Rainbow::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	m_animator.update(frameTime);
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();
	createRainbow(position, m_sizes, m_stripeCount, m_thickness, m_colors, builder);
}

void Rainbow::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}

sf::Vector2f Rainbow::rotateVecCopy(sf::Vector2f const & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle)
{
	sf::Vector2f result = vector;
	result -= origin;
	rotateVec(result, cosAngle, sinAngle);
	result += origin;
	return result;
}

