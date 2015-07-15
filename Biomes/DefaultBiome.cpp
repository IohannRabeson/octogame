#include "DefaultBiome.hpp"
#include "Tile.hpp"

#include <iostream>

DefaultBiome::DefaultBiome() :
	m_name("Default Biome"),
	m_mapSize(sf::Vector2f(512u, 128u)),
	m_transitionDuration(0.5f),
	m_bossInstancePosX(m_mapSize.x / 2.f),

	m_dayDuration(sf::seconds(15.f)),
	m_nightDuration(sf::seconds(15.f)),
	m_wind(50.f, 100.f),

	m_groundDecorsCount(15u, 30u),
	m_crystalCount(10u, 15u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 3u),
	m_moonCount(1u, 3u),
	m_rainbowCount(3u, 6u),
	m_cloudCount(20u, 40u),

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
	m_canCreateRainbow(false),

	m_treeDepth(6u, 8u),
	m_treeSize(sf::Vector2f(15.f, 60.f), sf::Vector2f(30.f, 150.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(30, 30, 30),
	m_treeAngle(15.f, 75.f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(150.f, 150.f)),
	m_leafColor(143, 208, 202),

	m_crystalSize(sf::Vector2f(10.f, 50.f), sf::Vector2f(25.f, 100.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(230, 168, 0, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_rockSize(sf::Vector2f(10.f, 100.f), sf::Vector2f(40.f, 200.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(107, 172, 166),

	m_cloudSize(sf::Vector2f(100.f, 10.f), sf::Vector2f(400.f, 60.f)),
	m_cloudPartCount(2u, 10u),
	m_cloudLifeTime(sf::seconds(15), sf::seconds(60)),
	m_cloudColor(255, 255, 255),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(60.f, 60.f), sf::Vector2f(150.f, 150.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 200),

	m_moonSize(sf::Vector2f(50.f, 30.f), sf::Vector2f(100.f, 100.f)),
	m_moonColor(200, 200, 200),
	m_moonLifeTime(sf::seconds(15.f), sf::seconds(30.f)),

	m_rainbowThickness(50.f, 100.f),
	m_rainbowPartSize(50.f, 200.f),
	m_rainbowLoopCount(1u, 5u),
	m_rainbowGrowTime(sf::seconds(4.f), sf::seconds(8.f))
{
	m_generator.setSeed(m_name);
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

sf::Vector2f	DefaultBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

float			DefaultBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DefaultBiome::getBossInstancePosX()
{
	return (m_bossInstancePosX);
}

sf::Time		DefaultBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		DefaultBiome::getNightDuration()
{
	return (m_nightDuration);
}

float			DefaultBiome::getWind()
{
	return (randomRangeFloat(m_wind));
}

std::size_t		DefaultBiome::getGroundDecorsCount()
{
	return (randomRangeInt(m_groundDecorsCount));
}

std::size_t		DefaultBiome::getCrystalCount()
{
	return (randomRangeInt(m_crystalCount));
}

std::size_t		DefaultBiome::getStarCount()
{
	return (randomRangeInt(m_starCount));
}

std::size_t		DefaultBiome::getSunCount()
{
	return (randomRangeInt(m_sunCount));
}

std::size_t		DefaultBiome::getMoonCount()
{
	return (randomRangeInt(m_moonCount));
}

std::size_t		DefaultBiome::getRainbowCount()
{
	return (randomRangeInt(m_rainbowCount));
}

std::size_t		DefaultBiome::getCloudCount()
{
	return (randomRangeInt(m_cloudCount));
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
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
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
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DefaultBiome::getMoonColor()
{
	return (m_moonColor);
}

sf::Time		DefaultBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			DefaultBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DefaultBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			DefaultBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		DefaultBiome::getRainbowLoopCount()
{
	return (randomRangeInt(m_rainbowLoopCount));
}

sf::Time		DefaultBiome::getRainbowGrowTime()
{
	return (randomRangeTime(m_rainbowGrowTime));
}

bool			DefaultBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
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

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}
