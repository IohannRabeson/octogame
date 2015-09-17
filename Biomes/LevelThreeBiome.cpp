#include "LevelThreeBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

LevelThreeBiome::LevelThreeBiome() :
	m_name("LevelThree"),
	m_id(Level::LevelThree),
	m_seed("LevelThree"),
	m_mapSize(sf::Vector2u(900u, 128u)),
	m_mapSeed(42u),
	m_octoStartPosition(50.f * 16.f, 650.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(255, 245, 217),
	m_tileEndColor(255, 252, 181),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(100.f)),
	m_startDayDuration(sf::seconds(15.f)),
	m_skyDayColor(255,156,103),
	m_skyNightColor(8, 20, 26),
	m_nightLightColor(0, 197, 255, 130),
	m_SunsetLightColor(238, 173, 181, 130),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(10u, 20u),
	m_treeCount(13u, 13u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(10u, 15u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 3u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(20u, 40u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(false),
	m_canCreateRock(true),
	m_canCreateTree(true),
	m_canCreateLeaf(true),
	m_treeIsMoving(true),
	m_canCreateMushroom(false),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(true),
	m_canCreateCloud(false),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),

	m_rockSize(sf::Vector2f(15.f, 100.f), sf::Vector2f(30.f, 400.f)),
	m_rockPartCount(50.f, 80.f),
	m_rockColor(240, 110, 110),

	m_treeDepth(6u, 8u),
	m_treeSize(sf::Vector2f(15.f, 100.f), sf::Vector2f(30.f, 200.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(53, 44, 45),
	m_treeAngle(15.f, 75.f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(100.f, 100.f)),
	m_leafColor(46, 133, 84),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(10), sf::seconds(30)),

	m_crystalSize(sf::Vector2f(10.f, 50.f), sf::Vector2f(25.f, 100.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(18, 14, 66, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(200.f, 100.f), sf::Vector2f(400.f, 200.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 200),

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
	m_rainbowIntervalTime(sf::seconds(1.f), sf::seconds(2.f)),

	m_indexTreePos(0u)
{
	m_generator.setSeed(m_seed);
#ifndef NDEBUG
	m_mapSeed = 42u;
#else
	m_mapSeed = m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	// TODO define map position and number of map
	//m_instances[12] = MINIMAP_OMP;
	//m_instances[86] = TEST_MAP2_OMP;

	// Define game objects
	m_gameObjects[40] = GameObjectType::Portal;
	m_gameObjects[740] = GameObjectType::Portal;
	m_gameObjects[5] = GameObjectType::JuNpc;
	m_gameObjects[600] = GameObjectType::RepairNanoRobot;
	m_instances[400] = TEST_MAP2_OMP;
	m_interestPointPosX = 500;

	m_treePos = {677, 682, 689, 697, 710, 711, 723, 760, 763, 785, 790, 794, 801};

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::LevelOne);
	m_destinations.push_back(Level::Default);
}

void			LevelThreeBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		LevelThreeBiome::getName()const
{
	return (m_name);
}

Level			LevelThreeBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	LevelThreeBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		LevelThreeBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	LevelThreeBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	LevelThreeBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			LevelThreeBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				LevelThreeBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	LevelThreeBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	LevelThreeBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

std::map<std::size_t, std::string> const & LevelThreeBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> LevelThreeBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -20, 0.1f, 1.f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 1.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	//layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -10, 0.1f, 0.9f, 11.f);
	//layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
	//	{
	//		return noise.perlin(x, y, 3, 2.f);
	//	});
	//vector.push_back(layer);
	//layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.6f, 0.2f), mapSize, 12.f, -10, 0.2f, 0.8f, 6.f);
	//layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
	//	{
	//		return noise.noise(x * 1.1f, y);
	//	});
	//vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator LevelThreeBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float start = 150.f / static_cast<float>(m_mapSize.x);
		float middle1 = 300.f / static_cast<float>(m_mapSize.x);
		float end = 450.f / static_cast<float>(m_mapSize.x);
		float offset = 50.f / static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		float bot = n / 1.5f + 0.6f;
		float top = n / 1.5f - 0.6f;

		if (x > start - offset && x <= start)
			return octo::cosinusInterpolation(n, bot, (x - start + offset) / offset);
		else if (x > start && x <= middle1 - offset)
			return bot;
		else if (x > middle1 - offset && x <= middle1)
			return octo::cosinusInterpolation(bot, top, (x - middle1 + offset) / offset);
		else if (x > middle1 && x <= end)
			return top;
		else if (x > end && x <= end + offset)
			return octo::cosinusInterpolation(n, top, (x - end + offset) / offset);
		else
			return n;
	};
}

Map::TileColorGenerator LevelThreeBiome::getTileColorGenerator()
{
	return [this](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		LevelThreeBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		LevelThreeBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		LevelThreeBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		LevelThreeBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		LevelThreeBiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		LevelThreeBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		LevelThreeBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		LevelThreeBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		LevelThreeBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			LevelThreeBiome::getWind()
{
	return (m_wind);
}

void			LevelThreeBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			LevelThreeBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		LevelThreeBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		LevelThreeBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		LevelThreeBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			LevelThreeBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			LevelThreeBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			LevelThreeBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		LevelThreeBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		LevelThreeBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		LevelThreeBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		LevelThreeBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		LevelThreeBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		LevelThreeBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		LevelThreeBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		LevelThreeBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		LevelThreeBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		LevelThreeBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	LevelThreeBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	LevelThreeBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		LevelThreeBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		LevelThreeBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			LevelThreeBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			LevelThreeBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

bool			LevelThreeBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			LevelThreeBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	LevelThreeBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		LevelThreeBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		LevelThreeBiome::getTreePositionX()
{
	return m_treePos[m_indexTreePos++];
}

sf::Vector2f	LevelThreeBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		LevelThreeBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		LevelThreeBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				LevelThreeBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			LevelThreeBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	LevelThreeBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		LevelThreeBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			LevelThreeBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			LevelThreeBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	LevelThreeBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		LevelThreeBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		LevelThreeBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

bool			LevelThreeBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	LevelThreeBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		LevelThreeBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		LevelThreeBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			LevelThreeBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	LevelThreeBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		LevelThreeBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		LevelThreeBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		LevelThreeBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			LevelThreeBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	LevelThreeBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		LevelThreeBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		LevelThreeBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			LevelThreeBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	LevelThreeBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		LevelThreeBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		LevelThreeBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			LevelThreeBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	LevelThreeBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		LevelThreeBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		LevelThreeBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			LevelThreeBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			LevelThreeBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			LevelThreeBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		LevelThreeBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		LevelThreeBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		LevelThreeBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			LevelThreeBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}


float			LevelThreeBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				LevelThreeBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			LevelThreeBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			LevelThreeBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				LevelThreeBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	LevelThreeBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		LevelThreeBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		LevelThreeBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
