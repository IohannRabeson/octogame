#include "JungleDBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

JungleDBiome::JungleDBiome() :
	m_name("Jungle D"),
	m_id(Level::JungleD),
	m_seed("Jungle D"),
	m_mapSize(sf::Vector2u(900u, 200u)),
	m_mapSeed(42u),
	m_octoStartPosition(33.f * 16.f, -650.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(0, 76, 54),
	m_tileEndColor(40, 164, 144),
	m_waterLevel(1.f),
	m_waterColor(196, 235, 1, 150),
	m_secondWaterColor(0, 189, 168, 150),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(80.f)),
	m_startDayDuration(sf::seconds(15.f)),
	m_skyDayColor(251, 253, 26),
	m_skyNightColor(195, 197, 18),
	m_nightLightColor(0, 0, 0, 130),
	m_SunsetLightColor(255, 182, 0, 100),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(60u, 80u),
	m_treeCount(40u, 50u),
	m_mushroomCount(39u, 40u),
	m_crystalCount(10u, 15u),
	m_starCount(500u, 800u),
	m_sunCount(2u, 3u),
	m_moonCount(3u, 4u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(20u, 40u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(true),
	m_canCreateThunder(true),
	m_canCreateSnow(false),
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
	m_canCreateGrass(true),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Jungle),

	m_rockSize(sf::Vector2f(50.f, 100.f), sf::Vector2f(150.f, 200.f)),
	m_rockPartCount(4.f, 10.f),
	m_rockColor(56, 50, 72),

	m_grassSizeY(20.f, 70.f),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),

	m_treeDepth(4u, 5u),
	m_treeSize(sf::Vector2f(30.f, 300.f), sf::Vector2f(200.f, 300.f)),
	m_treeLifeTime(sf::seconds(90), sf::seconds(180)),
	m_treeColor(40, 37, 44),
	m_treeAngle(15.f, 35.f),
	m_treeBeatMouvement(0.05f),
	m_leafSize(sf::Vector2f(20.f, 20.f), sf::Vector2f(250.f, 250.f)),
	m_leafColor(0, 90, 67),

	m_mushroomSize(sf::Vector2f(20.f, 100.f), sf::Vector2f(200.f, 300.f)),
	m_mushroomColor(255, 182, 0),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(40.f, 100.f), sf::Vector2f(80.f, 200.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(134, 160, 191, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(300.f, 200.f), sf::Vector2f(600.f, 400.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudMaxY(-1000.f),
	m_cloudMinY(-4000.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(50.f, 50.f), sf::Vector2f(100.f, 100.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 255),

	m_moonSize(sf::Vector2f(50.f, 30.f), sf::Vector2f(100.f, 100.f)),
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
	m_mapSeed = 42u;//m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	// Define game objects
	m_instances[230] = MAP_JUNGLE_D_TRAIL_OMP;
	m_instances[40] = MAP_JUNGLE_D_GUI_OMP;
	m_gameObjects[30] = GameObjectType::PortalJungle;
	m_gameObjects[40] = GameObjectType::BirdRedNpc;
	m_gameObjects[175] = GameObjectType::OverCoolNpc;
	m_gameObjects[110] = GameObjectType::BirdRedNpc;
	m_instances[613] = MAP_JUNGLE_D_SMALL_VILLAGE_OMP;
	m_gameObjects[740] = GameObjectType::Pedestal;
	m_gameObjects[744] = GameObjectType::WaterNanoRobot;
	m_instances[815] = MAP_JUNGLE_D_PORTAL_OMP;
	m_gameObjects[830] = GameObjectType::CanouilleNpc;
	for (std::size_t i = 0; i < 200; i += m_generator.randomInt(20u, 40u))
	{
		if (m_generator.randomBool(0.5))
			m_gameObjects[i] = GameObjectType::ForestSpirit1Npc;
		else
			m_gameObjects[i] = GameObjectType::ForestSpirit2Npc;
	}
	for (std::size_t i = 605; i < 755; i += m_generator.randomInt(20u, 30u))
	{
		if (m_generator.randomBool(0.5))
			m_gameObjects[i] = GameObjectType::ForestSpirit1Npc;
		else
			m_gameObjects[i] = GameObjectType::ForestSpirit2Npc;
	}
	for (std::size_t i = 585; i < 598; i += 2)
		m_gameObjects[i] = GameObjectType::BirdRedNpc;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::JungleD)
		m_octoStartPosition = sf::Vector2f(835.f * 16.f, -71.f * 16.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(346.f * 16.f, -280.f * 16.f);

	m_interestPointPosX = 400;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::WaterA);
	m_destinations.push_back(Level::JungleC);
}

void			JungleDBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		JungleDBiome::getName()const
{
	return (m_name);
}

Level			JungleDBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	JungleDBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		JungleDBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	JungleDBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	JungleDBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			JungleDBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				JungleDBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	JungleDBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	JungleDBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	JungleDBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	JungleDBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	JungleDBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

bool		JungleDBiome::isDeadlyWater()
{
	return true;
}

std::map<std::size_t, std::string> const & JungleDBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> JungleDBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -20, 0.1f, 1.f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 1.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -10, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
		return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	//layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.6f, 0.2f), mapSize, 12.f, -10, 0.2f, 0.8f, 6.f);
	//layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
	//	{
	//		return noise.noise(x * 1.1f, y);
	//	});
	//vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator JungleDBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f) - 0.3f;
		float m = n / 3.f;
		std::vector<float> pointX = {0.f      , 40.f  , 150.f  , 200.f    , 225.f   , 350.f   , 563.f, 570.f    , 610.f    , 615.f, 684.f , 734.f  , 750.f  , 800.f    , 900.f};
		std::vector<float> pointY = {m - 1.15f, -1.17f, -1.17f , m - 1.15f, n - 0.6f, n + 0.5f, 0.5f , m - 1.71f, m - 1.71f, -1.2f, -1.28f, m - 1.f, m - 1.f, m - 0.65f, m - 0.65f};
		for (std::size_t i = 0u; i < pointX.size(); i++)
			pointX[i] /= floatMapSize;

		for (std::size_t i = 0u; i < pointX.size() - 1u; i++)
		{
			if (x >= pointX[i] && x < pointX[i + 1])
			{
				float coef = (x - pointX[i]) / (pointX[i + 1] - pointX[i]);
				return octo::cosinusInterpolation(pointY[i], pointY[i + 1], coef);
			}
		}
		return n;
	};
}

Map::TileColorGenerator JungleDBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(76, 70, 102);
	sf::Color secondColorEnd(56, 50, 72);
	float startTransition = 1000.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 5000.f / static_cast<float>(m_mapSize.y);
	float endTransition = 8000.f / static_cast<float>(m_mapSize.y);
	return [this, secondColorStart, secondColorEnd, startTransition, endTransition, middleTransition](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		if (y > startTransition && y <= middleTransition)
		{
			float ratio = (y - (startTransition)) / (middleTransition - startTransition);
			return octo::linearInterpolation(octo::linearInterpolation(m_tileStartColor, secondColorStart, ratio), m_tileEndColor, transition);
		}
		else if (y > middleTransition && y <= endTransition)
		{
			float ratio = (y - (middleTransition)) / (endTransition - middleTransition);
			return octo::linearInterpolation(secondColorStart, octo::linearInterpolation(m_tileEndColor, secondColorEnd, ratio), transition);
		}
		if (y > endTransition)
			return octo::linearInterpolation(secondColorStart, secondColorEnd, transition);
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		JungleDBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		JungleDBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		JungleDBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		JungleDBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		JungleDBiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		JungleDBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		JungleDBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		JungleDBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		JungleDBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			JungleDBiome::getWind()
{
	return (m_wind);
}

void			JungleDBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			JungleDBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		JungleDBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		JungleDBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		JungleDBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			JungleDBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			JungleDBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			JungleDBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		JungleDBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		JungleDBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		JungleDBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		JungleDBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		JungleDBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		JungleDBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		JungleDBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		JungleDBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		JungleDBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		JungleDBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	JungleDBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	JungleDBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		JungleDBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		JungleDBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			JungleDBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			JungleDBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			JungleDBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			JungleDBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			JungleDBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	JungleDBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleDBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		JungleDBiome::getTreePositionX()
{
	return randomInt(1u, m_mapSize.x - 1u);
}

sf::Vector2f	JungleDBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		JungleDBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		JungleDBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				JungleDBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			JungleDBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	JungleDBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		JungleDBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			JungleDBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			JungleDBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	JungleDBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		JungleDBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		JungleDBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	JungleDBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	JungleDBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	JungleDBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	JungleDBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	JungleDBiome::getGrassPosX()
{
	return randomInt(0u, m_mapSize.x);
}

bool			JungleDBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	JungleDBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		JungleDBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		JungleDBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			JungleDBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	JungleDBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		JungleDBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	JungleDBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	JungleDBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	JungleDBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		JungleDBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		JungleDBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			JungleDBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	JungleDBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		JungleDBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		JungleDBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			JungleDBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	JungleDBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		JungleDBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		JungleDBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			JungleDBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	JungleDBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleDBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		JungleDBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			JungleDBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			JungleDBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			JungleDBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		JungleDBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		JungleDBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		JungleDBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			JungleDBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	JungleDBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	JungleDBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	JungleDBiome::getType() const
{
	return m_type;
}


float			JungleDBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				JungleDBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			JungleDBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			JungleDBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				JungleDBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	JungleDBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		JungleDBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		JungleDBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
