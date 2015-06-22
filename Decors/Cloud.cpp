#include "Cloud.hpp"
#include "ABiome.hpp"

Cloud::Cloud() :
	m_partCount(0),
	m_animation(1.f),
	m_lifeTime(sf::Time::Zero),
	m_alpha(0)
{
}

void Cloud::createOctogon(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color color, float const sizeUp, float const sizeDown, float const sizeRec, octo::VertexBuilder& builder)
{
	color.a = m_animation * m_alpha;

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

	// Corner up
	builder.createTriangle(upMidRight + origin, cornerUpRight + origin, upRight + origin, color);
	builder.createTriangle(upMidLeft + origin, cornerUpLeft + origin, upLeft + origin, color);

	// Fill with rectangle
	builder.createQuad(upLeft + origin, upRight + origin, cornerUpRight + origin, cornerUpLeft + origin, color);
	builder.createQuad(cornerUpLeft + origin, cornerUpRight + origin, cornerDownRight + origin, cornerDownLeft + origin, color);
	builder.createQuad(cornerDownLeft + origin, cornerDownRight + origin, downRight + origin, downLeft + origin, color);
	builder.createQuad(upMidLeft + origin, cornerUpLeft + origin, cornerDownLeft + origin, downMidLeft + origin, color);
	builder.createQuad(cornerUpRight + origin, upMidRight + origin, downMidRight + origin, cornerDownRight + origin, color);

	// Corner down
	builder.createTriangle(downMidRight + origin, cornerDownRight + origin, downRight + origin, color);
	builder.createTriangle(downMidLeft + origin, cornerDownLeft + origin, downLeft + origin, color);

	// Details
	builder.createQuad(downMidLeft + origin, recUpLeft + origin, recUpRight + origin, recDown + origin, color);
	builder.createQuad(downMidLeft + origin, recDown + origin, secondRec + origin, downLeft + origin, color);
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
	m_partCount = biome.getCloudPartCount();
	m_values.resize(m_partCount);
	//TODO: Dont forget to add random to timer for un syncronize decors
	//TO DO IN ALL CLASS

	m_lifeTime = biome.getCloudLifeTime();

	//TODO: Change this
	m_alpha = randomFloat(100, 220);

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
	m_size = sf::Vector2f(randomFloat(200.f, 300.f), randomFloat(400.f, 600.f));//biome.getCloudSize();
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

// TODO: To delete and add in Biome
float Cloud::randomFloat(float min, float max)
{
	if (max - min == 0)
		return max;
	std::uniform_real_distribution<float> distribution(min, max);
	std::random_device rd;
	std::mt19937 engine(rd());

	return distribution(engine);
}

