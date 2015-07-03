#include "DefaultBiome.hpp"

#include <iostream>

DefaultBiome::DefaultBiome() :
	m_name("Default Biome"),
	m_mapSize(sf::Vector2f(512u, 128u)),
	m_transitionDuration(0.5f),
	m_bossInstancePosX(m_mapSize.x / 2.f),

	m_groundDecorsCount(10u, 20u),
	m_crystalsCount(30u, 50u),
	m_skyDecorsCount(30u, 50u),

	m_canCreateTree(true),
	m_canCreateLeaf(true),
	m_treeIsMoving(true),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(true),
	m_canCreateRock(true),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),

	m_treeDepth(6u, 8u),
	m_treeSize(sf::Vector2f(20.f, 100.f), sf::Vector2f(40.f, 200.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(255, 105, 180),
	m_treeAngle(15.f, 75.f),
	m_leafSize(sf::Vector2f(50.f, 50.f), sf::Vector2f(200.f, 200.f)),
	m_leafColor(100, 105, 180),

	m_crystalSize(sf::Vector2f(10.f, 50.f), sf::Vector2f(25.f, 150.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(255, 105, 180),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_rockSize(sf::Vector2f(10.f, 150.f), sf::Vector2f(40.f, 400.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(255, 0, 100),

	m_cloudSize(sf::Vector2f(300.f, 10.f), sf::Vector2f(500.f, 40.f)),
	m_cloudPartCount(2u, 10u),
	m_cloudLifeTime(sf::seconds(15), sf::seconds(60)),
	m_cloudColor(255, 255, 255),

	m_starSize(sf::Vector2f(50.f, 50.f), sf::Vector2f(100.f, 100.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(60)),

	m_sunSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 162, 0),

	m_moonSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_moonColor(0, 111, 255)
{
	m_generator.setSeed("default_biome");
}

void			DefaultBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		DefaultBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	DefaultBiome::getMapSize()
{
	return (m_mapSize);
}

float			DefaultBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DefaultBiome::getBossInstancePosX(void)
{
	return (m_bossInstancePosX);
}

std::size_t		DefaultBiome::getGroundDecorsCount()
{
	return (randomRangeInt(m_groundDecorsCount));
}

std::size_t		DefaultBiome::getCrystalsCount()
{
	return (randomRangeInt(m_crystalsCount));
}

std::size_t		DefaultBiome::getSkyDecorsCount()
{
	return (randomRangeInt(m_skyDecorsCount));
}

std::size_t	DefaultBiome::getTreeDepth()
{
	return (randomRangeInt(m_treeDepth));
}

sf::Vector2f	DefaultBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		DefaultBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		DefaultBiome::getTreeColor()
{
	return (m_treeColor);
}

float			DefaultBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			DefaultBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

bool			DefaultBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			DefaultBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	DefaultBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DefaultBiome::getLeafColor()
{
	return (m_leafColor);
}

sf::Vector2f	DefaultBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		DefaultBiome::getCrystalPartCount()
{
	return (randomRangeInt(m_crystalPartCount));
}

sf::Color		DefaultBiome::getCrystalColor()
{
	return (m_crystalColor);
}

int				DefaultBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_bossInstancePosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			DefaultBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	DefaultBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		DefaultBiome::getShineEffectColor()
{
	return (m_shineEffectColor);
}

float			DefaultBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			DefaultBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	DefaultBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		DefaultBiome::getRockPartCount()
{
	return (randomRangeInt(m_rockPartCount));
}

sf::Color		DefaultBiome::getRockColor()
{
	return (m_rockColor);
}

bool			DefaultBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	DefaultBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		DefaultBiome::getCloudPartCount()
{
	return (randomRangeInt(m_cloudPartCount));
}

sf::Time		DefaultBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		DefaultBiome::getCloudColor()
{
	return (m_cloudColor);
}

bool			DefaultBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	DefaultBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		DefaultBiome::getStarColor()
{
	return (m_starColor);
}

sf::Time		DefaultBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			DefaultBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	DefaultBiome::getSunSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		DefaultBiome::getSunPartCount()
{
	return (randomRangeInt(m_sunPartCount));
}

sf::Color		DefaultBiome::getSunColor()
{
	return (m_sunColor);
}

bool			DefaultBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	DefaultBiome::getMoonSize()
{
	return (randomRangeVector2f(m_moonSize));
}

sf::Color		DefaultBiome::getMoonColor()
{
	return (m_moonColor);
}

bool			DefaultBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DefaultBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				DefaultBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			DefaultBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			DefaultBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				DefaultBiome::randomRangeInt(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	DefaultBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		DefaultBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.min.asMicroseconds())));
}
