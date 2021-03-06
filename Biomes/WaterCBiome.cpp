#include "WaterCBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

WaterCBiome::WaterCBiome() :
	m_name("Water B"),
	m_id(Level::WaterC),
	m_seed("Vince"),
	m_mapSize(sf::Vector2u(850u, 128u)),
	m_mapSeed(42u),
	m_octoStartPosition(49.f * 16.f, 240.f * 16.f),
	m_transitionDuration(2.f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(255, 208, 71),
	m_tileEndColor(243, 146, 0),
	m_waterLevel(400.f),
	m_waterColor(37, 47, 203, 100),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(90.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(140,135,234),
	m_skyNightColor(26,15,213),
	m_nightLightColor(0,204,0, 100),
	m_dayLightColor(sf::Color::Transparent),
	m_SunsetLightColor(0,204,0, 120),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 15u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(5u, 7u),
	m_treeCount(5u, 10u),
	m_mushroomCount(390u, 400u),
	m_crystalCount(20u, 30u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 2u),
	m_rainbowCount(2u, 2u),
	m_cloudCount(30u, 40u),
	m_groundRockCount(200u, 400u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(false),
	m_canCreateRock(true),
	m_canCreateTree(true),
	m_canCreateLeaf(true),
	m_treeIsMoving(true),
	m_canCreateMushroom(true),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(false),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_canCreateGrass(false),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Water),

	m_rockSize(sf::Vector2f(10.f, 100.f), sf::Vector2f(20.f, 200.f)),
	m_rockPartCount(4.f, 8.f),
	m_rockColor(230, 230, 230),

	m_grassSizeY(90.f, 91.f),
	m_grassSizeX(50.f, 70.f),
	m_grassColor(159, 24, 24, 130),
	m_grassCount(m_mapSize.x / 2),
	m_grassIndex(0u),

	m_treeDepth(6u, 7u),
	m_treeSize(sf::Vector2f(5.f, 160.f), sf::Vector2f(20.f, 161.f)),
	m_treeLifeTime(sf::seconds(20.f), sf::seconds(50.f)),
	m_treeColor(23, 36, 40),
	m_treeAngle(-180.f, 180.f),
	m_treeBeatMouvement(0.01f),
	m_leafSize(sf::Vector2f(20.f, 20.f), sf::Vector2f(150.f, 150.f)),
	m_leafColor(220, 42, 40),

	m_mushroomSize(sf::Vector2f(10.f, 20.f), sf::Vector2f(20.f, 50.f)),
	m_mushroomColor(255, 0, 0, 150.f),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(80.f, 250.f), sf::Vector2f(100.f, 450.f)),
	m_crystalPartCount(3u, 6u),
	m_crystalColor(103, 157, 208, 50),
	m_shineEffectSize(sf::Vector2f(200.f, 200.f), sf::Vector2f(300.f, 300.f)),
	m_shineEffectColor(153, 207, 255, 130),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudPartCount(3u, 4u),
	m_cloudMaxY(-1000.f),
	m_cloudMinY(-4000.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 255),

	m_moonSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_moonColor(255, 255, 255),
	m_moonLifeTime(sf::seconds(15.f), sf::seconds(30.f)),

	m_rainbowThickness(60.f, 100.f),
	m_rainbowPartSize(30.f, 100.f),
	m_rainbowLoopCount(1u, 4u),
	m_rainbowLifeTime(sf::seconds(5.f), sf::seconds(10.f)),
	m_rainbowIntervalTime(sf::seconds(5.f), sf::seconds(10.f))
{
	m_generator.setSeed(m_seed);
#ifndef NDEBUG
	m_mapSeed = 42u;
#else
	m_mapSeed = 42u;//m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	// Define game objects
	m_instances[20] = MAP_WATER_C_TRAIL_OMP;

	m_gameObjects[295] = GameObjectType::CheckPoint;
	m_gameObjects[520] = GameObjectType::BeachGuyNpc;
	m_gameObjects[550] = GameObjectType::Pedestal;
	m_gameObjects[554] = GameObjectType::SlowFallNanoRobot;

	m_instances[640] = MAP_WATER_C_SLOWFALL_TRAIL_OMP;

	m_interestPointPosX = 150;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::WaterD)
		m_octoStartPosition = sf::Vector2f(756.f * 16.f, -4760.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(492.f * 16.f, -3130.f);

	std::size_t columnCount = 20u;

	for (std::size_t i = 0u; i < columnCount; i++)
	{
		std::size_t index;
		
		if (randomBool(0.7f))
			index = randomInt(0u, 290u);
		else
			index = randomInt(450u, 530u);

		m_gameObjects[index] = GameObjectType::ColumnNpc;;
	}

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::WaterB);
	m_destinations.push_back(Level::WaterD);
}

void			WaterCBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		WaterCBiome::getName()const
{
	return (m_name);
}

Level			WaterCBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	WaterCBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		WaterCBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	WaterCBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	WaterCBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			WaterCBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				WaterCBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	WaterCBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	WaterCBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	WaterCBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	WaterCBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	WaterCBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & WaterCBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> WaterCBiome::getLayers()
{
	sf::Vector2u mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer;
	layer = new GenerativeLayer(randomColor(m_tileEndColor), sf::Vector2f(0.5f, 0.3f), sf::Vector2u(mapSize.x, mapSize.y * 2.5f), 12.f, 350, 0.5f, 0.3f, 10.f, 400.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 5.f, y);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(randomColor(m_tileStartColor), sf::Vector2f(0.5f, 0.3f), sf::Vector2u(mapSize.x, mapSize.y * 4.5f), 12.f, 300, 0.1f, 0.3f, 10.f, 1000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 30.f, y);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(randomColor(m_tileStartColor), sf::Vector2f(0.6f, 0.2f), sf::Vector2u(mapSize.x, mapSize.y * 4.5f), 12.f, 100, 0.2f, 0.3f, 7.f, 2000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 35.f, y + 100);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator WaterCBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		m_pointX = {0.f      , 303.f    , 307.f    , 308.f, 442.f, 443.f   , 470.f   , 570.f   , 589.f  , 592.f, 612.f, 613.f  , 640.f, 710.f,  790.f, 850.f};
		m_pointY = {n - 5.25f, n - 5.25f, n - 5.f, 4.6f , 4.6f , n - 6.2f, n - 6.2f, n - 6.5f, -4.618f, 4.6f , 4.6f , -4.415f, -5.1f, -5.2f, - 5.1f, n - 5.25f};
		for (std::size_t i = 0u; i < m_pointX.size(); i++)
			m_pointX[i] /= floatMapSize;

		for (std::size_t i = 0u; i < m_pointX.size() - 1u; i++)
		{
			if (x >= m_pointX[i] && x < m_pointX[i + 1])
			{
				float coef = (x - m_pointX[i]) / (m_pointX[i + 1] - m_pointX[i]);
				return octo::cosinusInterpolation(m_pointY[i], m_pointY[i + 1], coef);
			}
		}
		return n;
	};
}

Map::TileColorGenerator WaterCBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(205, 158, 21);
	sf::Color secondColorEnd(193, 96, 0);
	sf::Color thirdColorStart(53, 107, 208);
	sf::Color thirdColorEnd(26, 15, 213);
	float start1 = -23000.f / static_cast<float>(m_mapSize.y);
	float start2 = -18000.f / static_cast<float>(m_mapSize.y);
	float middle1 = -14000.f / static_cast<float>(m_mapSize.y);
	float middle2 = 3000.f / static_cast<float>(m_mapSize.y);
	float end1 = 4000.f / static_cast<float>(m_mapSize.y);
	float end2 = 5000.f / static_cast<float>(m_mapSize.y);
	return [this, secondColorStart, secondColorEnd, thirdColorStart, thirdColorEnd, start1, start2, middle1, middle2, end1, end2](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		if (y <= start1)
			return octo::linearInterpolation(secondColorStart, secondColorEnd, transition);
		else if (y > start1 && y <= start2)
		{
			float ratio = (y - (start1)) / (start2 - start1);
			return octo::linearInterpolation(octo::linearInterpolation(secondColorStart, m_tileStartColor, ratio), secondColorEnd, transition);
		}
		else if (y > start2 && y <= middle1)
		{
			float ratio = (y - (start2)) / (middle1 - start2);
			return octo::linearInterpolation(m_tileStartColor, octo::linearInterpolation(secondColorEnd, m_tileEndColor, ratio), transition);
		}
		else if (y > middle1 && y <= middle2)
		{
			return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
		}
		else if (y >= middle2 && y < end1)
		{
			float ratio = (y - (middle2)) / (end1 - middle2);
			return octo::linearInterpolation(octo::linearInterpolation(m_tileStartColor, thirdColorStart, ratio), m_tileEndColor, transition);
		}
		else if (y >= end1 && y < end2)
		{
			float ratio = (y - (end1)) / (end2 - end1);
			return octo::linearInterpolation(thirdColorStart, octo::linearInterpolation(m_tileEndColor, thirdColorEnd, ratio), transition);
		}
		return octo::linearInterpolation(thirdColorStart, thirdColorEnd, transition);
	};
}

sf::Color		WaterCBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		WaterCBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		WaterCBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		WaterCBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		WaterCBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		WaterCBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		WaterCBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		WaterCBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	WaterCBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		WaterCBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			WaterCBiome::getWind()
{
	return (m_wind);
}

void			WaterCBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			WaterCBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		WaterCBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		WaterCBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		WaterCBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			WaterCBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			WaterCBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			WaterCBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		WaterCBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		WaterCBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		WaterCBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		WaterCBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		WaterCBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		WaterCBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		WaterCBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		WaterCBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		WaterCBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		WaterCBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	WaterCBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	WaterCBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		WaterCBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		WaterCBiome::getTreeColor()
{
	return (randomColorLeaf(m_treeColor));
}

float			WaterCBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			WaterCBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			WaterCBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			WaterCBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			WaterCBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	WaterCBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterCBiome::getLeafColor()
{
	return (randomColorLeaf(m_leafColor));
}

std::size_t		WaterCBiome::getTreePositionX()
{
	return randomInt(1u, m_mapSize.x - 1u);
}

sf::Vector2f	WaterCBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		WaterCBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		WaterCBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				WaterCBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			WaterCBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	WaterCBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		WaterCBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			WaterCBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			WaterCBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	WaterCBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		WaterCBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		WaterCBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	WaterCBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	WaterCBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	WaterCBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	WaterCBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	WaterCBiome::getGrassPosX()
{
	m_grassIndex += randomInt(2u, 4u);
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			WaterCBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	WaterCBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		WaterCBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		WaterCBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			WaterCBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	WaterCBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		WaterCBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	WaterCBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	WaterCBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	WaterCBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		WaterCBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		WaterCBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			WaterCBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	WaterCBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		WaterCBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		WaterCBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			WaterCBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	WaterCBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		WaterCBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		WaterCBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			WaterCBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	WaterCBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterCBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		WaterCBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			WaterCBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			WaterCBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			WaterCBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		WaterCBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		WaterCBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		WaterCBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			WaterCBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	WaterCBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	WaterCBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	WaterCBiome::getType() const
{
	return m_type;
}


float			WaterCBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				WaterCBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			WaterCBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			WaterCBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				WaterCBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	WaterCBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		WaterCBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		WaterCBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);
}

sf::Color		WaterCBiome::randomColorLeaf(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);
}
