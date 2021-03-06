#include "DesertCBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

DesertCBiome::DesertCBiome() :
	m_name("Desert B"),
	m_id(Level::DesertC),
	m_seed("Cailloux"),
	m_mapSize(sf::Vector2u(915u, 64u)),
	m_mapSeed(42u),
	m_octoStartPosition(113.f * 16.f, -100.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(245, 222, 130),
	m_tileEndColor(245, 243, 249),
	m_waterLevel(300.f),
	m_waterColor(18, 14, 16, 200),
	m_secondWaterColor(96, 204, 233, 130),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(100.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(255, 150, 242),
	m_skyNightColor(166, 10, 92),
	m_nightLightColor(134, 63, 215, 130),
	m_dayLightColor(sf::Color::Transparent),
	m_SunsetLightColor(255, 59, 59, 130),
	m_wind(0.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(10u, 20u),
	m_treeCount(30u, 30u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(100u, 120u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 3u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(200u, 201u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(false),
	m_canCreateRock(false),
	m_canCreateTree(true),
	m_canCreateLeaf(true),
	m_treeIsMoving(true),
	m_canCreateMushroom(false),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(true),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_canCreateGrass(false),
	m_waterPersistence(1.f),
	m_type(ABiome::Type::None),

	m_rockSize(sf::Vector2f(5.f, 70.f), sf::Vector2f(20.f, 100.f)),
	m_rockPartCount(50.f, 80.f),
	m_rockColor(255, 232, 170),

	m_grassSizeY(30.f, 60.f),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),

	m_treeDepth(4u, 5u),
	m_treeSize(sf::Vector2f(4.f, 15.f), sf::Vector2f(8.f, 30.f)),
	m_treeLifeTime(sf::seconds(4), sf::seconds(10)),
	m_treeColor(245, 222, 130),
	m_treeAngle(5.f, 10.f),
	m_treeBeatMouvement(0.1f),
	m_leafSize(sf::Vector2f(10.f, 10.f), sf::Vector2f(30.f, 30.f)),
	m_leafColor(46, 133, 84),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(10), sf::seconds(30)),

	m_crystalSize(sf::Vector2f(10.f, 80.f), sf::Vector2f(25.f, 150.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(18, 14, 66, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(60.f, 60.f)),
	m_cloudPartCount(1u, 1u),
	m_cloudMaxY(3000.f),
	m_cloudMinY(360.f),
	m_cloudSpeed(sf::Vector2f(0.f, -50.f), sf::Vector2f(0.f, -80.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(18, 14, 66, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(300.f, 300.f), sf::Vector2f(350.f, 350.f)),
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
	if (progress.getLastDestination() == Level::DesertD)
		m_octoStartPosition = sf::Vector2f(33.f * 16.f, -550.f);
	else if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(10.f * 16.f, -1900.f);

	m_gameObjects[5] = GameObjectType::WindowGlitchNpc;
	m_gameObjects[30] = GameObjectType::PortalJungle;
	m_gameObjects[110] = GameObjectType::PortalDesert;
	m_instances[120] = MAP_DESERT_C_TRAIL_OMP;
	m_instances[15] = MAP_DESERT_C_SECRET_PORTAL_OMP;

	m_interestPointPosX = 500;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::DesertD);
	m_destinations.push_back(Level::DesertB);
}

void			DesertCBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		DesertCBiome::getName()const
{
	return (m_name);
}

Level			DesertCBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	DesertCBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		DesertCBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	DesertCBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	DesertCBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			DesertCBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DesertCBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	DesertCBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	DesertCBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	DesertCBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	DesertCBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	DesertCBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

bool		DesertCBiome::isDeadlyWater()
{
	return true;
}

std::map<std::size_t, std::string> const & DesertCBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> DesertCBiome::getLayers()
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

Map::MapSurfaceGenerator DesertCBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		m_pointX = { 0.f , 20.f    , 25.f , 49.f , 50.f , 51.f , 52.f , 53.f  , 140.f, 145.f, 915.f};
		m_pointY = { 0.0f, n - 0.2f, -1.8f, -1.8f, -0.75f, -0.6f, -0.5f, -0.45f, -0.5f, 0.2f, 0.2f};
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

Map::TileColorGenerator DesertCBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(245, 222, 130);
	sf::Color secondColorEnd(245, 243, 249);
	sf::Color thirdColorStart = m_skyDayColor;
	sf::Color thirdColorEnd = m_skyNightColor;
	float start1 = -10000.f / static_cast<float>(m_mapSize.y);
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
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		DesertCBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		DesertCBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		DesertCBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		DesertCBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		DesertCBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		DesertCBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		DesertCBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		DesertCBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	DesertCBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		DesertCBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			DesertCBiome::getWind()
{
	return (m_wind);
}

void			DesertCBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			DesertCBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		DesertCBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		DesertCBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		DesertCBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			DesertCBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			DesertCBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			DesertCBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		DesertCBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		DesertCBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		DesertCBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		DesertCBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		DesertCBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		DesertCBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		DesertCBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		DesertCBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		DesertCBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		DesertCBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	DesertCBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	DesertCBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		DesertCBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		DesertCBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			DesertCBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			DesertCBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			DesertCBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			DesertCBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			DesertCBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	DesertCBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertCBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		DesertCBiome::getTreePositionX()
{
	return randomInt(24.f, 50.f);
}

sf::Vector2f	DesertCBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		DesertCBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		DesertCBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				DesertCBiome::getCrystalPosX()
{
	return (randomInt(1u, m_mapSize.x));
}

bool			DesertCBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	DesertCBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		DesertCBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			DesertCBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			DesertCBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	DesertCBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		DesertCBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		DesertCBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	DesertCBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	DesertCBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	DesertCBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	DesertCBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	DesertCBiome::getGrassPosX()
{
	return randomInt(0u, m_mapSize.x);
}

bool			DesertCBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	DesertCBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		DesertCBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		DesertCBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			DesertCBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	DesertCBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		DesertCBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	DesertCBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	DesertCBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	DesertCBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		DesertCBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		DesertCBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			DesertCBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	DesertCBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		DesertCBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		DesertCBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			DesertCBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	DesertCBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		DesertCBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		DesertCBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			DesertCBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	DesertCBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertCBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		DesertCBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			DesertCBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DesertCBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			DesertCBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		DesertCBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		DesertCBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		DesertCBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			DesertCBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	DesertCBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	DesertCBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	DesertCBiome::getType() const
{
	return m_type;
}


float			DesertCBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				DesertCBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			DesertCBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			DesertCBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				DesertCBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	DesertCBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		DesertCBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		DesertCBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
