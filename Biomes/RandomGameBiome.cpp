#include "RandomGameBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

// Biomes
#include "IceABiome.hpp"
#include "IceBBiome.hpp"
#include "IceCBiome.hpp"
#include "IceDBiome.hpp"
#include "DesertABiome.hpp"
#include "DesertBBiome.hpp"
#include "DesertCBiome.hpp"
#include "DesertDBiome.hpp"
#include "JungleABiome.hpp"
#include "JungleBBiome.hpp"
#include "JungleCBiome.hpp"
#include "JungleDBiome.hpp"
#include "WaterABiome.hpp"
#include "WaterBBiome.hpp"
#include "WaterCBiome.hpp"
#include "WaterDBiome.hpp"
#include "FinalBiome.hpp"
#include "RedBiome.hpp"
#include "BlueBiome.hpp"
#include "PortalBiome.hpp"
#include "EndRocketBiome.hpp"
#include "EndTimeLapseBiome.hpp"
#include "RandomBiome.hpp"
#include "RewardsBiome.hpp"

#include <ctime>
#include <limits>
#include <iostream>

std::size_t RandomGameBiome::m_seedId = 110u;

RandomGameBiome::RandomGameBiome() :
	m_specialClouds(false),
	m_generator("random"),
	m_mapSeed(m_generator.randomInt(2u, 100000u)),
	m_tileStartColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_tileEndColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),
	m_waterColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(40, 150)),
	m_secondWaterColor(m_waterColor),

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

	m_rockCount(m_generator.randomInt(1, 40), m_generator.randomInt(40, 100)),
	m_treeCount(m_generator.randomInt(1, 20), m_generator.randomInt(20, 50)),
	m_mushroomCount(m_generator.randomInt(1, 100), m_generator.randomInt(100, 250)),
	m_crystalCount(m_generator.randomInt(1, 30), m_generator.randomInt(30, 100)),
	m_starCount(300u, 800u),
	m_sunCount(m_generator.randomInt(1, 3), m_generator.randomInt(3, 5)),
	m_moonCount(m_generator.randomInt(1, 2), m_generator.randomInt(2, 5)),
	m_rainbowCount(m_generator.randomInt(1, 3), m_generator.randomInt(3, 10)),
	m_cloudCount(m_generator.randomInt(1, 50), m_generator.randomInt(50, 100)),
	m_groundRockCount(m_generator.randomInt(100, 200), m_generator.randomInt(200, 700)),

	m_canCreateRain(m_generator.randomBool(0.5f)),
	m_canCreateThunder(m_generator.randomBool(0.6f)),
	m_canCreateSnow(m_generator.randomBool(0.5f)),
	m_canCreateRock(m_generator.randomBool(0.5f)),
	m_canCreateTree(m_generator.randomBool(0.5f)),
	m_canCreateLeaf(m_generator.randomBool(0.5f)),
	m_treeIsMoving(m_generator.randomBool(0.8f)),
	m_canCreateMushroom(m_generator.randomBool(0.5f)),
	m_canCreateCrystal(m_generator.randomBool(0.5f)),
	m_canCreateShineEffect(m_generator.randomBool(0.9f)),
	m_canCreateCloud(m_generator.randomBool(0.5f)),
	m_canCreateStar(m_generator.randomBool(0.9f)),
	m_canCreateSun(m_generator.randomBool(0.7f)),
	m_canCreateMoon(m_generator.randomBool(0.8f)),
	m_canCreateRainbow(m_generator.randomBool(0.4f)),
	m_canCreateGrass(m_generator.randomBool(0.5f)),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Random),

	m_rockSize(sf::Vector2f(m_generator.randomFloat(2.f, 50.f), m_generator.randomFloat(10.f, 60.f)), sf::Vector2f(m_generator.randomFloat(50.f, 100.f), m_generator.randomFloat(200.f, 600.f))),
	m_rockPartCount(m_generator.randomInt(2.f, 4.f), m_generator.randomFloat(4.f, 20.f)),
	m_rockColor(m_generator.randomInt(0, 255), m_generator.randomInt(0, 255), m_generator.randomInt(0, 255)),

	m_grassSizeY(m_generator.randomFloat(10.f, 0.f), m_generator.randomFloat(60.f, 200.f)),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(m_tileStartColor),
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
	m_biomeManager.registerBiome<IceABiome>(Level::IceA);
	m_biomeManager.registerBiome<IceBBiome>(Level::IceB);
	m_biomeManager.registerBiome<IceCBiome>(Level::IceC);
	m_biomeManager.registerBiome<IceDBiome>(Level::IceD);
	m_biomeManager.registerBiome<DesertABiome>(Level::DesertA);
	m_biomeManager.registerBiome<DesertBBiome>(Level::DesertB);
	m_biomeManager.registerBiome<DesertCBiome>(Level::DesertC);
	m_biomeManager.registerBiome<DesertDBiome>(Level::DesertD);
	m_biomeManager.registerBiome<JungleABiome>(Level::JungleA);
	m_biomeManager.registerBiome<JungleBBiome>(Level::JungleB);
	m_biomeManager.registerBiome<JungleCBiome>(Level::JungleC);
	m_biomeManager.registerBiome<JungleDBiome>(Level::JungleD);
	m_biomeManager.registerBiome<WaterABiome>(Level::WaterA);
	m_biomeManager.registerBiome<WaterBBiome>(Level::WaterB);
	m_biomeManager.registerBiome<WaterCBiome>(Level::WaterC);
	m_biomeManager.registerBiome<WaterDBiome>(Level::WaterD);
	m_biomeManager.registerBiome<FinalBiome>(Level::Final);
	m_biomeManager.registerBiome<RedBiome>(Level::Red);
	m_biomeManager.registerBiome<BlueBiome>(Level::Blue);
	m_biomeManager.registerBiome<PortalBiome>(Level::Portal);

	m_biomeManager.registerBiome<EndRocketBiome>(Level::EndRocket);
	m_biomeManager.registerBiome<EndTimeLapseBiome>(Level::EndTimeLapse);
	m_biomeManager.registerBiome<RandomBiome>(Level::Random);
	m_biomeManager.registerBiome<RewardsBiome>(Level::Rewards);
	m_biomeManager.registerBiome<RandomGameBiome>(Level::RandomGame);

	setBiome(Progress::getInstance().getNextDestination());
	m_name = m_biomeManager.getCurrentBiome().getName();
	m_id = m_biomeManager.getCurrentBiome().getId();
	m_grassCount = m_biomeManager.getCurrentBiome().getMapSize().x;
	m_specialClouds = randomBool(0.07f);
}

void			RandomGameBiome::setBiome(Level level)
{
	m_biomeManager.changeBiome(level, 0x12345);
}

void			RandomGameBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		RandomGameBiome::getName()const
{
	return m_name;
}

Level			RandomGameBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	RandomGameBiome::getMapSize()
{
	return m_biomeManager.getCurrentBiome().getMapSize();
}

std::size_t		RandomGameBiome::getMapSeed()
{
	return m_biomeManager.getCurrentBiome().getMapSeed();
}

sf::Vector2f	RandomGameBiome::getMapSizeFloat()
{
	return m_biomeManager.getCurrentBiome().getMapSizeFloat();
}

sf::Vector2f	RandomGameBiome::getOctoStartPosition()
{
	return m_biomeManager.getCurrentBiome().getOctoStartPosition();
}

float			RandomGameBiome::getTransitionDuration()
{
	return m_biomeManager.getCurrentBiome().getTransitionDuration();
}

int				RandomGameBiome::getInterestPointPosX()
{
	return m_biomeManager.getCurrentBiome().getInterestPointPosX();
}

std::map<std::size_t, GameObjectType> const &	RandomGameBiome::getGameObjects()
{
	return m_biomeManager.getCurrentBiome().getGameObjects();
}

Level	RandomGameBiome::getDestination()
{
	return m_biomeManager.getCurrentBiome().getDestination();
}

float	RandomGameBiome::getWaterLevel()
{
	return m_biomeManager.getCurrentBiome().getWaterLevel();
}

sf::Color	RandomGameBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	RandomGameBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

bool		RandomGameBiome::isDeadlyWater()
{
	return m_biomeManager.getCurrentBiome().isDeadlyWater();
}

bool		RandomGameBiome::isDeadlyGrass()
{
	return m_biomeManager.getCurrentBiome().isDeadlyGrass();
}

bool		RandomGameBiome::isSpecialCloud()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return true;
	else if (m_specialClouds)
		return true;
	return false;
}

sf::Time	RandomGameBiome::getTimeDieVoid()
{
	return m_biomeManager.getCurrentBiome().getTimeDieVoid();
}

std::map<std::size_t, std::string> const & RandomGameBiome::getInstances()
{
	return m_biomeManager.getCurrentBiome().getInstances();
}

std::vector<ParallaxScrolling::ALayer *> RandomGameBiome::getLayers()
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

Map::MapSurfaceGenerator RandomGameBiome::getMapSurfaceGenerator()
{
	return (m_biomeManager.getCurrentBiome().getMapSurfaceGenerator());
}

Map::TileColorGenerator RandomGameBiome::getTileColorGenerator()
{
	return [this](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		RandomGameBiome::getParticleColorGround()
{
	return m_biomeManager.getCurrentBiome().getParticleColorGround();
}

sf::Color		RandomGameBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		RandomGameBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		RandomGameBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		RandomGameBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		RandomGameBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		RandomGameBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		RandomGameBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	RandomGameBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		RandomGameBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			RandomGameBiome::getWind()
{
	return (m_wind);
}

void			RandomGameBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			RandomGameBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		RandomGameBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		RandomGameBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		RandomGameBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			RandomGameBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			RandomGameBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			RandomGameBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		RandomGameBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		RandomGameBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		RandomGameBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		RandomGameBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		RandomGameBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		RandomGameBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		RandomGameBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		RandomGameBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		RandomGameBiome::getCloudCount()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().getCloudCount());
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		RandomGameBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	RandomGameBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	RandomGameBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		RandomGameBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		RandomGameBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			RandomGameBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			RandomGameBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			RandomGameBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			RandomGameBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			RandomGameBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	RandomGameBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		RandomGameBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		RandomGameBiome::getTreePositionX()
{
	return randomInt(1, m_biomeManager.getCurrentBiome().getMapSize().x);
}

sf::Vector2f	RandomGameBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		RandomGameBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		RandomGameBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				RandomGameBiome::getCrystalPosX()
{
	return m_biomeManager.getCurrentBiome().getCrystalPosX();
}

bool			RandomGameBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	RandomGameBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		RandomGameBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			RandomGameBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			RandomGameBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	RandomGameBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		RandomGameBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		RandomGameBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	RandomGameBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	RandomGameBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	RandomGameBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	RandomGameBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	RandomGameBiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_biomeManager.getCurrentBiome().getMapSize().x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			RandomGameBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	RandomGameBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		RandomGameBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		RandomGameBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			RandomGameBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	RandomGameBiome::getCloudSize()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().getCloudSize());
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		RandomGameBiome::getCloudPartCount()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().getCloudPartCount());
	return (randomRangeSizeT(m_cloudPartCount));
}

float	RandomGameBiome::getCloudMaxY()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().getCloudMaxY());
	return (m_cloudMaxY);
}

float	RandomGameBiome::getCloudMinY()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().getCloudMinY());
	return (m_cloudMinY);
}

sf::Vector2f	RandomGameBiome::getCloudSpeed()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().getCloudSpeed());
	return m_biomeManager.getCurrentBiome().getCloudSpeed();
}

sf::Time		RandomGameBiome::getCloudLifeTime()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().getCloudLifeTime());
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		RandomGameBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			RandomGameBiome::canCreateCloud()
{
	if (m_biomeManager.getCurrentBiome().isSpecialCloud())
		return (m_biomeManager.getCurrentBiome().canCreateCloud());
	return (m_canCreateCloud);
}

sf::Vector2f	RandomGameBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		RandomGameBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		RandomGameBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			RandomGameBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	RandomGameBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		RandomGameBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		RandomGameBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			RandomGameBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	RandomGameBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		RandomGameBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		RandomGameBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			RandomGameBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			RandomGameBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			RandomGameBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		RandomGameBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		RandomGameBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		RandomGameBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			RandomGameBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	RandomGameBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	RandomGameBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	RandomGameBiome::getType() const
{
	return m_type;
}


float			RandomGameBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				RandomGameBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			RandomGameBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			RandomGameBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				RandomGameBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	RandomGameBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		RandomGameBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		RandomGameBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
