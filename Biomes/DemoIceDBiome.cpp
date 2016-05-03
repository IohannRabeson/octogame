#include "DemoIceDBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

DemoIceDBiome::DemoIceDBiome() :
	m_name("Ice D"),
	m_id(Level::DemoIceD),
	m_seed("Level_One"),
	m_mapSize(sf::Vector2u(600u, 1u)),
	m_mapSeed(42u),
	m_octoStartPosition(16.f * 37.f, -500.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(227, 227, 227),
	m_tileEndColor(27, 79, 101),
	m_waterLevel(-1.f),
	m_waterColor(255, 255, 255, 200),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(65.f)),
	m_startDayDuration(sf::seconds(9.f)),
	m_skyDayColor(8, 20, 26),
	m_skyNightColor(8, 20, 26),
	m_nightLightColor(sf::Color::Transparent),
	m_sunsetLightColor(sf::Color::Transparent),
	m_wind(200.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(7u, 8u),
	m_treeCount(10u, 10u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(10u, 20u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 3u),
	m_moonCount(2u, 2u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(40u, 60u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(true),
	m_canCreateRock(true),
	m_canCreateTree(true),
	m_canCreateLeaf(false),
	m_treeIsMoving(false),
	m_canCreateMushroom(false),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(false),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(false),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),

	m_rockSize(sf::Vector2f(50.f, 100.f), sf::Vector2f(100.f, 200.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(0, 31, 63),

	m_treeDepth(5u, 7u),
	m_treeSize(sf::Vector2f(20.f, 150.f), sf::Vector2f(50.f, 250.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(158, 176, 183),
	m_treeAngle(15.f, 75.f),
	m_treeBeatMouvement(0.f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(100.f, 100.f)),
	m_leafColor(143, 208, 202),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(20), sf::seconds(60)),

	m_crystalSize(sf::Vector2f(20.f, 100.f), sf::Vector2f(50.f, 250.f)),
	m_crystalPartCount(3u, 6u),
	m_crystalColor(227, 227, 227, 150),
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

	m_moonSize(sf::Vector2f(70.f, 50.f), sf::Vector2f(150.f, 150.f)),
	m_moonColor(200, 200, 200),
	m_moonLifeTime(sf::seconds(15.f), sf::seconds(30.f)),

	m_rainbowThickness(70.f, 120.f),
	m_rainbowPartSize(50.f, 200.f),
	m_rainbowLoopCount(1u, 5u),
	m_rainbowLifeTime(sf::seconds(6.f), sf::seconds(10.f)),
	m_rainbowIntervalTime(sf::seconds(1.f), sf::seconds(2.f))
{
	m_generator.setSeed(m_seed);
#ifndef NDEBUG
	m_mapSeed = 42u;
#else
	m_mapSeed = 42;//m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	m_interestPointPosX = 530;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::DesertA)
		m_octoStartPosition = sf::Vector2f(16.f * 357.f, -500.f);

	m_gameObjects[30] = GameObjectType::Portal;
	m_gameObjects[100] = GameObjectType::HouseFlatSnow;
	m_gameObjects[140] = GameObjectType::Snowman1Npc;
	m_gameObjects[190] = GameObjectType::BirdBlueNpc;
	m_gameObjects[270] = GameObjectType::EngineSnow;
	m_gameObjects[310] = GameObjectType::StrangerSnowNpc;
	m_gameObjects[340] = GameObjectType::BirdBlueNpc;
	m_gameObjects[355] = GameObjectType::Portal;
	m_gameObjects[400] = GameObjectType::SnowGirl2Npc;
	m_gameObjects[450] = GameObjectType::WeirdHouseSnow;
	m_gameObjects[475] = GameObjectType::StrangerGirlSnowNpc;
	m_gameObjects[510] = GameObjectType::BirdBlueNpc;
	m_gameObjects[570] = GameObjectType::EngineSnow;
	m_destinations.push_back(Level::DemoIceB);
	m_destinations.push_back(Level::DemoDesertA);
}

void			DemoIceDBiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			DemoIceDBiome::getId()const
{
	return m_id;
}

std::string		DemoIceDBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	DemoIceDBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		DemoIceDBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	DemoIceDBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	DemoIceDBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			DemoIceDBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DemoIceDBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	DemoIceDBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	DemoIceDBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	DemoIceDBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	DemoIceDBiome::getWaterColor()
{
	return m_waterColor;
}

std::map<std::size_t, std::string> const & DemoIceDBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> DemoIceDBiome::getLayers()
{
	//sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	/*
	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -20, 0.1f, 1.f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 10.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -10, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.6f, 0.2f), mapSize, 12.f, -10, 0.2f, 0.8f, 6.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 1.1f, y);
		});
	vector.push_back(layer);
	*/
	return vector;
}

Map::MapSurfaceGenerator DemoIceDBiome::getMapSurfaceGenerator()
{
	return [](Noise & noise, float x, float y)
	{
		return noise.fBm(x, y, 3, 3.f, 0.3f);
	};
}

Map::TileColorGenerator DemoIceDBiome::getTileColorGenerator()
{
	return [this](Noise & noise, float x, float y, float z)
	{
		//float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		float transition = (noise.noise(x / 10.f, std::cos(y / 20.f), z * 10.f) + 1.f) / 2.f;
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		DemoIceDBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		DemoIceDBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		DemoIceDBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		DemoIceDBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		DemoIceDBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		DemoIceDBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		DemoIceDBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		DemoIceDBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		DemoIceDBiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			DemoIceDBiome::getWind()
{
	return (m_wind);
}

void			DemoIceDBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			DemoIceDBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		DemoIceDBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		DemoIceDBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		DemoIceDBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			DemoIceDBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			DemoIceDBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			DemoIceDBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		DemoIceDBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		DemoIceDBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		DemoIceDBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		DemoIceDBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		DemoIceDBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		DemoIceDBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		DemoIceDBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		DemoIceDBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		DemoIceDBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		DemoIceDBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	DemoIceDBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	DemoIceDBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		DemoIceDBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		DemoIceDBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			DemoIceDBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			DemoIceDBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			DemoIceDBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			DemoIceDBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			DemoIceDBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	DemoIceDBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DemoIceDBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		DemoIceDBiome::getTreePositionX()
{
	return (randomInt(1u, m_mapSize.x - 1u));
}

sf::Vector2f	DemoIceDBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		DemoIceDBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		DemoIceDBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				DemoIceDBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			DemoIceDBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	DemoIceDBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		DemoIceDBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			DemoIceDBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			DemoIceDBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	DemoIceDBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		DemoIceDBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		DemoIceDBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

bool			DemoIceDBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	DemoIceDBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		DemoIceDBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		DemoIceDBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			DemoIceDBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	DemoIceDBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		DemoIceDBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		DemoIceDBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		DemoIceDBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			DemoIceDBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	DemoIceDBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		DemoIceDBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		DemoIceDBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			DemoIceDBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	DemoIceDBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		DemoIceDBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		DemoIceDBiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			DemoIceDBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	DemoIceDBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DemoIceDBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		DemoIceDBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			DemoIceDBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DemoIceDBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			DemoIceDBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		DemoIceDBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		DemoIceDBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		DemoIceDBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			DemoIceDBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}


float			DemoIceDBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				DemoIceDBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			DemoIceDBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			DemoIceDBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				DemoIceDBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	DemoIceDBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		DemoIceDBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		DemoIceDBiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
