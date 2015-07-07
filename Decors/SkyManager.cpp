#include "SkyManager.hpp"
#include "ABiome.hpp"
#include "ADecor.hpp"
#include "Star.hpp"

#include <Math.hpp>
#include <Application.hpp>
#include <Camera.hpp>

SkyManager::SkyManager(void) :
	m_timer(0.f),
	m_timerMax(30.f),
	m_starCount(600u)
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
	//TODO: Check with Iohann for delete element
	for (std::size_t i = 0u; i < m_starCount; i++)
		m_decorManager->add(new Star(&m_timeCoef));

	m_originStars.resize(m_starCount);

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
	m_decorManager->add(DecorManager::DecorTypes::Sun);
	m_decorManager->add(DecorManager::DecorTypes::Moon);

	m_originRotate = sf::Vector2f(cameraCenter.x, cameraSize.y);
	m_originSun = sf::Vector2f(0.f, cameraSize.y);
	m_originMoon = cameraSize;
}

void SkyManager::setup(ABiome & biome, DecorManager * decorManager)
{
	m_decorManager = decorManager;
	m_decorManager->setup(&biome);

	octo::Camera camera = octo::Application::getCamera();
	sf::Vector2f cameraSize = camera.getSize();
	sf::Vector2f cameraCenter = camera.getCenter();

	setupStars(biome, cameraSize);
	setupSunAndMoon(cameraSize, cameraCenter);
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

	DecorManager::Iterator decor = m_decorManager->begin();
	for (std::size_t i = 0; i < m_starCount; i++)
	{
		setPosition(decor, m_originStars[i] + m_originRotateStar, m_originRotateStar, cos, sin);
		decor++;
	}
	setPosition(decor, m_originSun, m_originRotate, cos, sin);
	decor++;
	m_originRotateStar = setPosition(decor, m_originMoon, m_originRotate, cos, sin);

	m_decorManager->update(frameTime, octo::Application::getCamera());
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
