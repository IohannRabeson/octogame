#include "RewardsBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <ctime>
#include <limits>
#include <iostream>

std::size_t RewardsBiome::m_seedId = 110u;

RewardsBiome::RewardsBiome() :
	m_generator("random"),
	m_randomSurfaceNumber(0u),
	m_name("Rewards"),
	m_id(Level::Rewards),
	m_seed("Rewards"),
	m_mapSize(sf::Vector2u(m_generator.randomInt(1450u, 2850u), m_generator.randomPiecewise(1500))),
	m_mapSeed(m_generator.randomInt(2u, 100000u)),
	m_octoStartPosition(23.f * 16.f, -300.f),
	m_transitionDuration(0.7f),
	m_interestPointPosX(45u),
	m_tileStartColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_tileEndColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_waterLevel(m_generator.randomInt(400u, 3000u)),
	m_waterColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(40, 150)),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(m_generator.randomFloat(20.f, 150.f))),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_skyNightColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_nightLightColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), 130),
	m_dayLightColor(sf::Color::Transparent),
	m_SunsetLightColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), 130),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(m_generator.randomFloat(10.f, 700.f), m_generator.randomFloat(700.f, 3000.f)),

	m_rockCount(m_generator.randomInt(1, 30), m_generator.randomInt(30, 100)),
	m_treeCount(m_generator.randomInt(1, 20), m_generator.randomInt(20, 50)),
	m_mushroomCount(m_generator.randomInt(1, 100), m_generator.randomInt(100, 250)),
	m_crystalCount(m_generator.randomInt(1, 30), m_generator.randomInt(30, 50)),
	m_starCount(300u, 800u),
	m_sunCount(m_generator.randomInt(1, 4), m_generator.randomInt(4, 8)),
	m_moonCount(m_generator.randomInt(1, 2), m_generator.randomInt(2, 6)),
	m_rainbowCount(m_generator.randomInt(1, 3), m_generator.randomInt(3, 10)),
	m_cloudCount(m_generator.randomInt(1, 50), m_generator.randomInt(50, 100)),
	m_groundRockCount(m_generator.randomInt(100, 200), m_generator.randomInt(200, 700)),

	m_canCreateRain(m_generator.randomBool(0.4f)),
	m_canCreateThunder(m_generator.randomBool(0.4f)),
	m_canCreateSnow(m_generator.randomBool(0.4f)),
	m_canCreateRock(m_generator.randomBool(0.4f)),
	m_canCreateTree(m_generator.randomBool(0.4f)),
	m_canCreateLeaf(m_generator.randomBool(0.5f)),
	m_treeIsMoving(m_generator.randomBool(0.8f)),
	m_canCreateMushroom(m_generator.randomBool(0.4f)),
	m_canCreateCrystal(m_generator.randomBool(0.4f)),
	m_canCreateShineEffect(m_generator.randomBool(0.8f)),
	m_canCreateCloud(m_generator.randomBool(0.6f)),
	m_canCreateStar(m_generator.randomBool(0.9f)),
	m_canCreateSun(m_generator.randomBool(0.8f)),
	m_canCreateMoon(m_generator.randomBool(0.8f)),
	m_canCreateRainbow(m_generator.randomBool(0.4f)),
	m_canCreateGrass(m_generator.randomBool(0.7f)),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Random),

	m_rockSize(sf::Vector2f(m_generator.randomFloat(2.f, 50.f), m_generator.randomFloat(10.f, 60.f)), sf::Vector2f(m_generator.randomFloat(50.f, 100.f), m_generator.randomFloat(200.f, 600.f))),
	m_rockPartCount(m_generator.randomInt(2.f, 4.f), m_generator.randomFloat(4.f, 20.f)),
	m_rockColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),

	m_grassSizeY(m_generator.randomFloat(10.f, 60.f), m_generator.randomFloat(60.f, 200.f)),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),
	m_grassIndex(0u),

	//TODO: Value to improve
	m_treeDepth(m_generator.randomInt(4u, 5u), m_generator.randomInt(6u, 7u)),
	m_treeSize(sf::Vector2f(m_generator.randomFloat(2.f, 20.f), m_generator.randomFloat(10.f, 100.f)), sf::Vector2f(m_generator.randomFloat(20.f, 100.f), m_generator.randomFloat(100.f, 600.f))),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_treeAngle(m_generator.randomFloat(-180, 30), m_generator.randomFloat(30, 180)),
	m_treeBeatMouvement(0.15f),
	m_leafSize(sf::Vector2f(m_generator.randomFloat(2.f, 20.f), m_generator.randomFloat(10.f, 100.f)), sf::Vector2f(m_generator.randomFloat(20.f, 100.f), m_generator.randomFloat(400.f, 600.f))),
	m_leafColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),

	//TODO: Randomize all of that
	m_mushroomSize(sf::Vector2f(m_generator.randomFloat(2.f, 50.f), m_generator.randomFloat(10.f, 60.f)), sf::Vector2f(m_generator.randomFloat(50.f, 100.f), m_generator.randomFloat(200.f, 600.f))),
	m_mushroomColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_mushroomLifeTime(sf::seconds(10), sf::seconds(30)),

	m_crystalSize(sf::Vector2f(m_generator.randomFloat(2.f, 50.f), m_generator.randomFloat(10.f, 60.f)), sf::Vector2f(m_generator.randomFloat(50.f, 100.f), m_generator.randomFloat(200.f, 600.f))),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), 150),
	m_shineEffectSize(sf::Vector2f(m_generator.randomFloat(2.f, 50.f), m_generator.randomFloat(10.f, 60.f)), sf::Vector2f(m_generator.randomFloat(50.f, 100.f), m_generator.randomFloat(200.f, 600.f))),
	m_shineEffectColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), 100),
	m_shineEffectRotateAngle(m_generator.randomFloat(50.f, 100.f), m_generator.randomFloat(100.f, 360.f)),

	m_cloudSize(sf::Vector2f(200.f, 100.f), sf::Vector2f(400.f, 200.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudMaxY(-1000.f),
	m_cloudMinY(-4000.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), 200),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(m_generator.randomFloat(10.f, 100.f), m_generator.randomFloat(10.f, 100.f)), sf::Vector2f(m_generator.randomFloat(100.f, 350.f), m_generator.randomFloat(100.f, 350.f))),
	m_sunPartCount(m_generator.randomInt(1, 4), m_generator.randomInt(4, 7)),
	m_sunColor(m_generator.randomInt(250, 255), m_generator.randomInt(250, 255), m_generator.randomInt(250, 255)),

	m_moonSize(sf::Vector2f(m_generator.randomFloat(10.f, 100.f), m_generator.randomFloat(10.f, 100.f)), sf::Vector2f(m_generator.randomFloat(100.f, 350.f), m_generator.randomFloat(100.f, 350.f))),
	m_moonColor(m_generator.randomInt(250, 255), m_generator.randomInt(250, 255), m_generator.randomInt(250, 255)),
	m_moonLifeTime(sf::seconds(15.f), sf::seconds(30.f)),

	m_rainbowThickness(70.f, 120.f),
	m_rainbowPartSize(50.f, 200.f),
	m_rainbowLoopCount(1u, 5u),
	m_rainbowLifeTime(sf::seconds(6.f), sf::seconds(10.f)),
	m_rainbowIntervalTime(sf::seconds(1.f), sf::seconds(2.f))
{
	//m_generator.setSeed(m_seed);
#ifndef NDEBUG
	m_mapSeed = 42u;
#else
	m_mapSeed = m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif
	Progress & progress = Progress::getInstance();
	m_mapSize = sf::Vector2u(m_generator.randomInt(650u, 2250u), m_generator.randomPiecewise(progress.getNanoRobotCount() * 60u + 30u)),
	m_randomSurfaceNumber = m_generator.randomInt(1u, 4u);
	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	// TODO define map position and number of map
	std::size_t portalPos = m_generator.randomInt(100u, m_mapSize.x - 40u);
	if (progress.isMenu())
	{
		m_gameObjects[portalPos] = GameObjectType::PortalRandom;
		m_destinations.push_back(Level::Rewards);
	}
	else
	{
		m_gameObjects[portalPos] = GameObjectType::Portal;
		m_destinations.push_back(Level::Rewards);
	}

	m_interestPointPosX = portalPos;

	for (std::size_t i = 0u; i < m_mapSize.x - 1u; i += m_generator.randomInt(15u, 200u))
	{
		if (m_generator.randomBool(0.5))
			m_gameObjects[i] = GameObjectType::ForestSpirit1Npc;
		else
			m_gameObjects[i] = GameObjectType::ForestSpirit2Npc;
	}

	for (std::size_t i = 0u; i < m_mapSize.x - 1u; i += m_generator.randomInt(15u, 300u))
	{
		if (m_generator.randomBool(0.5))
			m_gameObjects[i] = GameObjectType::BirdBlueNpc;
		else
			m_gameObjects[i] = GameObjectType::BirdRedNpc;
	}

	std::list<GameObjectType> const & npcList = progress.getNpcMet();

	for (auto npc = npcList.begin(); npc != npcList.end(); npc++)
	{
		std::size_t index = randomInt(10u, m_mapSize.x - 10u);

		m_gameObjects[index] = *npc;
	}
}

void			RewardsBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		RewardsBiome::getName()const
{
	return (m_name);
}

Level			RewardsBiome::getId()const
{
	return (m_id);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	RewardsBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		RewardsBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	RewardsBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	RewardsBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			RewardsBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				RewardsBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	RewardsBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	RewardsBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	RewardsBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	RewardsBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	RewardsBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & RewardsBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> RewardsBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

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
	return vector;
}

Map::MapSurfaceGenerator RewardsBiome::getMapSurfaceGenerator()
{
	switch (m_randomSurfaceNumber)
	{
		case 1u:
			return [](Noise & noise, float x, float y)
			{
				return noise.fBm(x, y, 3, 3.f, 0.3f);
			};
			break;
		case 2u:
			return [](Noise & noise, float x, float y)
			{
				return noise.noise(x * 1.1f, y);
			};
			break;
		case 3u:
			return [](Noise & noise, float x, float y)
			{
				return noise.perlin(x, y, 3, 2.f);
			};
			break;
		case 4u:
			if (Progress::getInstance().getNanoRobotCount() >= 3)
			{
				return [](Noise & noise, float x, float y)
				{
					return noise.perlin(x * 10.f, y, 2, 2.f);
				};
			}
			else
			{
				return [](Noise & noise, float x, float y)
				{
					return noise.perlin(x, y, 3, 2.f);
				};
			}
			break;
		default:
			break;
	}

	return [](Noise & noise, float x, float y)
	{
		return noise.fBm(x, y, 3, 3.f, 0.3f);
	};
}

Map::TileColorGenerator RewardsBiome::getTileColorGenerator()
{
	return [this](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		RewardsBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		RewardsBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		RewardsBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		RewardsBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		RewardsBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		RewardsBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		RewardsBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		RewardsBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	RewardsBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		RewardsBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			RewardsBiome::getWind()
{
	return (m_wind);
}

void			RewardsBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			RewardsBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		RewardsBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		RewardsBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		RewardsBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			RewardsBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			RewardsBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			RewardsBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		RewardsBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		RewardsBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		RewardsBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		RewardsBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		RewardsBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		RewardsBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		RewardsBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		RewardsBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		RewardsBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		RewardsBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	RewardsBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	RewardsBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		RewardsBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		RewardsBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			RewardsBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			RewardsBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			RewardsBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			RewardsBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			RewardsBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	RewardsBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		RewardsBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		RewardsBiome::getTreePositionX()
{
	return randomInt(1, m_mapSize.x);
}

sf::Vector2f	RewardsBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		RewardsBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		RewardsBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				RewardsBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			RewardsBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	RewardsBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		RewardsBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			RewardsBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			RewardsBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	RewardsBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		RewardsBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		RewardsBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	RewardsBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	RewardsBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	RewardsBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	RewardsBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	RewardsBiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			RewardsBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	RewardsBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		RewardsBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		RewardsBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			RewardsBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	RewardsBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		RewardsBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	RewardsBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	RewardsBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	RewardsBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		RewardsBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		RewardsBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			RewardsBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	RewardsBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		RewardsBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		RewardsBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			RewardsBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	RewardsBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		RewardsBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		RewardsBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			RewardsBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	RewardsBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		RewardsBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		RewardsBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			RewardsBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			RewardsBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			RewardsBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		RewardsBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		RewardsBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		RewardsBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			RewardsBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	RewardsBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	RewardsBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	RewardsBiome::getType() const
{
	return m_type;
}


float			RewardsBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				RewardsBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			RewardsBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			RewardsBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				RewardsBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	RewardsBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		RewardsBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		RewardsBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
