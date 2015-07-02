#include "TestBiome.hpp"

#include <iostream>

TestBiome::TestBiome() :
	m_groundDecorsCount(50u),
	m_crystalsCount(20u),
	m_skyDecorsCount(40u),
	m_treeDepth(8u),
	m_treeSize(30.f, 200.f),
	m_treeLifeTime(sf::seconds(1)),
	m_treeColor(255, 105, 180),
	m_treeAngle(45.f),
	m_treeIsMoving(true),
	m_canCreateTree(true),
	m_canCreateLeaf(true),
	m_leafSize(150.f, 150.f),
	m_leafColor(0, 105, 180),
	m_crystalSize(10.f, 100.f),
	m_crystalPartCount(4u),
	m_crystalColor(255, 105, 180),
	m_canCreateCrystal(true),
	m_shineEffectSize(150.f, 150.f),
	m_shineEffectColor(255,255,255,100),
	m_shineEffectRotateAngle(150.f),
	m_canCreateShineEffect(true),
	m_rockSize(300.f, 300.f),
	m_rockPartCount(5u),
	m_rockColor(255, 105, 180),
	m_canCreateRock(true),
	m_cloudSize(400.f, 200.f),
	m_cloudPartCount(5u),
	m_cloudColor(255, 105, 180),
	m_canCreateCloud(true),
	m_starSize(200.f, 200.f),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(20.f)),
	m_canCreateStar(true),
	m_sunSize(200.f, 200.f),
	m_sunPartCount(3u),
	m_sunColor(255, 105, 180),
	m_canCreateSun(true),
	m_moonColor(255, 105, 180),
	m_canCreateMoon(true),
	m_mapSize(512u, 128u),
	m_transitionDuration(0.5f),
	m_bossInstancePosX(0u)
{
	m_generator.setSeed("test_biome");
}

void			TestBiome::setup(std::size_t seed)
{
	std::cout << "Test biome setup...\n";
	static_cast<void>(seed);
}

std::string		TestBiome::getName()const
{
	return ("Biome Test");
}

void			TestBiome::setSeed(std::string string)
{
	m_generator.setSeed(string);
}

float			TestBiome::randomFloat(float min, float max)
{
	return m_generator.randomFloat(min, max);
}

int				TestBiome::randomInt(int min, int max)
{
	return m_generator.randomInt(min, max);
}

bool			TestBiome::randomBool(float percent)
{
	return m_generator.randomBool(percent);
}

std::size_t		TestBiome::getGroundDecorsCount()
{
	return (m_groundDecorsCount);
}

std::size_t		TestBiome::getCrystalsCount()
{
	return (m_crystalsCount);
}

std::size_t		TestBiome::getSkyDecorsCount()
{
	return (m_skyDecorsCount);
}

std::size_t	TestBiome::getTreeDepth()
{
	return (m_treeDepth);
}

sf::Vector2f	TestBiome::getTreeSize()
{
	return (m_treeSize);
}

sf::Time		TestBiome::getTreeLifeTime()
{
	return (m_treeLifeTime);
}

sf::Color		TestBiome::getTreeColor()
{
	return (m_treeColor);
}

float			TestBiome::getTreeAngle()
{
	return (m_treeAngle);
}

bool			TestBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

bool			TestBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			TestBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	TestBiome::getLeafSize()
{
	return (m_leafSize);
}

sf::Color		TestBiome::getLeafColor()
{
	return (m_leafColor);
}

sf::Vector2f	TestBiome::getCrystalSize()
{
	return (m_crystalSize);
}

std::size_t		TestBiome::getCrystalPartCount()
{
	return (m_crystalPartCount);
}

sf::Color		TestBiome::getCrystalColor()
{
	return (m_crystalColor);
}

int				TestBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_bossInstancePosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			TestBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	TestBiome::getShineEffectSize()
{
	return (m_shineEffectSize);
}

sf::Color		TestBiome::getShineEffectColor()
{
	return (m_shineEffectColor);
}

float			TestBiome::getShineEffectRotateAngle()
{
	return m_shineEffectRotateAngle;
}

bool			TestBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	TestBiome::getRockSize()
{
	return (m_rockSize);
}

std::size_t		TestBiome::getRockPartCount()
{
	return (m_rockPartCount);
}

sf::Color		TestBiome::getRockColor()
{
	return (m_rockColor);
}

bool			TestBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	TestBiome::getCloudSize()
{
	return (m_cloudSize);
}

std::size_t		TestBiome::getCloudPartCount()
{
	return (m_cloudPartCount);
}

sf::Time		TestBiome::getCloudLifeTime()
{
	return (m_cloudLifeTime);
}

sf::Color		TestBiome::getCloudColor()
{
	return (m_cloudColor);
}

bool			TestBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	TestBiome::getStarSize()
{
	return (m_starSize);
}

sf::Color		TestBiome::getStarColor()
{
	return (m_starColor);
}

sf::Time		TestBiome::getStarLifeTime()
{
	return (m_starLifeTime);
}

bool			TestBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	TestBiome::getSunSize()
{
	return (m_sunSize);
}

std::size_t		TestBiome::getSunPartCount()
{
	return (m_sunPartCount);
}

sf::Color		TestBiome::getSunColor()
{
	return (m_sunColor);
}

bool			TestBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	TestBiome::getMoonSize()
{
	return (m_moonSize);
}

sf::Color		TestBiome::getMoonColor()
{
	return (m_moonColor);
}

bool			TestBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

sf::Vector2u const & TestBiome::getMapSize()
{
	return m_mapSize;
}

float TestBiome::getTransitionDuration()
{
	return m_transitionDuration;
}

int		TestBiome::getBossInstancePosX(void)
{
	return m_bossInstancePosX;
}

