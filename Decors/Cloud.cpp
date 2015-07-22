#include "Cloud.hpp"
#include "ABiome.hpp"

Cloud::Cloud(void) :
	m_partCount(1u),
	m_animator(4.f, 5.f, 4.f, 0.1f),
	m_animation(1.f)
{
}

void Cloud::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color color, octo::VertexBuilder& builder)
{
	color.a *= m_animation;
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

	sf::Vector2f recDownRight(0.f, -size.y + sizeCorner.y);

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
	recDownRight += origin;

	builder.createTriangle(upLeft, cornerUpLeft, upMidLeft, color);
	builder.createTriangle(upRight, cornerUpRight, upMidRight, color);
	builder.createTriangle(downLeft, cornerDownLeft, downMidLeft, color);
	builder.createTriangle(downRight, cornerDownRight, downMidRight, color);

	builder.createQuad(upLeft, upRight, cornerUpRight, cornerUpLeft, color);
	builder.createQuad(cornerUpLeft, cornerUpRight, cornerDownRight, cornerDownLeft, color);
	builder.createQuad(cornerDownLeft, cornerDownRight, downRight, downLeft, color);
	builder.createQuad(upMidLeft, cornerUpLeft, cornerDownLeft, downMidLeft, color);
	builder.createQuad(cornerUpRight, upMidRight, downMidRight, cornerDownRight, color);

	builder.createQuad(upLeft, upRight, recDownRight, upMidLeft, color);
}

void Cloud::createCloud(std::vector<OctogonValue> const & values, sf::Vector2f const & origin, std::size_t partCount, sf::Color const & color, octo::VertexBuilder& builder)
{
	for (std::size_t i = 0; i < partCount; i++)
		createOctogon(values[i].size * m_animation, values[i].sizeCorner * m_animation, values[i].origin + origin, color, builder);
}

void Cloud::setup(ABiome& biome)
{
	m_color = biome.getCloudColor();
	m_partCount = biome.getCloudPartCount();
	m_values.resize(m_partCount);

	newCloud(biome);
}

void Cloud::newCloud(ABiome & biome)
{
	m_size = biome.getCloudSize();
	for (std::size_t i = 0; i < m_partCount; i++)
	{
		m_values[i].size.x = biome.getCloudSize().x / 5.f;
		m_values[i].size.y = m_values[i].size.x;
		m_values[i].sizeCorner = m_values[i].size / 2.f;
		m_values[i].origin.x = biome.randomFloat(-m_size.x / 2.f, m_size.x / 2.f);
		m_values[i].origin.y = biome.randomFloat(-m_size.y / 2.f, m_size.y / 2.f);
	}

	m_animator.setup(biome.getCloudLifeTime());
}

void Cloud::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	if (m_animator.update(frameTime))
		newCloud(biome);
	m_animation = m_animator.getAnimation();

	sf::Vector2f const & position = getPosition();
	createCloud(m_values, position, m_partCount, m_color, builder);

	if (true)//biome.isRaining()
	{
		sf::FloatRect behindCloud(position.x - 50, position.y, 100.f, 1000.f);
		m_rain.setCameraRect(behindCloud);
		m_rain.update(frameTime, builder);
	}
}
