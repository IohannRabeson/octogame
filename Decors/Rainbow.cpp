#include "Rainbow.hpp"
#include "ABiome.hpp"
#include <Interpolations.hpp>
#include <Math.hpp>

Rainbow::Rainbow(void) :
	m_cos(0),//std::cos(90.f * octo::Deg2Rad);
	m_sin(1),//std::sin(90.f * octo::Deg2Rad);
	m_loopCountMax(10u),
	m_loopCount(0u),
	m_partCount(0u),
	m_thickness(50.f),
	m_stripeCount(7u),
	m_timer(sf::Time::Zero),
	m_intervalTimer(sf::Time::Zero),
	m_intervalTimerMax(sf::Time::Zero),
	m_grow(true),
	m_firstFrame(true)
{
}

bool Rainbow::isDisabledIfOutOfScreen()const
{
	return (false);
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

void Rainbow::createBicolorQuad(sf::Vector2f const & upLeft, sf::Vector2f const & upRight, sf::Vector2f const & downRight, sf::Vector2f const & downLeft, sf::Color const & colorUp, sf::Color const & colorDown, octo::VertexBuilder & builder)
{
	builder.createVertex(upLeft, colorUp);
	builder.createVertex(upRight, colorUp);
	builder.createVertex(downRight, colorDown);
	builder.createVertex(upLeft, colorUp);
	builder.createVertex(downLeft, colorDown);
	builder.createVertex(downRight, colorDown);
}

void Rainbow::createPart(Line const & start, Line const & end, std::size_t stripeCount, sf::Vector2f const & origin, std::vector<sf::Color> colorsStart, std::vector<sf::Color> colorsEnd, float interpolateValue, octo::VertexBuilder& builder)
{
	for (std::size_t i = 0; i < stripeCount; i++)
	{
		if (m_grow)
		{
			colorsEnd[i].a *= interpolateValue;
			sf::Vector2f downLeft = start[i];
			sf::Vector2f downRight = start[i + 1];
			sf::Vector2f upLeft = octo::linearInterpolation(downLeft, end[i], interpolateValue);
			sf::Vector2f upRight = octo::linearInterpolation(downRight, end[i + 1], interpolateValue);
			createBicolorQuad(upRight + origin, upLeft + origin, downLeft + origin, downRight + origin, colorsEnd[i], colorsStart[i], builder);
		}
		else
		{
			colorsStart[i].a *= interpolateValue;
			sf::Vector2f upLeft = end[i];
			sf::Vector2f upRight = end[i + 1];
			sf::Vector2f downLeft = octo::linearInterpolation(upLeft, start[i], interpolateValue);
			sf::Vector2f downRight = octo::linearInterpolation(upRight, start[i + 1], interpolateValue);
			createBicolorQuad(upRight + origin, upLeft + origin, downLeft + origin, downRight + origin, colorsEnd[i], colorsStart[i], builder);
		}
	}
}

void Rainbow::createRainbow(sf::Vector2f const & origin, std::vector<sf::Vector2f> const & sizes, std::size_t stripeCount, float thickness, std::vector<sf::Color> const & colors, std::vector<sf::Color> const & transparent, octo::VertexBuilder& builder)
{
	sf::Vector2f originRotate;

	for (std::size_t i = 0; i < m_partCount - 1; i++)
	{
		if (i == 0)
			createFirstLine(m_start, stripeCount, thickness);
		else
			m_start = m_end;
		originRotate = m_start[0] + sizes[i];
		rotateLine(m_start, m_end, stripeCount, originRotate, m_cos, m_sin);
		if (m_firstFrame == false)
		{
			if (i == 0)
				createPart(m_start, m_end, stripeCount, origin, transparent, colors, m_interpolateValues[i], builder);
			else
				createPart(m_start, m_end, stripeCount, origin, colors, colors, m_interpolateValues[i], builder);
		}
	}
	m_start = m_end;
	originRotate = m_start[0] + sf::Vector2f(0.f, -m_start[0].y);
	rotateLine(m_start, m_end, stripeCount, originRotate, m_cos, m_sin);
	if (m_firstFrame == true)
	{
		m_endPosition = m_end[0];
		m_firstFrame = false;
	}
	else
		createPart(m_start, m_end, stripeCount, origin, colors, transparent, m_interpolateValues[m_partCount - 1], builder);
}

void Rainbow::setupSizes(ABiome & biome, std::vector<sf::Vector2f> & sizes, std::size_t loopCount, std::size_t partCount, float thickness)
{
	sizes[0] = sf::Vector2f(biome.getRainbowPartSize() * 4 + thickness, 0.f);

	for (size_t j = 0; j < loopCount; j++)
	{
		for (std::size_t i = 0; i < partCount; i++)
		{
			float partSize = biome.getRainbowPartSize();
			if (i == 0)
				sizes[(j * 4) + i + 1] = sf::Vector2f(0.f, partSize * 2.f);
			if (i == 1)
				sizes[(j * 4) + i + 1] = sf::Vector2f(-partSize / 2.f, 0.f);
			if (i == 2)
				sizes[(j * 4) + i + 1] = sf::Vector2f(0.f, -partSize / 2.f);
			if (i == 3)
				sizes[(j * 4) + i + 1] = sf::Vector2f(partSize * 2.f, 0.f);
		}
	}
	sizes[partCount - 1] = sizes[0];
}

void Rainbow::setupColors(std::vector<sf::Color> & colors, std::vector<sf::Color> & transparent)
{
	colors[0] = sf::Color(255, 0, 0, 150);
	colors[1] = sf::Color(255, 127, 0, 150);
	colors[2] = sf::Color(255, 255, 0, 150);
	colors[3] = sf::Color(0, 255, 0, 150);
	colors[4] = sf::Color(0, 0, 255, 150);
	colors[5] = sf::Color(75, 0, 130, 150);
	colors[6] = sf::Color(143, 0, 255, 150);

	transparent[0] = sf::Color(255, 255, 255, 0);
	transparent[1] = sf::Color(255, 255, 255, 0);
	transparent[2] = sf::Color(255, 255, 255, 0);
	transparent[3] = sf::Color(255, 255, 255, 0);
	transparent[4] = sf::Color(255, 255, 255, 0);
	transparent[5] = sf::Color(255, 255, 255, 0);
	transparent[6] = sf::Color(255, 255, 255, 0);
}

void Rainbow::newRainbow(ABiome & biome)
{
	m_firstFrame = true;
	m_grow = true;
	m_loopCount = biome.getRainbowLoopCount();
	if (m_loopCount > m_loopCountMax)
		m_loopCount = m_loopCountMax;
	m_partCount = 2u + m_loopCount * 4u;
	m_thickness = biome.getRainbowThickness();
	setupSizes(biome, m_sizes, m_loopCount, m_partCount, m_thickness);
	setupColors(m_colors, m_transparent);

	float totalFlatSizeX = 0.f;
	for (std::size_t i = 0; i < m_partCount; i++)
		totalFlatSizeX += fabs(m_sizes[i].x + m_sizes[i].y);

	float lifeTime = biome.getRainbowLifeTime().asSeconds();
	for (std::size_t i = 0; i < m_partCount; i++)
	{
		m_timerMax[i] = sf::seconds(lifeTime * (fabs(m_sizes[i].x + m_sizes[i].y)) / totalFlatSizeX / 2.f);
		m_interpolateValues[i] = 0.f;
	}
	m_timer = sf::Time::Zero;
	m_intervalTimer = sf::Time::Zero;
	m_intervalTimerMax = biome.getRainbowIntervalTime() + sf::seconds(lifeTime);
}

void Rainbow::setup(ABiome& biome)
{
	std::size_t partCountMax = 2u + m_loopCountMax * 4u;
	m_sizes.resize(partCountMax);

	m_timerMax.resize(partCountMax);
	m_interpolateValues.resize(partCountMax);
	m_start.resize(m_stripeCount + 1);
	m_end.resize(m_stripeCount + 1);
	m_colors.resize(m_stripeCount);
	m_transparent.resize(m_stripeCount);
	newRainbow(biome);
}

void Rainbow::computeInterpolateValuesGrow(sf::Time frameTime, std::vector<float> & values)
{
	if (values[m_partCount - 1] == 1.f)
	{
		m_grow = false;
		return;
	}
	m_timer += frameTime;
	for (std::size_t i = 0; i < m_partCount; i++)
	{
		if (values[i] != 1.f)
		{
			values[i] = m_timer / m_timerMax[i];
			if (m_timer >= m_timerMax[i])
			{
				m_timer = sf::Time::Zero;
				values[i] = 1.f;
			}
			break;
		}
	}
}

void Rainbow::computeInterpolateValuesDie(sf::Time frameTime, std::vector<float> & values)
{
	m_timer += frameTime;
	for (std::size_t i = 0; i < m_partCount; i++)
	{
		if (values[i] != 0.f)
		{
			values[i] = 1 - (m_timer / m_timerMax[i]);
			if (m_timer >= m_timerMax[i])
			{
				m_timer = sf::Time::Zero;
				values[i] = 0.f;
			}
			break;
		}
	}
}

void Rainbow::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	m_intervalTimer += frameTime;
	if (m_intervalTimer <= m_intervalTimerMax)
	{
		sf::Vector2f const & position = getPosition();
		if (m_grow)
			computeInterpolateValuesGrow(frameTime, m_interpolateValues);
		else
			computeInterpolateValuesDie(frameTime, m_interpolateValues);
		// position - m_endPosition make the arrival be the origin
		createRainbow(position - m_endPosition, m_sizes, m_stripeCount, m_thickness, m_colors, m_transparent, builder);
	}
	//TODO:
	else if (biome.isRaining);// && m_clock->isDay())
	{
		newRainbow(biome);
	}
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

