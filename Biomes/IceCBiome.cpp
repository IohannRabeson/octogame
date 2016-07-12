#include "IceCBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

IceCBiome::IceCBiome() :
	m_name("Ice C"),
	m_id(Level::IceC),
	m_seed("Level_One"),
	m_mapSize(sf::Vector2u(540u, 256u)),
	m_mapSeed(42u),
	m_octoStartPosition(120.f * 16.f, -3750.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(85, 150, 179),
	m_tileEndColor(27, 79, 101),
	m_waterLevel(-1.f),
	m_waterColor(255, 255, 255, 200),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(60.f)),
	m_startDayDuration(sf::seconds(9.f)),
	m_skyDayColor(255, 154, 0),
	m_skyNightColor(255, 154, 0),
	m_nightLightColor(8, 20, 26, 50),
	m_sunsetLightColor(161, 11, 11, 50),
	m_wind(300.f),
	m_rainDropPerSecond(1u, 2u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(7u, 8u),
	m_treeCount(10u, 10u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(4u, 8u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 2u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(200u, 250u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(true),
	m_canCreateRock(true),
	m_canCreateTree(true),
	m_canCreateLeaf(false),
	m_treeIsMoving(false),
	m_canCreateMushroom(false),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(false),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_canCreateGrass(false),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Ice),

	m_rockSize(sf::Vector2f(50.f, 100.f), sf::Vector2f(100.f, 200.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(0, 31, 63),

	m_grassSizeY(30.f, 60.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),
	m_grassIndex(0u),

	m_treeDepth(5u, 7u),
	m_treeSize(sf::Vector2f(20.f, 150.f), sf::Vector2f(50.f, 250.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(158, 176, 183),
	m_treeAngle(15.f, 75.f),
	m_treeBeatMouvement(0.f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(100.f, 100.f)),
	m_leafColor(143, 208, 202),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(20), sf::seconds(60)),

	m_crystalSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(30.f, 100.f)),
	m_crystalPartCount(2u, 3u),
	m_crystalColor(227, 227, 227, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(400.f, 400.f), sf::Vector2f(1000.f, 1000.f)),
	m_cloudPartCount(1u, 1u),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 200),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(60.f, 60.f), sf::Vector2f(60.f, 60.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 254),

	m_moonSize(sf::Vector2f(70.f, 50.f), sf::Vector2f(150.f, 150.f)),
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

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	m_instances[10] = MAP_ICE_C_TRAIL_A_OMP;
	m_instances[180] = MAP_ICE_C_TRAIL_B_OMP;
	m_interestPointPosX = 530;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::IceD)
		m_octoStartPosition = sf::Vector2f(185.f * 16.f, -850.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(239.f * 16.f, -1350.f);

	m_destinations.push_back(Level::IceB);
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::IceD);
}

void			IceCBiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			IceCBiome::getId()const
{
	return m_id;
}

std::string		IceCBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	IceCBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		IceCBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	IceCBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	IceCBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			IceCBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				IceCBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	IceCBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	IceCBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	IceCBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	IceCBiome::getWaterColor()
{
	return m_waterColor;
}

std::map<std::size_t, std::string> const & IceCBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> IceCBiome::getLayers()
{
	//sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	/*
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
	*/
	return vector;
}

Map::MapSurfaceGenerator IceCBiome::getMapSurfaceGenerator()
{
	return [](Noise & noise, float x, float y)
	{
		return noise.fBm(x, y, 3, 3.f, 0.3f);
	};
}

Map::TileColorGenerator IceCBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(m_tileStartColor + sf::Color(100, 100, 100));
	sf::Color secondColorEnd(m_tileEndColor + sf::Color(100, 100, 100));
	float startTransition = -48;
	float middleTransition = -28;
	float endTransition = -8;
	return [this, secondColorStart, secondColorEnd, startTransition, endTransition, middleTransition](Noise & noise, float x, float y, float z)
	{
		if (y > -3000 && y < -48)
			return m_cloudColor;
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		if (y > startTransition && y <= middleTransition)
		{
			float ratio = (y - (startTransition)) / (middleTransition - startTransition);
			return octo::linearInterpolation(octo::linearInterpolation(secondColorStart, m_tileStartColor, ratio), secondColorEnd, transition);
		}
		else if (y > middleTransition && y <= endTransition)
		{
			float ratio = (y - (middleTransition)) / (endTransition - middleTransition);
			return octo::linearInterpolation(m_tileStartColor, octo::linearInterpolation(secondColorEnd, m_tileEndColor, ratio), transition);
		}
		if (y > endTransition)
			return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		IceCBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		IceCBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		IceCBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		IceCBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		IceCBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		IceCBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		IceCBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		IceCBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		IceCBiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			IceCBiome::getWind()
{
	return (m_wind);
}

void			IceCBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			IceCBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		IceCBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		IceCBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		IceCBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			IceCBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			IceCBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			IceCBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		IceCBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		IceCBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		IceCBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		IceCBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		IceCBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		IceCBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		IceCBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		IceCBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		IceCBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		IceCBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	IceCBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	IceCBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		IceCBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		IceCBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			IceCBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			IceCBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			IceCBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			IceCBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			IceCBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	IceCBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceCBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		IceCBiome::getTreePositionX()
{
	return (randomInt(1u, m_mapSize.x - 1u));
}

sf::Vector2f	IceCBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		IceCBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		IceCBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				IceCBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			IceCBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	IceCBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		IceCBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			IceCBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			IceCBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	IceCBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		IceCBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		IceCBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	IceCBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

sf::Color	IceCBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	IceCBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	IceCBiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			IceCBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	IceCBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		IceCBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		IceCBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			IceCBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	IceCBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		IceCBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		IceCBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		IceCBiome::getCloudColor()
{
	return (m_cloudColor);
}

bool			IceCBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	IceCBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		IceCBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		IceCBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			IceCBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	IceCBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		IceCBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		IceCBiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			IceCBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	IceCBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceCBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		IceCBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			IceCBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			IceCBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			IceCBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		IceCBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		IceCBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		IceCBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			IceCBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	IceCBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	IceCBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	IceCBiome::getType() const
{
	return m_type;
}


float			IceCBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				IceCBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			IceCBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			IceCBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				IceCBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	IceCBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		IceCBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		IceCBiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
