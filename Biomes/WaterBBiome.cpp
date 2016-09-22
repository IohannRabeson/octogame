#include "WaterBBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

WaterBBiome::WaterBBiome() :
	m_name("Water B"),
	m_id(Level::WaterB),
	m_seed("Vince"),
	m_mapSize(sf::Vector2u(700u, 180u)),
	m_mapSeed(42u),
	m_octoStartPosition(604.f * 16.f, -180.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(250, 229, 205),
	m_tileEndColor(244, 201, 154),
	m_waterLevel(1000.f),
	m_waterColor(3, 57, 108, 60),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(90.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(255, 0, 0),
	m_skyNightColor(255, 0, 0),
	m_nightLightColor(255, 90, 61, 130),
	m_dayLightColor(sf::Color::Transparent),
	m_SunsetLightColor(255, 147, 46, 130),
	m_wind(0.f),
	m_rainDropPerSecond(10u, 15u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(40u, 45u),
	m_treeCount(30u, 30u),
	m_mushroomCount(390u, 400u),
	m_crystalCount(20u, 30u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 2u),
	m_rainbowCount(2u, 2u),
	m_cloudCount(480u, 490u),
	m_groundRockCount(200u, 400u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(false),
	m_canCreateRock(true),
	m_canCreateTree(false),
	m_canCreateLeaf(true),
	m_treeIsMoving(true),
	m_canCreateMushroom(true),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(true),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(true),
	m_canCreateGrass(false),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Water),

	m_rockSize(sf::Vector2f(10.f, 100.f), sf::Vector2f(20.f, 200.f)),
	m_rockPartCount(4.f, 8.f),
	m_rockColor(159, 24, 24),

	m_grassSizeY(60.f, 70.f),
	m_grassSizeX(25.f, 40.f),
	m_grassColor(244, 201, 154, 150),
	m_grassCount(m_mapSize.x / 2),
	m_grassIndex(0u),

	m_treeDepth(3u, 4u),
	m_treeSize(sf::Vector2f(10.f, 50.f), sf::Vector2f(20.f, 100.f)),
	m_treeLifeTime(sf::seconds(20.f), sf::seconds(50.f)),
	m_treeColor(103, 157, 208, 50),
	m_treeAngle(-180.f, 180.f),
	m_treeBeatMouvement(0.4f),
	m_leafSize(sf::Vector2f(25.f, 25.f), sf::Vector2f(70.f, 70.f)),
	m_leafColor(103, 157, 208, 75),

	m_mushroomSize(sf::Vector2f(10.f, 20.f), sf::Vector2f(20.f, 50.f)),
	m_mushroomColor(255, 0, 0, 150.f),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(20.f, 150.f), sf::Vector2f(35.f, 200.f)),
	m_crystalPartCount(5u, 8u),
	m_crystalColor(103, 157, 208, 50),
	m_shineEffectSize(sf::Vector2f(200.f, 200.f), sf::Vector2f(300.f, 300.f)),
	m_shineEffectColor(153, 207, 255, 130),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(500.f, 500.f), sf::Vector2f(800.f, 800.f)),
	m_cloudPartCount(1u, 1u),
	m_cloudMaxY(1500.f),
	m_cloudMinY(-13000.f),
	m_cloudSpeed(sf::Vector2f(0.f, -140.f), sf::Vector2f(0.f, -250.f)),
	m_cloudLifeTime(sf::seconds(600), sf::seconds(900)),
	m_cloudColor(103, 157, 208, 120),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(159, 24, 24),

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
	m_instances[100] = MAP_WATER_B_TRAIL_OMP;
	m_gameObjects[680] = GameObjectType::CedricEndNpc;
	m_gameObjects[30] = GameObjectType::Concert;
	m_gameObjects[80] = GameObjectType::PortalWater;

	m_interestPointPosX = 500;

	Progress & progress = Progress::getInstance();
	std::list<GameObjectType> const & npcList = progress.getNpcMet();

	for (auto npc = npcList.begin(); npc != npcList.end(); npc++)
	{
		std::size_t index;
		
		if (randomBool(0.5f))
			index = randomInt(1u, 90u);
		else
			index = randomInt(610u, 699u);

		if (*npc != GameObjectType::CedricStartNpc)
			m_gameObjects[index] = *npc;
	}

	if (progress.getLastDestination() == Level::WaterC)
		m_octoStartPosition = sf::Vector2f(784.f * 16.f, -13050.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(363.f * 16.f, -15130.f);

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::WaterA);
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::WaterC);
}

void			WaterBBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		WaterBBiome::getName()const
{
	return (m_name);
}

Level			WaterBBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	WaterBBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		WaterBBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	WaterBBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	WaterBBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			WaterBBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				WaterBBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	WaterBBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	WaterBBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	WaterBBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	WaterBBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	WaterBBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & WaterBBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> WaterBBiome::getLayers()
{
	sf::Vector2u mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getCrystalColor(), sf::Vector2f(0.4f, 0.5f), mapSize, 10.f, 50, 0.1f, 0.4f, 11.f, 40.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 20.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getCrystalColor(), sf::Vector2f(0.5f, 0.4f), mapSize, 10.f, 40, 0.1f, 0.4f, 11.f, 40.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 10.f + 100.f, y + 100.f, 2, 2.f);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator WaterBBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 1.0f);
		float m = n / 10.f;
		std::vector<float> pointX = {0.f     , 80.f    , 100.f, 101.f, 150.f, 180.f, 520.f, 550.f, 599.f, 600.f, 620.f   , 700.f   };
		std::vector<float> pointY = {m - 10.f, m - 10.f, -10.f, -1.0f, -1.0f, n    , n    , -1.0f, -1.0f, -10.f, m - 10.f, m - 10.f};
		for (std::size_t i = 0u; i < pointX.size(); i++)
			pointX[i] /= floatMapSize;

		for (std::size_t i = 0u; i < pointX.size() - 1u; i++)
		{
			if (x >= pointX[i] && x < pointX[i + 1])
			{
				float coef = (x - pointX[i]) / (pointX[i + 1] - pointX[i]);
				return octo::linearInterpolation(pointY[i], pointY[i + 1], coef);
			}
		}
		return n;
	};
}

Map::TileColorGenerator WaterBBiome::getTileColorGenerator()
{
	sf::Color secondColorEnd = getRockColor();
	sf::Color secondColorStart = getRockColor();
	sf::Color thirdColorStart(53, 107, 208);
	sf::Color thirdColorEnd(103, 157, 208);
	float start1 = -200000.f / static_cast<float>(m_mapSize.y);
	float start2 = -130000.f / static_cast<float>(m_mapSize.y);
	float middle1 = -105000.f / static_cast<float>(m_mapSize.y);
	float middle2 = -100000.f / static_cast<float>(m_mapSize.y);
	float end1 = -10000.f / static_cast<float>(m_mapSize.y);
	float end2 = 17000.f / static_cast<float>(m_mapSize.y);
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

sf::Color		WaterBBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		WaterBBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		WaterBBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		WaterBBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		WaterBBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		WaterBBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		WaterBBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		WaterBBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	WaterBBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		WaterBBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			WaterBBiome::getWind()
{
	return (m_wind);
}

void			WaterBBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			WaterBBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		WaterBBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		WaterBBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		WaterBBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			WaterBBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			WaterBBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			WaterBBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		WaterBBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		WaterBBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		WaterBBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		WaterBBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		WaterBBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		WaterBBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		WaterBBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		WaterBBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		WaterBBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		WaterBBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	WaterBBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	WaterBBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		WaterBBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		WaterBBiome::getTreeColor()
{
	return (randomColorLeaf(m_treeColor));
}

float			WaterBBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			WaterBBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			WaterBBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			WaterBBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			WaterBBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	WaterBBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterBBiome::getLeafColor()
{
	return (randomColorLeaf(m_leafColor));
}

std::size_t		WaterBBiome::getTreePositionX()
{
	return randomInt(1u, m_mapSize.x - 1u);
}

sf::Vector2f	WaterBBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		WaterBBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		WaterBBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				WaterBBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			WaterBBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	WaterBBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		WaterBBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			WaterBBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			WaterBBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	WaterBBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		WaterBBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		WaterBBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	WaterBBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	WaterBBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	WaterBBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	WaterBBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	WaterBBiome::getGrassPosX()
{
	m_grassIndex += randomInt(2u, 4u);
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			WaterBBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	WaterBBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		WaterBBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		WaterBBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			WaterBBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	WaterBBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		WaterBBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	WaterBBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	WaterBBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	WaterBBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		WaterBBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		WaterBBiome::getCloudColor()
{
	return (randomColorLeaf(m_cloudColor));
}

bool			WaterBBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	WaterBBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		WaterBBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		WaterBBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			WaterBBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	WaterBBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		WaterBBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		WaterBBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			WaterBBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	WaterBBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterBBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		WaterBBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			WaterBBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			WaterBBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			WaterBBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		WaterBBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		WaterBBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		WaterBBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			WaterBBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	WaterBBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	WaterBBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	WaterBBiome::getType() const
{
	return m_type;
}


float			WaterBBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				WaterBBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			WaterBBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			WaterBBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				WaterBBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	WaterBBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		WaterBBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		WaterBBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}

sf::Color		WaterBBiome::randomColorLeaf(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
