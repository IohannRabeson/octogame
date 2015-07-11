#include "SkyManager.hpp"
#include "ABiome.hpp"
#include "ADecor.hpp"
#include "Star.hpp"

#include <Math.hpp>
#include <Application.hpp>
#include <Camera.hpp>

//TODO: Add canCreate* for moon, stars and sun

SkyManager::SkyManager(void) :
	m_decorManagerBack(200000),
	m_decorManagerFront(20000),
	m_wind(0.f),
	//TODO: Put this in GameClock and add this in biome
	m_timer(0.f),
	m_timerMax(30.f),
	m_starCount(0u),
	m_cloudCount(0u)
{
}

sf::Vector2f SkyManager::setPosition(DecorManager::Iterator decor, sf::Vector2f origin, sf::Vector2f originRotate, float cos, float sin)
{
	rotateVec(origin, originRotate, cos, sin);
	(*decor)->setPosition(origin);
	return origin;
}

void SkyManager::setupStars(ABiome & biome, sf::Vector2f cameraSize)
{
	m_starCount = biome.getStarCount();
	m_originStars.resize(m_starCount);
	//TODO: Check with Iohann for delete element
	for (std::size_t i = 0u; i < m_starCount; i++)
		m_decorManagerBack.add(new Star(&m_timeCoef));

	for (std::size_t i = 0u; i < m_starCount; i++)
	{
		m_originStars[i].x = biome.randomFloat(-cameraSize.x, cameraSize.x);
		m_originStars[i].y = 0.f;
		float angle = biome.randomFloat(0.f, 360.f) * octo::Deg2Rad;
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		rotateVec(m_originStars[i], cos, sin);
	}
}

void SkyManager::setupSunAndMoon(sf::Vector2f cameraSize, sf::Vector2f cameraCenter)
{
	m_decorManagerBack.add(DecorManager::DecorTypes::Sun);
	m_decorManagerBack.add(DecorManager::DecorTypes::Moon);

	m_originRotate = sf::Vector2f(cameraCenter.x, cameraSize.y);
	m_originSun = sf::Vector2f(0.f, cameraSize.y);
	m_originMoon = cameraSize;
}

void SkyManager::setupClouds(ABiome & biome, sf::Vector2f const & cameraSize)
{
	m_cloudCount = biome.getCloudCount();
	m_originClouds.resize(m_cloudCount);
	for (size_t i = 0; i < m_cloudCount; i++)
	{
		m_decorManagerBack.add(DecorManager::DecorTypes::Cloud);
		m_originClouds[i].x = biome.randomFloat(0.f, m_mapSizeFloat.x);
		m_originClouds[i].y = biome.randomFloat(cameraSize.y / 2.f, -m_mapSizeFloat.y);
	}
}

void SkyManager::setup(ABiome & biome)
{
	m_decorManagerBack.setup(&biome);
	m_decorManagerFront.setup(&biome);

	octo::Camera camera = octo::Application::getCamera();
	sf::Vector2f cameraSize = camera.getSize();
	sf::Vector2f cameraCenter = camera.getCenter();

	m_mapSizeFloat = biome.getMapSizeFloat();
	m_wind = biome.getWind();

	setupStars(biome, cameraSize);
	setupSunAndMoon(cameraSize, cameraCenter);
	setupClouds(biome, cameraSize);
}

void SkyManager::update(sf::Time frameTime)
{
	m_timer += frameTime.asSeconds();
	if (m_timer >= m_timerMax)
		m_timer = 0.f;
	m_timeCoef = m_timer / m_timerMax;

	float angle = m_timeCoef * 360.f * octo::Deg2Rad;
	float cos = std::cos(angle);
	float sin = std::sin(angle);

	DecorManager::Iterator decor = m_decorManagerBack.begin();
	for (auto it = m_originStars.begin(); it != m_originStars.end(); it++)
		setPosition(decor++, *it + m_originRotateStar, m_originRotateStar, cos, sin);
	setPosition(decor++, m_originSun, m_originRotate, cos, sin);
	m_originRotateStar = setPosition(decor++, m_originMoon, m_originRotate, cos, sin);

	for (auto it = m_originClouds.begin(); it != m_originClouds.end(); it++)
	{
		it->x += m_wind * frameTime.asSeconds();
		//TODO: make a loop with position
		(*decor)->setPosition(*it);
		decor++;
	}

	m_decorManagerBack.update(frameTime, octo::Application::getCamera());
	m_decorManagerFront.update(frameTime, octo::Application::getCamera());
}

DecorManager const & SkyManager::getDecorsBack(void) const
{
	return m_decorManagerBack;
}

DecorManager const & SkyManager::getDecorsFront(void) const
{
	return m_decorManagerFront;
}

void SkyManager::rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle)
{
	float x = vector.x * cosAngle - vector.y * sinAngle;
	vector.y = vector.y * cosAngle + vector.x * sinAngle;
	vector.x = x;
}

void SkyManager::rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle)
{
	vector -= origin;
	rotateVec(vector, cosAngle, sinAngle);
	vector += origin;
}
