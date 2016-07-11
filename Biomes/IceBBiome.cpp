#include "IceBBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

IceBBiome::IceBBiome() :
	m_name("Ice B"),
	m_id(Level::IceB),
	m_seed("Ice B yo"),
	m_mapSize(sf::Vector2u(550u, 256u)),
	m_mapSeed(42u),
	m_octoStartPosition(453.f * 16.f, 2100.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(227, 227, 227),
	m_tileEndColor(87, 139, 161),
	m_waterLevel(-1.f),
	m_waterColor(255, 255, 255, 200),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(45.f)),
	m_startDayDuration(sf::seconds(9.f)),
	m_skyDayColor(8, 20, 26),
	m_skyNightColor(130, 79, 8, 100),
	m_nightLightColor(8, 20, 26, 90),
	m_sunsetLightColor(136 , 0, 30, 20),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(17u, 28u),
	m_treeCount(6u, 7u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(4u, 8u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 3u),
	m_moonCount(2u, 2u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(20u, 40u),
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
	m_canCreateSun(false),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_canCreateGrass(false),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Ice),

	m_rockSize(sf::Vector2f(10.f, 140.f), sf::Vector2f(30.f, 200.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(0, 31, 63),

	m_grassSizeY(30.f, 60.f),
	m_grassColor(m_tileStartColor),
	m_grassIndex(0u),

	m_treeDepth(5u, 5u),
	m_treeSize(sf::Vector2f(100.f, 50.f), sf::Vector2f(200.f, 100.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(188, 206, 213),
	m_treeAngle(15.f, 75.f),
	m_treeBeatMouvement(0.f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(100.f, 100.f)),
	m_leafColor(143, 208, 202),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(20), sf::seconds(60)),

	m_crystalSize(sf::Vector2f(5.f, 25.f), sf::Vector2f(10.f, 50.f)),
	m_crystalPartCount(2u, 3u),
	m_crystalColor(227, 227, 227, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(200.f, 100.f), sf::Vector2f(400.f, 200.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(60.f, 60.f), sf::Vector2f(150.f, 150.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 200),

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

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);


	m_gameObjects[40] = GameObjectType::JuGlitchNpc;
	m_instances[50] = MAP_ICE_B_TRAIL_A_OMP;
	m_instances[108] = MAP_ICE_B_TRAIL_B_OMP;
	m_instances[178] = MAP_ICE_B_TRAIL_C_OMP;
	m_instances[219] = MAP_ICE_B_TRAIL_D_OMP;
	m_instances[370] = MAP_ICE_B_PORTAL_OMP;
	m_gameObjects[385] = GameObjectType::WindowGlitchNpc;
	m_gameObjects[390] = GameObjectType::EngineSnow;
	m_gameObjects[450] = GameObjectType::PortalSnow;
	m_interestPointPosX = 530;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::IceC)
		m_octoStartPosition = sf::Vector2f(279.f * 16.f, 1640.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(367.f * 16.f, -30.f);

	m_gameObjects[400] = GameObjectType::BirdBlueNpc;
	m_gameObjects[320] = GameObjectType::BirdBlueNpc;

	m_destinations.push_back(Level::IceC);
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::IceA);
}

void			IceBBiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			IceBBiome::getId()const
{
	return m_id;
}

std::string		IceBBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	IceBBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		IceBBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	IceBBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	IceBBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			IceBBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				IceBBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	IceBBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	IceBBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	IceBBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	IceBBiome::getWaterColor()
{
	return m_waterColor;
}

std::map<std::size_t, std::string> const & IceBBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> IceBBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(randomColor(sf::Color(0, 5, 10)), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -35, 0.1f, 1.f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 2.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(randomColor(sf::Color(0, 5, 10)), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -30, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator IceBBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		float m = noise.fBm(x, y, 3, 3.f, 0.3f) / 4.f;
		std::vector<float> pointX = {25.f, 50.f, 51.f, 317.f, 318.f, 325.f, 370.f    , 410.f    , 500.f};
		std::vector<float> pointY = {n   , 0.f , 0.2f, 0.2f , 0.f  , n    , m + 0.04f, m + 0.04f, n};
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

Map::TileColorGenerator IceBBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(getRockColor());
	sf::Color secondColorEnd(getRockColor());
	float startTransition = 28000.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 40500.f / static_cast<float>(m_mapSize.y);
	float endTransition = 50000.f / static_cast<float>(m_mapSize.y);
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

sf::Color		IceBBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		IceBBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		IceBBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		IceBBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		IceBBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		IceBBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		IceBBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		IceBBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		IceBBiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			IceBBiome::getWind()
{
	return (m_wind);
}

void			IceBBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			IceBBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		IceBBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		IceBBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		IceBBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			IceBBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			IceBBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			IceBBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		IceBBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		IceBBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		IceBBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		IceBBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		IceBBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		IceBBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		IceBBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		IceBBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		IceBBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		IceBBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	IceBBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	IceBBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		IceBBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		IceBBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			IceBBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			IceBBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			IceBBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			IceBBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			IceBBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	IceBBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceBBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		IceBBiome::getTreePositionX()
{
	return (randomInt(1u, m_mapSize.x - 1u));
}

sf::Vector2f	IceBBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		IceBBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		IceBBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				IceBBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			IceBBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	IceBBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		IceBBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			IceBBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			IceBBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	IceBBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		IceBBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		IceBBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	IceBBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

sf::Color	IceBBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	IceBBiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			IceBBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	IceBBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		IceBBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		IceBBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			IceBBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	IceBBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		IceBBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		IceBBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		IceBBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			IceBBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	IceBBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		IceBBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		IceBBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			IceBBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	IceBBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		IceBBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		IceBBiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			IceBBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	IceBBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceBBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		IceBBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			IceBBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			IceBBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			IceBBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		IceBBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		IceBBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		IceBBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			IceBBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	IceBBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	IceBBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	IceBBiome::getType() const
{
	return m_type;
}


float			IceBBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				IceBBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			IceBBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			IceBBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				IceBBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	IceBBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		IceBBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		IceBBiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
