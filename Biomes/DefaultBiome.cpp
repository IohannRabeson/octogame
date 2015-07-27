#include "DefaultBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include <Interpolations.hpp>

#include <iostream>

DefaultBiome::DefaultBiome() :
	m_name("Default Biome"),
	m_mapSize(sf::Vector2f(512u, 128u)),
	m_transitionDuration(0.5f),
	m_bossInstancePosX(m_mapSize.x / 2.f),

	m_dayDuration(sf::seconds(40.f)),
	m_skyDayColor(188, 200, 206),
	m_skyNightColor(8, 20, 26),
	m_nightLightColor(0, 197, 255, 100),
	m_SunsetLightColor(238, 173, 181, 100),
	m_wind(100.f, 150.f),

	m_rockCount(10u, 20u),
	m_treeCount(5u, 10u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(10u, 15u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 3u),
	m_moonCount(1u, 3u),
	m_rainbowCount(3u, 6u),
	m_cloudCount(20u, 40u),
	m_groundRockCount(100u, 200u),

	m_canCreateRock(true),
	m_canCreateTree(true),
	m_canCreateLeaf(true),
	m_treeIsMoving(true),
	m_canCreateMushroom(true),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(true),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),

	m_rockSize(sf::Vector2f(10.f, 100.f), sf::Vector2f(40.f, 200.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(107, 172, 166),

	m_treeDepth(6u, 8u),
	m_treeSize(sf::Vector2f(15.f, 60.f), sf::Vector2f(30.f, 150.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(30, 30, 30),
	m_treeAngle(15.f, 75.f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(100.f, 100.f)),
	m_leafColor(143, 208, 202),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(107, 172, 166),
	m_mushroomLifeTime(sf::seconds(20), sf::seconds(60)),

	m_crystalSize(sf::Vector2f(10.f, 50.f), sf::Vector2f(25.f, 100.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(230, 168, 0, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(200.f, 100.f), sf::Vector2f(400.f, 200.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 200),

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

	m_instances[12] = MINIMAP_OMP;
	m_instances[86] = TEST_MAP2_OMP;
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

std::map<std::size_t, std::string> const & DefaultBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> DefaultBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(sf::Color(185, 185, 30), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -20, 0.1f, 1.f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlinNoise(x * 10.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(sf::Color(170, 170, 70), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -10, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlinNoise(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(sf::Color(180, 180, 110), sf::Vector2f(0.6f, 0.2f), mapSize, 12.f, -10, 0.2f, 0.8f, 6.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 1.1f, y);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator DefaultBiome::getMapSurfaceGenerator()
{
	return [](Noise & noise, float x, float y)
	{
		return noise.fBm(x, y, 3, 3.f, 0.3f);
	};
}

Map::TileColorGenerator DefaultBiome::getTileColorGenerator()
{
	return [](Noise & noise, float x, float y, float z)
	{
		static const sf::Color end = sf::Color(254, 231, 170);
		static const sf::Color start = sf::Color(230, 168, 0);

		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		return octo::linearInterpolation(start, end, transition);
	};
}

sf::Time		DefaultBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Color		DefaultBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		DefaultBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		DefaultBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		DefaultBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			DefaultBiome::getWind()
{
	return (randomRangeFloat(m_wind));
}

std::size_t		DefaultBiome::getRockCount()
{
	return (randomRangeInt(m_rockCount));
}

std::size_t		DefaultBiome::getTreeCount()
{
	return (randomRangeInt(m_treeCount));
}

std::size_t		DefaultBiome::getMushroomCount()
{
	return (randomRangeInt(m_mushroomCount));
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

std::size_t		DefaultBiome::getGroundRockCount()
{
	return (randomRangeInt(m_groundRockCount));
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

sf::Vector2f	DefaultBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		DefaultBiome::getMushroomColor()
{
	return (m_mushroomColor);
}

sf::Time		DefaultBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			DefaultBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
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
