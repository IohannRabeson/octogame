#include "Cloud.hpp"
#include "ABiome.hpp"
#include "SkyCycle.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <Camera.hpp>
#include <ResourceManager.hpp>

#include "ResourceDefinitions.hpp"

Cloud::Cloud(void) :
	Cloud(nullptr)
{
}

Cloud::Cloud(SkyCycle * cycle) :
	m_partCount(1u),
	m_cloudMinX(0.f),
	m_cloudMaxX(0.f),
	m_cloudMinY(0.f),
	m_cloudMaxY(0.f),
	m_animator(4.f, 5.f, 4.f, 0.2f),
	m_animation(1.f),
	m_isCollide(false),
	m_canWeather(false),
	m_thunderCloud(false),
	m_lightning(1),
	m_lightningSize(0.f),
	m_cycle(cycle)
{
}

Cloud::~Cloud(void)
{
	for (std::size_t i = 0; i < m_partCount; i++)
	{
		delete m_rain[i];
		delete m_snow[i];
	}
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

	if (m_speed.y != 0.f)
	{
		octo::rotateVector(upLeft, m_cos, m_sin);
		octo::rotateVector(upRight, m_cos, m_sin);
		octo::rotateVector(upMidLeft, m_cos, m_sin);
		octo::rotateVector(upMidRight, m_cos, m_sin);
		octo::rotateVector(downLeft, m_cos, m_sin);
		octo::rotateVector(downRight, m_cos, m_sin);
		octo::rotateVector(downMidLeft, m_cos, m_sin);
		octo::rotateVector(downMidRight, m_cos, m_sin);
	}

	upLeft += origin;
	upRight += origin;
	upMidLeft += origin;
	upMidRight += origin;
	downLeft += origin;
	downRight += origin;
	downMidLeft += origin;
	downMidRight += origin;
	recDownRight += origin;

	sf::Color deltaColor = color;
	if (m_speed.y != 0.f)
		deltaColor = color + sf::Color(7, 7, 7, 0);
	builder.createTriangle(origin, upLeft, upRight, deltaColor);
	builder.createTriangle(origin, upRight, upMidRight, deltaColor);
	builder.createTriangle(origin, upMidRight, downMidRight, deltaColor);
	builder.createTriangle(origin, downMidRight, downRight, deltaColor);
	builder.createTriangle(origin, downRight, downLeft, color);
	builder.createTriangle(origin, downLeft, downMidLeft, color);
	builder.createTriangle(origin, downMidLeft, upMidLeft, color);
	builder.createTriangle(origin, upMidLeft, upLeft, color);

	if (Progress::getInstance().getNextDestination() != Level::IceC && Progress::getInstance().getNextDestination() != Level::WaterB)
		builder.createQuad(upLeft, upRight, recDownRight, upMidLeft, color);
}

bool Cloud::isOctogonContain(sf::Vector2f const & size, sf::Vector2f const & position, sf::Vector2f const & point) const
{
	if (!m_isCollide && (point.x > position.x - size.x && point.x < position.x + size.x)
		&& (point.y > position.y - size.y && point.y < position.y + size.y))
	{
		return true;
	}
	return false;
}

void Cloud::createCloud(std::vector<OctogonValue> const & values, sf::Vector2f const & origin, std::size_t partCount, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f const & octoPosition = Progress::getInstance().getOctoPos();
	m_isCollide = false;
	for (std::size_t i = 0; i < partCount; i++)
	{
		if (isOctogonContain(values[i].size * m_animation, values[i].origin + origin, octoPosition))
		{
			m_isCollide = true;
		}
		createOctogon(values[i].size * m_animation, values[i].sizeCorner * m_animation, values[i].origin + origin, color, builder);
	}
}

void Cloud::setupLightning(ABiome & biome)
{
	m_thunderCloud = biome.randomBool(0.6);

	if (m_thunderCloud)
	{
		octo::Camera& camera = octo::Application::getCamera();

		m_lightning.addArc(camera.getCenter(), camera.getCenter(), 2.f);
		m_lightning.addArc(camera.getCenter(), camera.getCenter(), 4.f);
		m_lightning.addArc(camera.getCenter(), camera.getCenter(), 6.f);
		m_lightning.addArc(camera.getCenter(), camera.getCenter(), 4.f);
		m_lightning.addArc(camera.getCenter(), camera.getCenter(), 2.f);

		m_lightningSize = biome.getLightningSize();
	}
}

void Cloud::setup(ABiome& biome)
{
	m_color = biome.getCloudColor();
	m_partCount = biome.getCloudPartCount();
	m_cloudMinY = biome.getCloudMinY();
	m_cloudMaxY = biome.getCloudMaxY();
	m_position = sf::Vector2f(biome.randomFloat(m_cloudMinX, m_cloudMaxX), biome.randomFloat(m_cloudMinY, m_cloudMaxY));
	m_values.resize(m_partCount);
	m_rain.resize(m_partCount);
	m_snow.resize(m_partCount);
	m_dropUpLeft.resize(m_partCount);

	for (std::size_t i = 0; i < m_partCount; i++)
	{
		m_rain[i] = new DropSystem();
		m_rain[i]->setDrop(sf::Vector2f(0.5f, 50.f), 1024.f, m_color, biome);
		m_snow[i] = new DropSystem();
		m_snow[i]->setDrop(sf::Vector2f(5.f, 5.f), 256.f, m_color, biome);
	}

	newCloud(biome);
	setupLightning(biome);
}

void Cloud::newCloud(ABiome & biome)
{
	m_size = biome.getCloudSize();
	m_speed.x = biome.getCloudSpeed().x + biome.getWind();
	m_speed.y = biome.randomFloat(biome.getCloudSpeed().y * 1.5f, biome.getCloudSpeed().y * 0.5f);

	if (m_speed.y == 0.f)
		m_position = sf::Vector2f(biome.randomFloat(m_cloudMinX, m_cloudMaxX), biome.randomFloat(m_cloudMinY, m_cloudMaxY));
	else
	{
		float angle = biome.randomFloat(0.f, 180.f);
		m_position.x = biome.randomFloat(m_cloudMinX, m_cloudMaxX);
		m_cos = std::cos(angle * octo::Deg2Rad);
		m_sin = std::sin(angle * octo::Deg2Rad);
	}

	for (std::size_t i = 0; i < m_partCount; i++)
	{
		m_values[i].size.x = biome.getCloudSize().x / 5.f;
		m_values[i].size.y = m_values[i].size.x;
		m_values[i].sizeCorner = m_values[i].size / 2.f;
		m_values[i].origin.x = biome.randomFloat(-m_size.x / 2.f, m_size.x / 2.f);
		m_values[i].origin.y = biome.randomFloat(-m_size.y / 2.f, m_size.y / 2.f);

		m_dropUpLeft[i] = sf::Vector2f(m_values[i].origin.x - m_values[i].size.x, m_values[i].origin.y);
	}

	m_animator.setup(biome.getCloudLifeTime());
}

void Cloud::updateThunder(sf::Time frameTime, ABiome & biome, octo::VertexBuilder & builder, sf::Vector2f const & position)
{
	float thunder = m_cycle == nullptr ? 0.f : m_cycle->getThunderValue();
	if (m_thunderCloud && thunder && biome.randomBool(0.6))
	{
		std::size_t cloudNumber = biome.randomInt(0, m_partCount - 1);
		m_p0 = position + m_values[cloudNumber].origin;
		m_p1 = sf::Vector2f(position.x, position.y + m_lightningSize * thunder);
		for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
			m_lightning.setArc(i, m_p0, m_p1);
		for (auto i = 0u; i < m_lightning.getArcCount(); ++i)
			m_lightning.setArc(i, m_p0, m_p1);
		m_lightning.update(frameTime, builder);
	}
}

void Cloud::updateRain(sf::Time frameTime, ABiome & biome, octo::VertexBuilder & builder, sf::Vector2f const & position, float weather)
{
	for (std::size_t i = 0; i < m_partCount; i++)
	{
		sf::Vector2f size(m_values[i].size.x * 2.f, m_values[i].size.y);
		sf::FloatRect rect(m_dropUpLeft[i] + position, size * m_animation);
		float angle = biome.getWind() / 4.f + biome.randomFloat(-5.f, 5.f);
		if (angle > 45.f)
			angle = 45.f;
		else if (angle < -45.f)
			angle = -45.f;
		m_rain[i]->setDropRect(rect);
		// -i create 1 second of delay and avoid one line of drop a the begining
		m_rain[i]->setDropPerSecond(weather - i);
		m_rain[i]->update(frameTime, angle, builder);
	}
}

void Cloud::updateSnow(sf::Time frameTime, ABiome & biome, octo::VertexBuilder & builder, sf::Vector2f const & position, float weather)
{
	for (std::size_t i = 0; i < m_partCount; i++)
	{
		sf::Vector2f size(m_values[i].size.x * 2.f, m_values[i].size.y);
		sf::FloatRect rect(m_dropUpLeft[i] + position, size * m_animation);
		m_snow[i]->setDropRect(rect);
		m_snow[i]->setDropPerSecond(weather - i);
		m_snow[i]->update(frameTime, biome.randomFloat(-45.f, 45.f), builder);
	}
}

void Cloud::updatePosition(sf::Time frameTime)
{
	octo::Camera const & camera = octo::Application::getCamera();
	m_cloudMinX = camera.getCenter().x - camera.getSize().x * 2.f;
	m_cloudMaxX = camera.getCenter().x + camera.getSize().x * 2.f;

	m_position.x += m_speed.x * frameTime.asSeconds();
	if (m_position.x >= m_cloudMaxX)
		m_position.x = m_cloudMinX;
	else if (m_position.x <= m_cloudMinX)
		m_position.x = m_cloudMaxX;

	if (m_speed.y != 0.f)
	{
		m_position.y += m_speed.y * frameTime.asSeconds();
		if (m_position.y >= m_cloudMaxY)
			m_position.y = m_cloudMinY;
		else if (m_position.y <= m_cloudMinY)
		{
			if (m_animator.getState() == DecorAnimator::State::Grow)
				m_position.y = m_cloudMaxY;
			else if (m_animator.getState() != DecorAnimator::State::Die)
				m_animator.die();
		}
	}
	setPosition(m_position);
}

void Cloud::update(sf::Time frameTime, octo::VertexBuilder& builder, ABiome& biome)
{
	updatePosition(frameTime);

	float weather = m_cycle == nullptr ? 0.f : m_cycle->getWeatherValue() / 4.f;
	if (m_animator.getState() == DecorAnimator::State::Life && weather == 0.f)
		m_canWeather = true;
	else if (m_animator.getState() != DecorAnimator::State::Life)
		m_canWeather = false;

	if (m_canWeather)
	{
		if (biome.canCreateThunder())
			updateThunder(frameTime, biome, builder, m_position);

		if (biome.canCreateRain())
			updateRain(frameTime, biome, builder, m_position, weather);
		else if (biome.canCreateSnow())
			updateSnow(frameTime, biome, builder, m_position, weather);
	}

	if (m_animator.update(frameTime))
		newCloud(biome);
	m_animation = m_animator.getAnimation();

	createCloud(m_values, m_position, m_partCount, m_color, builder);

	if (m_isCollide)
	{
		if (m_animator.getState() != DecorAnimator::State::Die)
			Progress::getInstance().resetDoubleJump(true);
		m_animator.die();
	}
}
