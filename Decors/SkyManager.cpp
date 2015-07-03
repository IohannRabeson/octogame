#include "SkyManager.hpp"
#include "ABiome.hpp"
#include "ADecor.hpp"

//TODO:: To change with new system
#include "RandomGenerator.hpp"

#include <Math.hpp>
#include <Application.hpp>
#include <Camera.hpp>

SkyManager::SkyManager(void) :
	m_timer(0.f),
	m_timerMax(30.f),
	m_starCount(600u)
{
}

sf::Vector2f SkyManager::computePosition(DecorManager::Iterator decor, sf::Vector2f origin, sf::Vector2f originRotate, float cos, float sin)
{
	rotateVec(origin, originRotate, cos, sin);
	(*decor)->setPosition(origin);
	return origin;
}

void SkyManager::setup(ABiome & biome, DecorManager * decorManager)
{
	m_decorManager = decorManager;
	m_decorManager->setup(&biome);

	m_decorManager->add(DecorManager::DecorTypes::Sun);
	//TODO: To change for moon
	m_decorManager->add(DecorManager::DecorTypes::Sun);
	for (std::size_t i = 0u; i < m_starCount; i++)
		m_decorManager->add(DecorManager::DecorTypes::Star);

	octo::Camera camera = octo::Application::getCamera();
	sf::Vector2f cameraSize = camera.getSize();
	sf::Vector2f cameraCenter = camera.getCenter();

	m_originRotate = sf::Vector2f(cameraCenter.x, cameraSize.y);
	m_originSun = sf::Vector2f(0.f, cameraSize.y);
	m_originMoon = cameraSize;
	m_originStars.resize(m_starCount);

	for (std::size_t i = 0u; i < m_starCount; i++)
	{
		m_originStars[i].x = RandomGenerator::randomFloat(-cameraSize.x, cameraSize.x);
		m_originStars[i].y = 0.f;
		float angle = RandomGenerator::randomFloat(0.f, 360.f) * octo::Deg2Rad;
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		rotateVec(m_originStars[i], cos, sin);
	}
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
	computePosition(decor, m_originSun, m_originRotate, cos, sin);
	decor++;
	m_originRotateStar = computePosition(decor, m_originMoon, m_originRotate, cos, sin);
	for (std::size_t i = 0; i < m_starCount; i++)
	{
		decor++;
		computePosition(decor, m_originStars[i] + m_originRotateStar, m_originRotateStar, cos, sin);
	}

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
