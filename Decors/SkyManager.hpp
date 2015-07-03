#ifndef SKYMANAGER_HPP
# define SKYMANAGER_HPP

#include "DecorManager.hpp"

class ABiome;

class SkyManager
{
public:
	SkyManager(void);
	~SkyManager(void) = default;

	void setup(ABiome & biome, DecorManager * decorManager);
	void update(sf::Time frameTime);

private:
	sf::Vector2f computePosition(DecorManager::Iterator decor, sf::Vector2f origin, sf::Vector2f originRotate, float cos, float sin);

	DecorManager *				m_decorManager;

	float						m_timeCoef;
	//TODO: Get from GameClock
	float						m_timer;
	float						m_timerMax;

	std::size_t					m_starCount;
	//TODO: To sync with paralax?
	sf::Vector2f				m_originRotate;
	sf::Vector2f				m_originSun;
	sf::Vector2f				m_originMoon;
	sf::Vector2f				m_originRotateStar;
	std::vector<sf::Vector2f>	m_originStars;

private:
	static void rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle);
	static void rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle);
};

#endif
