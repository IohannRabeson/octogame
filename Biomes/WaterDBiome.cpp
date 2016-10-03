#include "WaterDBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

WaterDBiome::WaterDBiome() :
	m_name("Water D"),
	m_id(Level::WaterA),
	m_seed("Water D"),
	m_mapSize(sf::Vector2u(1200u, 32u)),
	m_mapSeed(42u),
	m_octoStartPosition(63.f * 16.f, -200.f * 16.f),
	//m_octoStartPosition(63.f * 16.f, -500.f * 16.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(250, 229, 205),
	m_tileEndColor(244, 201, 154),
	m_waterLevel(340.f),
	m_waterColor(3, 57, 108, 130),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(90.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(255, 0, 0),
	m_skyNightColor(255, 0, 0),
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

	m_instances[50] = MAP_WATER_D_TRAIL_OMP;

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

void			WaterDBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		WaterDBiome::getName()const
{
	return (m_name);
}

Level			WaterDBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	WaterDBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		WaterDBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	WaterDBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	WaterDBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			WaterDBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				WaterDBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	WaterDBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	WaterDBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	WaterDBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	WaterDBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	WaterDBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

bool		WaterDBiome::isDeadlyWater()
{
	return false;
}

bool		WaterDBiome::isDeadlyGrass()
{
	return true;
}

std::map<std::size_t, std::string> const & WaterDBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> WaterDBiome::getLayers()
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

Map::MapSurfaceGenerator WaterDBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		std::vector<float> pointX = { 0.f , 20.f, 40.f   , 70.f   , 110.f, 120.f, 155.f, 915.f};
		std::vector<float> pointY = { -0.1f, n   , n + 6.f, n + 6.f, n    , -0.2f, 0.2f , 0.2f};
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

Map::TileColorGenerator WaterDBiome::getTileColorGenerator()
{
	sf::Color secondColorStart = m_tileStartColor;//getRockColor();
	sf::Color secondColorEnd = m_tileEndColor;//getRockColor();
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

sf::Color		WaterDBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		WaterDBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		WaterDBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		WaterDBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		WaterDBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		WaterDBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		WaterDBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		WaterDBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	WaterDBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		WaterDBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			WaterDBiome::getWind()
{
	return (m_wind);
}

void			WaterDBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			WaterDBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		WaterDBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		WaterDBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		WaterDBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			WaterDBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			WaterDBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			WaterDBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		WaterDBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		WaterDBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		WaterDBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		WaterDBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		WaterDBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		WaterDBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		WaterDBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		WaterDBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		WaterDBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		WaterDBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	WaterDBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	WaterDBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		WaterDBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		WaterDBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			WaterDBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			WaterDBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			WaterDBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			WaterDBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			WaterDBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	WaterDBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterDBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		WaterDBiome::getTreePositionX()
{
	return randomInt(6.f, 110.f);
}

sf::Vector2f	WaterDBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		WaterDBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		WaterDBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				WaterDBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			WaterDBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	WaterDBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		WaterDBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			WaterDBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			WaterDBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	WaterDBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		WaterDBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		WaterDBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	WaterDBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	WaterDBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	WaterDBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	WaterDBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	WaterDBiome::getGrassPosX()
{
	return randomInt(0u, m_mapSize.x);
}

bool			WaterDBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	WaterDBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		WaterDBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		WaterDBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			WaterDBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	WaterDBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		WaterDBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	WaterDBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	WaterDBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	WaterDBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		WaterDBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		WaterDBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			WaterDBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	WaterDBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		WaterDBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		WaterDBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			WaterDBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	WaterDBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		WaterDBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		WaterDBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			WaterDBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	WaterDBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterDBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		WaterDBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			WaterDBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			WaterDBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			WaterDBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		WaterDBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		WaterDBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		WaterDBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			WaterDBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	WaterDBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	WaterDBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	WaterDBiome::getType() const
{
	return m_type;
}


float			WaterDBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				WaterDBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			WaterDBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			WaterDBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				WaterDBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	WaterDBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		WaterDBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		WaterDBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
