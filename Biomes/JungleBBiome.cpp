#include "JungleBBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

JungleBBiome::JungleBBiome() :
	m_name("Jungle B"),
	m_id(Level::JungleB),
	m_seed("Jungle B"),
	m_mapSize(sf::Vector2u(550u, 200u)),
	m_mapSeed(42u),
	m_octoStartPosition(113.f * 16.f, 400.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(0, 76, 54),
	m_tileEndColor(0, 124, 104),
	m_waterLevel(7100.f),
	m_waterColor(196, 235, 1, 150),
	m_secondWaterColor(134, 160, 191, 150),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(80.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(252, 252, 160),
	m_skyNightColor(175, 177, 18),
	m_nightLightColor(0, 0, 0, 80),
	m_dayLightColor(sf::Color::Transparent),
	m_SunsetLightColor(255, 182, 0, 100),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(10u, 15u),
	m_treeCount(30u, 40u),
	m_mushroomCount(70u, 90u),
	m_crystalCount(10u, 15u),
	m_starCount(500u, 800u),
	m_sunCount(4u, 5u),
	m_moonCount(3u, 4u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(20u, 40u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(true),
	m_canCreateThunder(true),
	m_canCreateSnow(false),
	m_canCreateRock(true),
	m_canCreateTree(true),
	m_canCreateLeaf(false),
	m_treeIsMoving(false),
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

	m_rockSize(sf::Vector2f(30.f, 50.f), sf::Vector2f(40.f, 300.f)),
	m_rockPartCount(4.f, 10.f),
	m_rockColor(56, 50, 72),

	m_grassSizeY(60.f, 100.f),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),

	m_treeDepth(5u, 6u),
	m_treeSize(sf::Vector2f(30.f, 150.f), sf::Vector2f(120.f, 250.f)),
	m_treeLifeTime(sf::seconds(90), sf::seconds(180)),
	m_treeColor(40, 37, 44),
	m_treeAngle(15.f, 35.f),
	m_treeBeatMouvement(0.05f),
	m_leafSize(sf::Vector2f(20.f, 20.f), sf::Vector2f(250.f, 250.f)),
	m_leafColor(0, 90, 67),

	m_mushroomSize(sf::Vector2f(30.f, 150.f), sf::Vector2f(300.f, 900.f)),
	m_mushroomColor(255, 182, 0),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(10.f, 110.f), sf::Vector2f(20.f, 130.f)),
	m_crystalPartCount(6u, 10u),
	m_crystalColor(134, 160, 191, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(300.f, 200.f), sf::Vector2f(600.f, 400.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudMaxY(1000.f),
	m_cloudMinY(-2500.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(20.f, 20.f), sf::Vector2f(50.f, 50.f)),
	m_sunPartCount(4u, 8u),
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
	m_instances[30] = MAP_JUNGLE_B_TRAIL_OMP;
	m_gameObjects[90] = GameObjectType::AnthemJungle;
	m_gameObjects[110] = GameObjectType::PortalJungle;
	m_instances[339] = MAP_JUNGLE_B_FLUE_OMP;
	m_instances[387] = MAP_JUNGLE_B_FLUE_PART_OMP;
	m_instances[405] = MAP_JUNGLE_B_ELEVATOR_OMP;
	m_instances[35] = MAP_JUNGLE_B_CLIFF_OMP;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::JungleC)
		m_octoStartPosition = sf::Vector2f(447.f * 16.f, -450.f);
	else if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(100.f * 16.f, 4300.f);
	for (std::size_t i = 430u; i < m_mapSize.x - 10u; i += m_generator.randomInt(15u, 30u))
	{
		if (m_generator.randomBool(0.5))
			m_gameObjects[i] = GameObjectType::ForestSpirit1Npc;
		else
			m_gameObjects[i] = GameObjectType::ForestSpirit2Npc;
	}

	m_interestPointPosX = 80;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::JungleC);
	m_destinations.push_back(Level::JungleA);
}

void			JungleBBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		JungleBBiome::getName()const
{
	return (m_name);
}

Level			JungleBBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	JungleBBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		JungleBBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	JungleBBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	JungleBBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			JungleBBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				JungleBBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	JungleBBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	JungleBBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	JungleBBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	JungleBBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	JungleBBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

bool		JungleBBiome::isDeadlyWater()
{
	return true;
}

std::map<std::size_t, std::string> const & JungleBBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> JungleBBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getRockColor(), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, 10, 0.1f, 1.f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 1.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getRockColor(), sf::Vector2f(0.3f, 0.5f), mapSize, 10.f, 5, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
		return noise.perlin(x, y, 3, 4.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getRockColor(), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, 0, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
		return noise.perlin(x, y + 100.f, 3, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(m_tileStartColor, sf::Vector2f(0.5f, 0.3f), mapSize, 12.f, 0, 0.1f, 0.3f, 6.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 1.1f, y);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(m_tileStartColor, sf::Vector2f(0.6f, 0.2f), sf::Vector2u(mapSize.x, mapSize.y * 1.6f), 12.f, -90, 0.4f, 0.3f, 6.f, 3000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 20.f, y);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(m_tileStartColor, sf::Vector2f(0.7f, 0.1f), sf::Vector2u(mapSize.x, mapSize.y * 1.2f), 12.f, -330, 0.5f, 0.1f, 6.f, 4000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 20.f, y + 100.f);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator JungleBBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f) - 0.3f;
		//float m = n / 3.f;
		std::vector<float> pointX = {0.f     , 35.f , 44.f    , 45.f, 55.f, 56.f , 60.f , 90.f          , 200.f    , 320.f, 359.f, 381.f, 382.f, 389.f, 415.f, 417.f , 465.f, 550.f};
		std::vector<float> pointY = {n - 1.2f, -1.7f, n - 1.2f, 5.f , 5.f , -0.1f, -0.6f, n / 2.f - 0.7f, n - 0.7f, -0.1f, -0.1f, 0.f  , 5.f  , 3.38f, 3.38f, -1.05f, -1.1f, n - 1.1f};
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

Map::TileColorGenerator JungleBBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(76, 70, 102);
	sf::Color secondColorEnd(56, 50, 72);
	float startTransition = -12000.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 6000.f / static_cast<float>(m_mapSize.y);
	float endTransition = 30000.f / static_cast<float>(m_mapSize.y);
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

sf::Color		JungleBBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		JungleBBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		JungleBBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		JungleBBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		JungleBBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		JungleBBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		JungleBBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		JungleBBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	JungleBBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		JungleBBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			JungleBBiome::getWind()
{
	return (m_wind);
}

void			JungleBBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			JungleBBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		JungleBBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		JungleBBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		JungleBBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			JungleBBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			JungleBBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			JungleBBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		JungleBBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		JungleBBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		JungleBBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		JungleBBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		JungleBBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		JungleBBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		JungleBBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		JungleBBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		JungleBBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		JungleBBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	JungleBBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	JungleBBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		JungleBBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		JungleBBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			JungleBBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			JungleBBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			JungleBBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			JungleBBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			JungleBBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	JungleBBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleBBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		JungleBBiome::getTreePositionX()
{
	return randomInt(430u, m_mapSize.x - 10u);
}

sf::Vector2f	JungleBBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		JungleBBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		JungleBBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				JungleBBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			JungleBBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	JungleBBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		JungleBBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			JungleBBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			JungleBBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	JungleBBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		JungleBBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		JungleBBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	JungleBBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	JungleBBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	JungleBBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	JungleBBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	JungleBBiome::getGrassPosX()
{
	return randomInt(0u, m_mapSize.x);
}

bool			JungleBBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	JungleBBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		JungleBBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		JungleBBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			JungleBBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	JungleBBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		JungleBBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	JungleBBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	JungleBBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	JungleBBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		JungleBBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		JungleBBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			JungleBBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	JungleBBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		JungleBBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		JungleBBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			JungleBBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	JungleBBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		JungleBBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		JungleBBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			JungleBBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	JungleBBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleBBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		JungleBBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			JungleBBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			JungleBBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			JungleBBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		JungleBBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		JungleBBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		JungleBBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			JungleBBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	JungleBBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	JungleBBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	JungleBBiome::getType() const
{
	return m_type;
}


float			JungleBBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				JungleBBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			JungleBBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			JungleBBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				JungleBBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	JungleBBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		JungleBBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		JungleBBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
