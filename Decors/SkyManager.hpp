#ifndef SKYMANAGER_HPP
# define SKYMANAGER_HPP

#include "DecorManager.hpp"

class ABiome;
class Star;

class SkyManager
{
public:
	SkyManager(void);
	~SkyManager(void) = default;

	void setup(ABiome & biome);
	void update(sf::Time frameTime);
	DecorManager const & getDecors(void) const;

private:
	sf::Vector2f setPosition(DecorManager::Iterator decor, sf::Vector2f origin, sf::Vector2f originRotate, float cos, float sin);
	void setupStars(ABiome & biome, sf::Vector2f cameraSize);
	void setupSunAndMoon(sf::Vector2f cameraSize, sf::Vector2f cameraCenter);
	void setupClouds(ABiome & biome);

	DecorManager				m_decorManager;
	float						m_wind;
	sf::Vector2f				m_mapSizeFloat;

	float						m_timeCoef;
	//TODO: Get from GameClock
	float						m_timer;
	float						m_timerMax;

	std::size_t					m_starCount;
	std::size_t					m_cloudCount;
	//TODO: To sync with paralax?
	sf::Vector2f				m_originRotate;
	sf::Vector2f				m_originSun;
	sf::Vector2f				m_originMoon;
	sf::Vector2f				m_originRotateStar;
	std::vector<sf::Vector2f>	m_originStars;
	std::vector<sf::Vector2f>	m_originClouds;

private:
	static void rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle);
	static void rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle);
};

#endif
