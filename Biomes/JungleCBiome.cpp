#include "JungleCBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

JungleCBiome::JungleCBiome() :
	m_name("Jungle C"),
	m_id(Level::JungleC),
	m_seed("Jungle C"),
	m_mapSize(sf::Vector2u(920u, 64u)),
	m_mapSeed(42u),
	m_octoStartPosition(134.f * 16.f, -1650.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(0, 76, 54),
	m_tileEndColor(40, 164, 144),
	m_waterLevel(300.f),
	m_waterColor(196, 235, 1, 150),
	m_secondWaterColor(0, 189, 168, 150),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(80.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(251, 253, 26),
	m_skyNightColor(195, 197, 18),
	m_nightLightColor(0, 0, 0, 130),
	m_dayLightColor(sf::Color::Transparent),
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

	m_grassSizeY(50.f, 170.f),
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
	m_cloudMaxY(350.f),
	m_cloudMinY(-2000.f),
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
	m_mapSeed = 42;//m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif

	m_tileStartColor = getLeafColor();
	m_tileEndColor = getLeafColor();
	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::JungleD)
		m_octoStartPosition = sf::Vector2f(868.f * 16.f, 100.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(167.f * 16.f, -2600.f);

	m_gameObjects[5] = GameObjectType::WindowGlitchNpc;
	m_gameObjects[40] = GameObjectType::BirdRedNpc;
	m_gameObjects[100] = GameObjectType::FishRedNpc;
	m_gameObjects[115] = GameObjectType::BirdRedNpc;
	m_instances[120] = MAP_JUNGLE_C_TRAIL_OMP;

	for (std::size_t i = 0; i < 900; i += m_generator.randomInt(10u, 60u))
	{
		if (m_generator.randomBool(0.5))
			m_gameObjects[i] = GameObjectType::ForestSpirit1Npc;
		else
			m_gameObjects[i] = GameObjectType::ForestSpirit2Npc;
	}

	m_interestPointPosX = 500;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::JungleB);
	m_destinations.push_back(Level::JungleD);
	m_destinations.push_back(Level::Random);
}

void			JungleCBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		JungleCBiome::getName()const
{
	return (m_name);
}

Level			JungleCBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	JungleCBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		JungleCBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	JungleCBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	JungleCBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			JungleCBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				JungleCBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	JungleCBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	JungleCBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	JungleCBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	JungleCBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	JungleCBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

bool		JungleCBiome::isDeadlyWater()
{
	return true;
}

std::map<std::size_t, std::string> const & JungleCBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> JungleCBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(m_skyDayColor, sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -40, 0.1f, 0.8f, 10.f, 2000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 10.f, y * 10.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(m_skyDayColor, sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -20, 0.3f, 0.6f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 30.f, y);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator JungleCBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		m_pointX = { 0.f , 20.f, 40.f   , 70.f   , 110.f, 120.f, 155.f, 915.f};
		m_pointY = { -0.1f, n   , n + 6.f, n + 6.f, n    , -0.2f, 0.2f , 0.2f};
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

Map::TileColorGenerator JungleCBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(76, 70, 102);
	sf::Color secondColorEnd(56, 50, 72);
	sf::Color thirdColorStart = m_skyDayColor;
	sf::Color thirdColorEnd = m_skyNightColor;
	float start1 = -12000.f / static_cast<float>(m_mapSize.y);
	float start2 = -1500.f / static_cast<float>(m_mapSize.y);
	float middle1 = 300.f / static_cast<float>(m_mapSize.y);
	float middle2 = 1450.f / static_cast<float>(m_mapSize.y);
	float end1 = 1700.f / static_cast<float>(m_mapSize.y);
	float end2 = 10000.f / static_cast<float>(m_mapSize.y);
	return [this, secondColorStart, secondColorEnd, thirdColorStart, thirdColorEnd, start1, start2, middle1, middle2, end1, end2](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		if (y > start1 && y <= start2)
		{
			float ratio = (y - (start1)) / (start2 - start1);
			return octo::linearInterpolation(octo::linearInterpolation(m_tileStartColor, secondColorStart, ratio), m_tileEndColor, transition);
		}
		else if (y > start2 && y <= middle1)
		{
			float ratio = (y - (start2)) / (middle1 - start2);
			return octo::linearInterpolation(secondColorStart, octo::linearInterpolation(m_tileEndColor, secondColorEnd, ratio), transition);
		}
		else if (y > middle1 && y <= middle2)
		{
			return octo::linearInterpolation(secondColorStart, secondColorEnd, transition);
		}
		else if (y >= middle2 && y < end1)
		{
			float ratio = (y - (middle2)) / (end1 - middle2);
			return octo::linearInterpolation(octo::linearInterpolation(secondColorStart, thirdColorStart, ratio), secondColorEnd, transition);
		}
		else if (y >= end1 && y < end2)
		{
			float ratio = (y - (end1)) / (end2 - end1);
			return octo::linearInterpolation(thirdColorStart, octo::linearInterpolation(secondColorEnd, thirdColorEnd, ratio), transition);
		}
		return octo::linearInterpolation(thirdColorStart, thirdColorEnd, transition);
	};
}

sf::Color		JungleCBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		JungleCBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		JungleCBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		JungleCBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		JungleCBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		JungleCBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		JungleCBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		JungleCBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	JungleCBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		JungleCBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			JungleCBiome::getWind()
{
	return (m_wind);
}

void			JungleCBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			JungleCBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		JungleCBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		JungleCBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		JungleCBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			JungleCBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			JungleCBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			JungleCBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		JungleCBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		JungleCBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		JungleCBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		JungleCBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		JungleCBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		JungleCBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		JungleCBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		JungleCBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		JungleCBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		JungleCBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	JungleCBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	JungleCBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		JungleCBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		JungleCBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			JungleCBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			JungleCBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			JungleCBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			JungleCBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			JungleCBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	JungleCBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleCBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		JungleCBiome::getTreePositionX()
{
	return randomInt(6.f, 110.f);
}

sf::Vector2f	JungleCBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		JungleCBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		JungleCBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				JungleCBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			JungleCBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	JungleCBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		JungleCBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			JungleCBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			JungleCBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	JungleCBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		JungleCBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		JungleCBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	JungleCBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	JungleCBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	JungleCBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	JungleCBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	JungleCBiome::getGrassPosX()
{
	return randomInt(0u, m_mapSize.x);
}

bool			JungleCBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	JungleCBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		JungleCBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		JungleCBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			JungleCBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	JungleCBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		JungleCBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	JungleCBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	JungleCBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	JungleCBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		JungleCBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		JungleCBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			JungleCBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	JungleCBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		JungleCBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		JungleCBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			JungleCBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	JungleCBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		JungleCBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		JungleCBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			JungleCBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	JungleCBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleCBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		JungleCBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			JungleCBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			JungleCBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			JungleCBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		JungleCBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		JungleCBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		JungleCBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			JungleCBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	JungleCBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	JungleCBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	JungleCBiome::getType() const
{
	return m_type;
}


float			JungleCBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				JungleCBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			JungleCBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			JungleCBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				JungleCBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	JungleCBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		JungleCBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		JungleCBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
