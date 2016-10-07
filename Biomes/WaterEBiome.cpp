#include "WaterEBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

WaterEBiome::WaterEBiome() :
	m_name("Water D"),
	m_id(Level::WaterA),
	m_seed("Water D"),
	m_mapSize(sf::Vector2u(1200u, 32u)),
	m_mapSeed(42u),
	m_octoStartPosition(73.f * 16.f, -180.f * 16.f),
	m_transitionDuration(2.0f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(250, 229, 205),
	m_tileEndColor(244, 201, 154),
	m_waterLevel(-1.f),
	m_waterColor(3, 57, 108, 130),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(90.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(5, 0, 0),
	m_skyNightColor(5, 0, 0),
	m_nightLightColor(255, 90, 61, 130),
	m_dayLightColor(sf::Color::Transparent),
	m_SunsetLightColor(255, 147, 46, 130),
	m_wind(30.f),
	m_rainDropPerSecond(10u, 15u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(10u, 15u),
	m_treeCount(30u, 30u),
	m_mushroomCount(390u, 400u),
	m_crystalCount(20u, 30u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 2u),
	m_rainbowCount(2u, 2u),
	m_cloudCount(30u, 40u),
	m_groundRockCount(200u, 400u),

	m_canCreateRain(true),
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

	m_grassSizeY(80.f, 90.f),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(159, 24, 24, 150),
	m_grassCount(m_mapSize.x),

	m_treeDepth(6u, 7u),
	m_treeSize(sf::Vector2f(5.f, 100.f), sf::Vector2f(20.f, 120.f)),
	m_treeLifeTime(sf::seconds(20.f), sf::seconds(50.f)),
	m_treeColor(103, 157, 208, 50),
	m_treeAngle(-180.f, 180.f),
	m_treeBeatMouvement(0.01f),
	m_leafSize(sf::Vector2f(20.f, 20.f), sf::Vector2f(150.f, 150.f)),
	m_leafColor(103, 157, 208, 50),

	m_mushroomSize(sf::Vector2f(10.f, 20.f), sf::Vector2f(20.f, 50.f)),
	m_mushroomColor(255, 0, 0, 150.f),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(20.f, 150.f), sf::Vector2f(40.f, 350.f)),
	m_crystalPartCount(3u, 6u),
	m_crystalColor(103, 157, 208, 50),
	m_shineEffectSize(sf::Vector2f(200.f, 200.f), sf::Vector2f(300.f, 300.f)),
	m_shineEffectColor(153, 207, 255, 130),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(50.f, 10.f), sf::Vector2f(100.f, 20.f)),
	m_cloudPartCount(3u, 5u),
	m_cloudMaxY(m_waterLevel),
	m_cloudMinY(m_waterLevel),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(10.f, 10.f), sf::Vector2f(20.f, 20.f)),
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
	m_mapSeed = 42;//m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif
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

	m_instances[50] = MAP_WATER_E_TRAIL_OMP;

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

void			WaterEBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		WaterEBiome::getName()const
{
	return (m_name);
}

Level			WaterEBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	WaterEBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		WaterEBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	WaterEBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	WaterEBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			WaterEBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				WaterEBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	WaterEBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	WaterEBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	WaterEBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	WaterEBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	WaterEBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

bool		WaterEBiome::isDeadlyWater()
{
	return false;
}

bool		WaterEBiome::isDeadlyGrass()
{
	return true;
}

sf::Time	WaterEBiome::getTimeDieVoid()
{
	return (sf::seconds(3.f));
}

std::map<std::size_t, std::string> const & WaterEBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> WaterEBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(m_skyDayColor, sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -40, 0.1f, 0.8f, -1.f, 2000.f);
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

Map::MapSurfaceGenerator WaterEBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		std::vector<float> pointX = {0.f , 1200.f};
		std::vector<float> pointY = {50.f, 50.f};
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

Map::TileColorGenerator WaterEBiome::getTileColorGenerator()
{
	sf::Color secondColorStart = getRockColor();
	sf::Color secondColorEnd = getRockColor();
	sf::Color thirdColorStart(53, 107, 208);
	sf::Color thirdColorEnd(103, 157, 208);
	float start1 = -3900.f / static_cast<float>(m_mapSize.y);
	float start2 = -2200.f / static_cast<float>(m_mapSize.y);
	float middle1 = 0.f / static_cast<float>(m_mapSize.y);
	float middle2 = 200.f / static_cast<float>(m_mapSize.y);
	float end1 = 1000.f / static_cast<float>(m_mapSize.y);
	float end2 = 1700.f / static_cast<float>(m_mapSize.y);
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

sf::Color		WaterEBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		WaterEBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		WaterEBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		WaterEBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		WaterEBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		WaterEBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		WaterEBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		WaterEBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	WaterEBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		WaterEBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			WaterEBiome::getWind()
{
	return (m_wind);
}

void			WaterEBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			WaterEBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		WaterEBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		WaterEBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		WaterEBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			WaterEBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			WaterEBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			WaterEBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		WaterEBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		WaterEBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		WaterEBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		WaterEBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		WaterEBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		WaterEBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		WaterEBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		WaterEBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		WaterEBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		WaterEBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	WaterEBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	WaterEBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		WaterEBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		WaterEBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			WaterEBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			WaterEBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			WaterEBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			WaterEBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			WaterEBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	WaterEBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterEBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		WaterEBiome::getTreePositionX()
{
	return randomInt(6.f, 110.f);
}

sf::Vector2f	WaterEBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		WaterEBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		WaterEBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				WaterEBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			WaterEBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	WaterEBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		WaterEBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			WaterEBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			WaterEBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	WaterEBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		WaterEBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		WaterEBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	WaterEBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	WaterEBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	WaterEBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	WaterEBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	WaterEBiome::getGrassPosX()
{
	return randomInt(0u, m_mapSize.x);
}

bool			WaterEBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	WaterEBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		WaterEBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		WaterEBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			WaterEBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	WaterEBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		WaterEBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	WaterEBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	WaterEBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	WaterEBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		WaterEBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		WaterEBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			WaterEBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	WaterEBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		WaterEBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		WaterEBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			WaterEBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	WaterEBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		WaterEBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		WaterEBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			WaterEBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	WaterEBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterEBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		WaterEBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			WaterEBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			WaterEBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			WaterEBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		WaterEBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		WaterEBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		WaterEBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			WaterEBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	WaterEBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	WaterEBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	WaterEBiome::getType() const
{
	return m_type;
}


float			WaterEBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				WaterEBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			WaterEBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			WaterEBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				WaterEBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	WaterEBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		WaterEBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		WaterEBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}