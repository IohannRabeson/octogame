#include "Cloud.hpp"
#include "ABiome.hpp"
#include "SkyCycle.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <Camera.hpp>

Cloud::Cloud(void) :
	Cloud(nullptr)
{
}

Cloud::Cloud(SkyCycle * cycle) :
	m_partCount(1u),
	m_animator(4.f, 5.f, 4.f, 0.1f),
	m_animation(1.f),
	m_cycle(cycle),
	m_lightning(200000)
{
}

Cloud::~Cloud(void)
{
	for (std::size_t i = 0; i < m_partCount; i++)
		delete m_rain[i];
}

bool Cloud::isDisabledIfOutOfScreen()const
{
	return (false);
}

void Cloud::createOctogon(sf::Vector2f const & size, sf::Vector2f const & sizeCorner, sf::Vector2f const & origin, sf::Color color, octo::VertexBuilder& builder)
{
	color.a *= m_animation;
	sf::Vector2f upLeft(-size.x + sizeCorner.x, -size.y);
	sf::Vector2f upRight(size.x - sizeCorner.x, -size.y);
	sf::Vector2f upMidLeft(-size.x, -size.y + sizeCorner.y);
	sf::Vector2f upMidRight(size.x, -size.y + sizeCorner.y);
	sf::Vector2f downLeft(-size.x + sizeCorner.x, size.y);
	sf::Vector2f downRight(size.x - sizeCorner.x, size.y);
	sf::Vector2f downMidLeft(-size.x, size.y - sizeCorner.y);
	sf::Vector2f downMidRight(size.x, size.y - sizeCorner.y);

	sf::Vector2f recDownRight(0.f, -size.y + sizeCorner.y);

	upLeft += origin;
	upRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downLeft += origin;
	downRight += origin;
	downMidLeft += origin;
	downMidRight += origin;
	recDownRight += origin;

	builder.createTriangle(origin, upLeft, upRight, color);
	builder.createTriangle(origin, upRight, upMidRight, color);
	builder.createTriangle(origin, upMidRight, downMidRight, color);
	builder.createTriangle(origin, downMidRight, downRight, color);
	builder.createTriangle(origin, downRight, downLeft, color);
	builder.createTriangle(origin, downLeft, downMidLeft, color);
	builder.createTriangle(origin, downMidLeft, upMidLeft, color);
	builder.createTriangle(origin, upMidLeft, upLeft, color);

	builder.createQuad(upLeft, upRight, recDownRight, upMidLeft, color);
}

void Cloud::createCloud(std::vector<OctogonValue> const & values, sf::Vector2f const & origin, std::size_t partCount, sf::Color const & color, octo::VertexBuilder& builder)
{
	for (std::size_t i = 0; i < partCount; i++)
		createOctogon(values[i].size * m_animation, values[i].sizeCorner * m_animation, values[i].origin + origin, color, builder);
}

void Cloud::setupLightning(void)
{
	octo::Camera& camera = octo::Application::getCamera();

	m_lightning.addArc(camera.getCenter(), camera.getCenter(), 2.f);
	m_lightning.addArc(camera.getCenter(), camera.getCenter(), 4.f);
	m_lightning.addArc(camera.getCenter(), camera.getCenter(), 6.f);
	m_lightning.addArc(camera.getCenter(), camera.getCenter(), 4.f);
	m_lightning.addArc(camera.getCenter(), camera.getCenter(), 2.f);
}

void Cloud::setup(ABiome& biome)
{
	m_color = biome.getCloudColor();
	m_partCount = biome.getCloudPartCount();
	m_values.resize(m_partCount);
	m_rain.resize(m_partCount);
	m_rainUpLeft.resize(m_partCount);

	for (std::size_t i = 0; i < m_partCount; i++)
		m_rain[i] = new RainSystem();

	newCloud(biome);
	setupLightning();
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

		m_rainUpLeft[i] = sf::Vector2f(m_values[i].origin.x - m_values[i].size.x, m_values[i].origin.y + m_values[i].size.y);
	}

	m_animator.setup(biome.getCloudLifeTime());
}

void Cloud::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	sf::Vector2f const & position = getPosition();
	float weather = m_cycle == nullptr ? 0.f : m_cycle->getWeatherValue();
	float thunder = m_cycle == nullptr ? 0.f : m_cycle->getThunderValue();

	if (thunder)
	{
		m_p0 = position;
		m_p1 = sf::Vector2f(position.x, position.y + 1000.f * thunder);
		for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
			m_lightning.setArc(i, m_p0, m_p1);
		for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
			m_lightning.setArc(i, m_p0, m_p1);

		m_lightning.update(frameTime, builder);
	}

	//TODO: To improve: Replace rain rect just top of screen, dont update rain all time
	if (biome.canRain())
	{
		sf::Vector2f size(0.f, biome.getMapSizeFloat().y);
		for (std::size_t i = 0; i < m_partCount; i++)
		{
			size.x = m_values[i].size.x * 2.f;
			sf::FloatRect rect(m_rainUpLeft[i] + position, size * m_animation);
			m_rain[i]->setDropAngle(biome.getWind() / 4.f);
			m_rain[i]->setRainRect(rect);
			m_rain[i]->setDropPerSecond(weather);
			m_rain[i]->update(frameTime, builder);
		}
	}

	if (m_animator.update(frameTime))
		newCloud(biome);
	m_animation = m_animator.getAnimation();

	createCloud(m_values, position, m_partCount, m_color, builder);
}
