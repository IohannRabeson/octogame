#include "Crystal.hpp"
#include "DecorBuilder.hpp"
#include "ABiome.hpp"

//TODO: Other solutions?
#define PI  3.14159265358979323846

Crystal::Crystal() :
	m_partCount(0u),
	m_animation(1u)
	//TODO: Implement shine effet
	/*
	m_shineCrystalNumber(0u),
	m_shineVertexNumber(0u),
	m_shineTimer(sf::seconds(0.f)),
	m_shineTimerMax(sf::seconds(3.f))
	*/
{
}

sf::Vector2f Crystal::createPolygon(sf::Vector2f const & size, sf::Vector2f const & origin, float const angle, sf::Color color, DecorBuilder & builder)
{
	sf::Vector2f up(0.0f, -size.x - size.y);
	sf::Vector2f upLeft(-size.x, -size.y);
	sf::Vector2f upMid(0.0f, -size.y);
	sf::Vector2f upRight(size.x, -size.y);

	sf::Vector2f down(0.0f, size.x);
	sf::Vector2f downLeft(-size.x, 0.f);
	sf::Vector2f downMid(0.0f, 0.f);
	sf::Vector2f downRight(size.x, 0.f);

	float radianAngle = angle * PI / 180.0f;
	float cosA = std::cos(radianAngle);
	float sinA = std::sin(radianAngle);

	rotateVec(up, cosA, sinA);
	rotateVec(upLeft, cosA, sinA);
	rotateVec(upMid, cosA, sinA);
	rotateVec(upRight, cosA, sinA);
	rotateVec(down, cosA, sinA);
	rotateVec(downLeft, cosA, sinA);
	rotateVec(downMid, cosA, sinA);
	rotateVec(downRight, cosA, sinA);

	sf::Color tmpAddColor(5, 5, 5, 0);

	// Down right
	color += tmpAddColor;
	builder.createTriangle(down + origin, downMid + origin, downRight + origin, color);
	// Mid right
	color += tmpAddColor;
	builder.createQuad(upRight + origin, upMid + origin, downMid + origin, downRight + origin, color);
	// Up right
	color += tmpAddColor;
	builder.createTriangle(up + origin, upMid + origin, upRight + origin, color);
	// Down left
	color += tmpAddColor;
	builder.createTriangle(down + origin, downMid + origin, downLeft + origin, color);
	// Mid left
	color += tmpAddColor;
	builder.createQuad(upLeft + origin, upMid + origin, downMid + origin, downLeft + origin, color);
	// Up left
	color += tmpAddColor;
	builder.createTriangle(up + origin, upMid + origin, upLeft + origin, color);

	//TODO: Implement shine effet
	/*
	if (m_shineTimer.asSeconds() >= 0.3f)
	{
		//TODO: Change this random number generation
		m_shineVertexNumber = static_cast<int>(randomFloat(0.f, 3.99f));
		m_shineTimer = sf::seconds(0.f);
	}
	switch (m_shineVertexNumber)
	{
		case 0:
			return up;
		case 1:
			return upLeft;
		case 2:
			return upRight;
		case 3:
			return upMid;
		default:
			return up;
	}
	*/
	return up;
}

void Crystal::createCrystal(std::vector<CrystalValue> const & values, sf::Vector2f const & origin, DecorBuilder & builder)
{
	for (unsigned int i = 0; i < m_partCount; i++)
		m_up[i] = createPolygon(values[i].size * m_animation, origin, values[i].angle, values[i].color, builder);
}

void Crystal::setup(ABiome& biome)
{
	m_color = biome.getCrystalColor();
	m_partCount = biome.getCrystalPartCount();
	m_values.resize(m_partCount);

	m_up.resize(m_partCount);

	for (unsigned int i = 0; i < m_partCount; i++)
	{
		m_values[i].size = sf::Vector2f(10.f, 100.f);//biome.getCrystalSize();
		m_values[i].angle = randomFloat(-45.f + (i * 90.f / m_partCount), -45.f + ((i + 1) * 90.f / m_partCount));
		int deltaColor = randomFloat(0.f, 80.f);
		m_values[i].color = m_color + sf::Color(deltaColor, deltaColor, deltaColor, 0);
	}
}

void Crystal::update(sf::Time frameTime, DecorBuilder& builder, ABiome&)
{
	sf::Vector2f const & position = getPosition();
	//TODO: Test this with terrain
	createCrystal(m_values, sf::Vector2f(position.x, position.y + m_size.x), builder);

	//TODO: Implement shine effet
	(void)frameTime;
	/*
	m_shineTimer += frameTime;
	//TODO: Change randomFloat by randomInt
	if (m_shineTimer.asSeconds() == 0.f)
		m_shineCrystalNumber = randomFloat(0, m_partCount - 0.01);
	m_star.setOrigin(m_up[m_shineCrystalNumber] + m_origin);
	m_star.shine(deltatime);
	*/
}

void Crystal::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}

// TODO: To delete and add in Biome
float Crystal::randomFloat(float min, float max)
{
	if (max - min == 0)
		return max;
	std::uniform_int_distribution<float> distribution(min, max);
	std::random_device rd;
	std::mt19937 engine(rd());

	return distribution(engine);
}
