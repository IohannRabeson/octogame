#include "SkyManager.hpp"
#include "GameClock.hpp"
#include "ABiome.hpp"
#include "ADecor.hpp"
#include "Star.hpp"
#include "Sky.hpp"
#include "SunLight.hpp"

#include <Math.hpp>
#include <Application.hpp>
#include <Camera.hpp>

SkyManager::SkyManager(void) :
	//TODO:Estimate how much vertex we need
	m_decorManagerBack(200000),
	m_decorManagerFront(200000),
	m_wind(0.f),
	m_clock(nullptr),
	m_sunCount(0u),
	m_moonCount(0u),
	m_starCount(0u),
	m_cloudCount(0u)
{
}

sf::Vector2f SkyManager::setRotatePosition(DecorManager::Iterator decor, sf::Vector2f origin, sf::Vector2f const & originRotate, sf::Vector2f const & offsetCamera, float cos, float sin)
{
	rotateVec(origin, originRotate, cos, sin);
	(*decor)->setPosition(origin + offsetCamera);
	return origin;
}

void SkyManager::setupStars(ABiome & biome, sf::Vector2f const & cameraSize)
{
	if (biome.canCreateStar())
	{
		m_starCount = biome.getStarCount();
		m_originStars.resize(m_starCount);
		for (std::size_t i = 0u; i < m_starCount; i++)
			m_decorManagerBack.add(new Star(m_clock));

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
}

void SkyManager::setupSunAndMoon(ABiome & biome, sf::Vector2f const & cameraSize, sf::Vector2f const & cameraCenter)
{
	m_originRotate = sf::Vector2f(cameraCenter.x, cameraSize.y);
	float deltaPos = cameraSize.y / 3;

	if (biome.canCreateSun())
	{
		m_sunCount = biome.getSunCount();
		m_originSuns.resize(m_sunCount);
		for (std::size_t i = 0u; i < m_sunCount; i++)
		{
			m_decorManagerBack.add(DecorManager::DecorTypes::Sun);
			m_originSuns[i].x = 0.f + biome.randomFloat(0.f, deltaPos);
			m_originSuns[i].y = cameraSize.y;
			float angle = biome.randomFloat(-20.f, 20.f) * octo::Deg2Rad;
			float cos = std::cos(angle);
			float sin = std::sin(angle);
			rotateVec(m_originSuns[i], m_originRotate, cos, sin);
		}
	}
	if (biome.canCreateMoon())
	{
		m_moonCount = biome.getMoonCount();
		m_originMoons.resize(m_moonCount);
		for (std::size_t i = 0u; i < m_moonCount; i++)
		{
			m_decorManagerBack.add(DecorManager::DecorTypes::Moon);
			m_originMoons[i].x = cameraSize.x + biome.randomFloat(-deltaPos, 0.f);
			m_originMoons[i].y = cameraSize.y;
			float angle = biome.randomFloat(-20.f, 20.f) * octo::Deg2Rad;
			float cos = std::cos(angle);
			float sin = std::sin(angle);
			rotateVec(m_originMoons[i], m_originRotate, cos, sin);
		}
	}
}

void SkyManager::setupClouds(ABiome & biome, sf::Vector2f const & cameraSize, sf::Vector2f const & cameraCenter, sf::Vector2f const & mapSize)
{
	if (biome.canCreateCloud())
	{
		m_cloudCount = biome.getCloudCount();
		m_originCloudsFront.resize(m_cloudCount);
		float leftLimit = cameraCenter.x - cameraSize.x * 2.f;
		float rightLimit = cameraCenter.x + cameraSize.x * 2.f;
		for (size_t i = 0; i < m_cloudCount; i++)
		{
			m_decorManagerFront.add(DecorManager::DecorTypes::Cloud);
			m_originCloudsFront[i].x = biome.randomFloat(leftLimit, rightLimit);
			m_originCloudsFront[i].y = biome.randomFloat(cameraSize.y / 2.f, -mapSize.y);
		}
	}
}

void SkyManager::newRainCycle(ABiome & biome)
{
	biome.setWeather(0.f);
	m_sunnyTimer = sf::Time::Zero;
	m_sunnyTimerMax = biome.getSunnyTime();
	m_rainingTimer = sf::Time::Zero;
	m_rainingTimerMax = biome.getRainingTime() / 2.f;
	m_rainAppear = true;
	m_rainDropPerSecond = biome.getRainDropPerSecond();
}

void SkyManager::setup(ABiome & biome, GameClock & clock)
{
	m_decorManagerBack.setup(&biome);
	m_decorManagerFront.setup(&biome);
	m_clock = &clock;

	octo::Camera & camera = octo::Application::getCamera();
	sf::Vector2f const & cameraSize = camera.getSize();
	sf::Vector2f const & cameraCenter = camera.getCenter();

	m_mapSizeFloat = biome.getMapSizeFloat();
	m_wind = biome.getWind();

	m_decorManagerBack.add(new Sky(m_clock));
	setupStars(biome, cameraSize);
	setupSunAndMoon(biome, cameraSize, cameraCenter);
	setupClouds(biome, cameraSize, cameraCenter, m_mapSizeFloat);
	m_decorManagerFront.add(new SunLight(m_clock));

	newRainCycle(biome);
}

void SkyManager::computeRain(sf::Time frameTime, ABiome & biome)
{
	if (m_sunnyTimer <= m_sunnyTimerMax)
		m_sunnyTimer += frameTime;
	else if (m_rainingTimer <= m_rainingTimerMax && m_rainAppear)
	{
		m_rainingTimer += frameTime;
		biome.setWeather(m_rainingTimer / m_rainingTimerMax * m_rainDropPerSecond);
	}
	else if (m_rainingTimer >= sf::Time::Zero)
	{
		m_rainAppear = false;
		m_rainingTimer -= frameTime;
		biome.setWeather(m_rainingTimer / m_rainingTimerMax * m_rainDropPerSecond);
	}
	else
		newRainCycle(biome);
}

void SkyManager::update(sf::Time frameTime, ABiome & biome)
{
	octo::Camera const & camera = octo::Application::getCamera();
	sf::FloatRect const & rec = camera.getRectangle();
	sf::Vector2f cameraCenter = camera.getCenter();
	sf::Vector2f cameraSize = camera.getSize();
	sf::Vector2f offsetCamera(rec.left, rec.top);
	float angle = m_clock->getCycleValue() * 360.f * octo::Deg2Rad;
	float cos = std::cos(angle);
	float sin = std::sin(angle);

	DecorManager::Iterator decorBack = m_decorManagerBack.begin();
	(*decorBack)->setPosition(camera.getCenter());
	decorBack++;
	for (auto it = m_originStars.begin(); it != m_originStars.end(); it++)
		setRotatePosition(decorBack++, *it + m_originRotateStar, m_originRotateStar, offsetCamera, cos, sin);
	for (auto it = m_originSuns.begin(); it != m_originSuns.end(); it++)
		setRotatePosition(decorBack++, *it, m_originRotate, offsetCamera, cos, sin);
	for (auto it = m_originMoons.begin(); it != m_originMoons.end(); it++)
		m_originRotateStar = setRotatePosition(decorBack++, *it, m_originRotate, offsetCamera, cos, sin);

	float windMove = m_wind * frameTime.asSeconds();
	float leftLimit = cameraCenter.x - cameraSize.x * 2.f;
	float rightLimit = cameraCenter.x + cameraSize.x * 2.f;
	DecorManager::Iterator decorFront = m_decorManagerFront.begin();
	for (auto it = m_originCloudsFront.begin(); it != m_originCloudsFront.end(); it++)
	{
		it->x += windMove;
		if (it->x >= rightLimit)
			it->x = leftLimit;
		else if (it->x <= leftLimit)
			it->x = rightLimit;
		(*decorFront)->setPosition(*it);
		decorFront++;
	}
	(*decorFront)->setPosition(camera.getCenter());
	m_decorManagerBack.update(frameTime, camera);
	m_decorManagerFront.update(frameTime, camera);

	computeRain(frameTime, biome);
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
