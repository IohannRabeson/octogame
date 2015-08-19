#ifndef SKYMANAGER_HPP
# define SKYMANAGER_HPP

#include "DecorManager.hpp"

class SkyCycle;
class ABiome;
class Star;

class SkyManager
{
public:
	SkyManager(void);
	~SkyManager(void) = default;

	void setup(ABiome & biome, SkyCycle & cycle);
	void update(sf::Time frameTime);
	DecorManager const & getDecorsBack(void) const;
	DecorManager const & getDecorsFront(void) const;

private:
	sf::Vector2f setRotatePosition(DecorManager::Iterator decor,
									sf::Vector2f origin,
									sf::Vector2f const & originRotate,
									sf::Vector2f const & offsetCamera,
									float cos,
									float sin);

	void setupStars(ABiome & biome,
					sf::Vector2f const & cameraSize);

	void setupSunAndMoon(ABiome & biome,
						sf::Vector2f const & cameraSize,
						sf::Vector2f const & cameraCenter);

	void setupClouds(ABiome & biome,
					sf::Vector2f const & cameraSize,
					sf::Vector2f const & cameraCenter,
					sf::Vector2f const & mapSize);

	DecorManager				m_decorManagerBack;
	DecorManager				m_decorManagerFront;
	float						m_wind;
	sf::Vector2f				m_mapSizeFloat;

	SkyCycle *					m_cycle;

	std::size_t					m_sunCount;
	std::size_t					m_moonCount;
	std::size_t					m_starCount;
	std::size_t					m_cloudCount;

	sf::Vector2f				m_originRotate;
	std::vector<sf::Vector2f>	m_originSuns;
	std::vector<sf::Vector2f>	m_originMoons;
	sf::Vector2f				m_originRotateStar;
	std::vector<sf::Vector2f>	m_originStars;
	std::vector<sf::Vector2f>	m_originCloudsBack;
	std::vector<sf::Vector2f>	m_originCloudsFront;

private:
	static void rotateVec(sf::Vector2f & vector, float const cosAngle, float const sinAngle);
	static void rotateVec(sf::Vector2f & vector, sf::Vector2f const & origin, float const cosAngle, float const sinAngle);
};

#endif
