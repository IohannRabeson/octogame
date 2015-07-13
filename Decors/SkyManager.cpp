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
}

void SkyManager::setupSunAndMoon(ABiome & biome, sf::Vector2f const & cameraSize, sf::Vector2f const & cameraCenter)
{
	m_originRotate = sf::Vector2f(cameraCenter.x, cameraSize.y);

	if (biome.canCreateSun())
	{
		m_sunCount = biome.getSunCount();
		m_originSuns.resize(m_sunCount);
		for (std::size_t i = 0u; i < m_sunCount; i++)
		{
			m_decorManagerBack.add(DecorManager::DecorTypes::Sun);
			m_originSuns[i] = sf::Vector2f(0.f, cameraSize.y);
			float angle = biome.randomFloat(-20.f, 20.f) * octo::Deg2Rad;
			float cos = std::cos(angle);
			float sin = std::sin(angle);
			rotateVec(m_originSuns[i], cos, sin);
		}
	}
	if (biome.canCreateMoon())
	{
		m_moonCount = biome.getMoonCount();
		m_originMoons.resize(m_moonCount);
		for (std::size_t i = 0u; i < m_moonCount; i++)
		{
			m_decorManagerBack.add(DecorManager::DecorTypes::Moon);
			m_originMoons[i] = cameraSize;
			float angle = biome.randomFloat(-20.f, 20.f) * octo::Deg2Rad;
			float cos = std::cos(angle);
			float sin = std::sin(angle);
			rotateVec(m_originMoons[i], cos, sin);
		}
	}
}

void SkyManager::setupClouds(ABiome & biome, sf::Vector2f const & cameraSize)
{
	if (biome.canCreateCloud())
	{
		m_cloudCount = biome.getCloudCount();
		m_originCloudsBack.resize(m_cloudCount);
		for (size_t i = 0; i < m_cloudCount; i++)
		{
			m_decorManagerBack.add(DecorManager::DecorTypes::Cloud);
			m_originCloudsBack[i].x = biome.randomFloat(0.f, m_mapSizeFloat.x);
			m_originCloudsBack[i].y = biome.randomFloat(cameraSize.y / 2.f, -m_mapSizeFloat.y);
		}
		m_originCloudsFront.resize(m_cloudCount);
		for (size_t i = 0; i < m_cloudCount; i++)
		{
			m_decorManagerFront.add(DecorManager::DecorTypes::Cloud);
			m_originCloudsFront[i].x = biome.randomFloat(0.f, m_mapSizeFloat.x);
			m_originCloudsFront[i].y = biome.randomFloat(cameraSize.y / 2.f, -m_mapSizeFloat.y);
		}
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
	setupSunAndMoon(biome, cameraSize, cameraCenter);
	setupClouds(biome, cameraSize);
}

void SkyManager::update(sf::Time frameTime)
{
	// To move in GameClock
	m_timer += frameTime.asSeconds();
	if (m_timer >= m_timerMax)
		m_timer = 0.f;
	m_timeCoef = m_timer / m_timerMax;

	octo::Camera camera = octo::Application::getCamera();
	sf::FloatRect rec = camera.getRectangle();
	sf::Vector2f offsetCamera(rec.left, rec.top);
	float angle = m_timeCoef * 360.f * octo::Deg2Rad;
	float cos = std::cos(angle);
	float sin = std::sin(angle);

	DecorManager::Iterator decorBack = m_decorManagerBack.begin();
	for (auto it = m_originStars.begin(); it != m_originStars.end(); it++)
		setRotatePosition(decorBack++, *it + m_originRotateStar, m_originRotateStar, offsetCamera, cos, sin);
	for (auto it = m_originSuns.begin(); it != m_originSuns.end(); it++)
		setRotatePosition(decorBack++, *it, m_originRotate, offsetCamera, cos, sin);
	for (auto it = m_originMoons.begin(); it != m_originMoons.end(); it++)
		m_originRotateStar = setRotatePosition(decorBack++, *it, m_originRotate, offsetCamera, cos, sin);

	for (auto it = m_originCloudsBack.begin(); it != m_originCloudsBack.end(); it++)
	{
		it->x += m_wind * frameTime.asSeconds();
		//TODO: make a loop with position (ju?)
		(*decorBack)->setPosition(*it);
		decorBack++;
	}
	DecorManager::Iterator decorFront = m_decorManagerFront.begin();
	for (auto it = m_originCloudsFront.begin(); it != m_originCloudsFront.end(); it++)
	{
		it->x += m_wind * frameTime.asSeconds();
		//TODO: make a loop with position (ju?)
		(*decorFront)->setPosition(*it);
		decorFront++;
	}
	m_decorManagerBack.update(frameTime, camera);
	m_decorManagerFront.update(frameTime, camera);
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
