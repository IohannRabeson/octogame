#include "Cloud.hpp"
#include "ABiome.hpp"

std::mt19937	Cloud::m_engine;

Cloud::Cloud() :
	m_partCount(0),
	m_animation(1.f),
	m_lifeTime(sf::Time::Zero)
{
	std::random_device rd;
	m_engine.seed(rd());
}

void Cloud::createOctogon(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color color, float const sizeUp, float const sizeDown, float const sizeRec, octo::VertexBuilder& builder)
{
	color.a = m_animation * color.a;

	sf::Vector2f upLeft(-size.x + sizeUp, -size.y - sizeUp);
	sf::Vector2f upRight(size.x - sizeUp, -size.y - sizeUp);
	sf::Vector2f cornerUpLeft(-size.x + sizeUp, -size.y);
	sf::Vector2f cornerUpRight(size.x - sizeUp, -size.y);
	sf::Vector2f upMidLeft(-size.x, -size.y);
	sf::Vector2f upMidRight(size.x, -size.y);

	sf::Vector2f downLeft(-size.x + sizeDown, size.y + sizeDown);
	sf::Vector2f downRight(size.x - sizeDown, size.y + sizeDown);
	sf::Vector2f cornerDownLeft(-size.x + sizeDown, size.y);
	sf::Vector2f cornerDownRight(size.x - sizeDown, size.y);
	sf::Vector2f downMidLeft(-size.x, size.y);
	sf::Vector2f downMidRight(size.x, size.y);

	sf::Vector2f recUpLeft(downMidLeft.x, downMidLeft.y - sizeRec);
	sf::Vector2f recUpRight(downMidLeft.x + 4.f, recUpLeft.y);
	sf::Vector2f recDown(downMidLeft.x + 4.f, downMidLeft.y);
	sf::Vector2f secondRec(downLeft.x, downLeft.y - 4.f);

	upLeft += origin;
	upRight += origin;
	cornerUpLeft += origin;
	cornerUpRight += origin;
	upMidLeft += origin;
	upMidRight += origin;

	downLeft += origin;
	downRight += origin;
	cornerDownLeft += origin;
	cornerDownRight += origin;
	downMidLeft += origin;
	downMidRight += origin;

	recUpLeft += origin;
	recUpRight += origin;
	recDown += origin;
	secondRec += origin;

	// Corner up
	builder.createTriangle(upMidRight, cornerUpRight, upRight, color);
	builder.createTriangle(upMidLeft, cornerUpLeft, upLeft, color);

	// Fill with rectangle
	builder.createQuad(upLeft, upRight, cornerUpRight, cornerUpLeft, color);
	builder.createQuad(cornerUpLeft, cornerUpRight, cornerDownRight, cornerDownLeft, color);
	builder.createQuad(cornerDownLeft, cornerDownRight, downRight, downLeft, color);
	builder.createQuad(upMidLeft, cornerUpLeft, cornerDownLeft, downMidLeft, color);
	builder.createQuad(cornerUpRight, upMidRight, downMidRight, cornerDownRight, color);

	// Corner down
	builder.createTriangle(downMidRight, cornerDownRight, downRight, color);
	builder.createTriangle(downMidLeft, cornerDownLeft, downLeft, color);

	// Details
	builder.createQuad(downMidLeft, recUpLeft, recUpRight, recDown, color);
	builder.createQuad(downMidLeft, recDown, secondRec, downLeft, color);
}

void Cloud::createCloud(std::vector<OctogonValue> const & values, sf::Vector2f const & originCloud, sf::Color const & color, octo::VertexBuilder& builder)
{
	for (std::size_t i = 0; i < m_partCount; i++)
		createOctogon(sf::Vector2f(values[i].size.x, values[i].size.y * m_animation), values[i].origin + originCloud, color,
						values[i].sizeUp * m_animation, values[i].sizeDown * m_animation, values[i].sizeRec * m_animation, builder);
}

void Cloud::setup(ABiome& biome)
{
	m_size = biome.getCloudSize();
	m_color = biome.getCloudColor();
	m_color.a = randomFloat(80.f, 200.f);
	m_partCount = biome.getCloudPartCount();
	m_values.resize(m_partCount);
	m_lifeTime = biome.getCloudLifeTime();

	std::size_t i = 0;
	float totalY = 0;
	float cornerSize = m_size.x / (m_partCount * 2.f);
	sf::Vector2f size;

	// Compute left random values
	size.x = m_size.x;
	size.y = cornerSize;
	sf::Vector2f origin = sf::Vector2f(0.f, 0.f);
	while (i < m_partCount / 2)
	{
		//TODO: Find a better way to have more random values here
		size.y = randomFloat(cornerSize * 0.5f, cornerSize);
		totalY += size.y;
		size.x -= totalY;
		origin.y += randomFloat(-totalY, 0.f);
		if (size.y * 2 < size.x)
		{
			m_values[i].size = size;
			m_values[i].origin = origin;
			m_values[i].sizeUp = randomFloat(size.y, size.y * 2);
			m_values[i].sizeDown = randomFloat(size.y, size.y * 2);
			m_values[i].sizeRec = randomFloat(10, size.y * 2);
		}
		else
			break;
		i++;
	}

	// Compute right random values
	totalY = 0;
	m_size = biome.getCloudSize();
	cornerSize = m_size.x / (m_partCount * 2.f);
	size.x = m_size.x - cornerSize;
	size.y = cornerSize;
	origin = sf::Vector2f(0.f, 0.f + size.y);
	while (i < m_partCount)
	{
		//TODO: Find a better way to have more random values here
		size.y = randomFloat(cornerSize * 0.5f, cornerSize);
		totalY += size.y;
		size.x -= totalY;
		origin.y += randomFloat(0.0f, totalY);
		if (size.y * 2 < size.x)
		{
			m_values[i].size = size;
			m_values[i].origin = origin;
			m_values[i].sizeUp = randomFloat(size.y, size.y * 2);
			m_values[i].sizeDown = randomFloat(size.y, size.y * 2);
			m_values[i].sizeRec = randomFloat(10, size.y * 2);
		}
		i++;
	}
}

void Cloud::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome&)
{
	(void)frameTime;

	sf::Vector2f const & position = getPosition();
	createCloud(m_values, position, m_color, builder);
}

float Cloud::randomFloat(float min, float max)
{
	if (max - min == 0)
		return max;
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(m_engine);
}

