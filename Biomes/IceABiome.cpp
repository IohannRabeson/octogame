#include "IceABiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

IceABiome::IceABiome() :
	m_name("Ice A"),
	m_id(Level::IceA),
	m_seed("Level_One"),
	m_mapSize(sf::Vector2u(610u, 16u)),
	m_mapSeed(42u),
	m_octoStartPosition(136.f * 16.f, 60.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(227, 227, 227),
	m_tileEndColor(137, 189, 211),
	m_waterLevel(-1.f),
	m_waterColor(255, 255, 255, 200),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(35.f)),
	m_startDayDuration(sf::seconds(20.f)),
	m_skyDayColor(8, 20, 26),
	m_skyNightColor(78, 47, 4, 130),
	m_nightLightColor(8, 20, 26, 50),
	m_sunsetLightColor(61, 0, 13, 20),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(7u, 8u),
	m_treeCount(19u, 19u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(4u, 8u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
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
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_canCreateGrass(false),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Ice),

	m_rockSize(sf::Vector2f(5.f, 50.f), sf::Vector2f(20.f, 70.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(0, 31, 63),

	m_grassSizeY(30.f, 60.f),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),
	m_grassIndex(0u),

	m_treeDepth(5u, 5u),
	m_treeSize(sf::Vector2f(15.f, 60.f), sf::Vector2f(30.f, 150.f)),
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
	m_cloudMaxY(-500.f),
	m_cloudMinY(-2500.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 200),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(7.f, 7.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(208, 220, 226),

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
	m_mapSeed = 42u;

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	m_gameObjects[150] = GameObjectType::GroundTransformNanoRobot;
	m_interestPointPosX = 420;
	m_gameObjects[128] = GameObjectType::SpaceShip;
	m_instances[70] = MAP_ICE_A_TRAIL_LEFT_OMP;
	m_instances[120] = MAP_ICE_A_CRATER_OMP;
	m_instances[170] = MAP_ICE_A_TRAIL_RIGHT_OMP;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::IceB)
		m_octoStartPosition = sf::Vector2f(423 * 16.f, 0.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(404.f * 16.f, -1250.f);

	m_instances[370] = MAP_ICE_A_SECRET_OMP;
	m_gameObjects[420] = GameObjectType::PortalSnow;
	m_instances[470] = MAP_ICE_A_SECRET_OMP;
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::IceB);

	m_gameObjects[344] = GameObjectType::BirdBlueNpc;
	m_gameObjects[200] = GameObjectType::JuNpc;
	m_gameObjects[400] = GameObjectType::FranfranNpc;
	m_gameObjects[490] = GameObjectType::BirdBlueNpc;

	m_treePos = {156, 300, 306, 309, 320, 329, 340, 354, 359, 375, 450, 459, 463, 469, 485, 501, 510, 523, 550};
}

void			IceABiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			IceABiome::getId()const
{
	return m_id;
}

std::string		IceABiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	IceABiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		IceABiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	IceABiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	IceABiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			IceABiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				IceABiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	IceABiome::getGameObjects()
{
	return m_gameObjects;
}

Level	IceABiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	IceABiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	IceABiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	IceABiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & IceABiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> IceABiome::getLayers()
{
	sf::Vector2u const & mapSize = sf::Vector2u(getMapSize().x, getMapSize().y * 4u);
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround() - sf::Color(130, 130, 130, 0), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, 10, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround() - sf::Color(130, 130, 130, 0), sf::Vector2f(0.5f, 0.3f), mapSize, 15.f, 15, 0.2f, 0.8f, 6.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator IceABiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		std::vector<float> pointX = {50.f, 70.f, 120.f, 125.f, 165.f, 170.f, 220.f, 240.f, 350.f, 369.f, 377.f, 396.f, 450.f, 469.f, 477.f, 496.f};
		std::vector<float> pointY = {n   , 0.f , 0.f  , 2.4f , 2.4f , 0.f  , 0.f  , n    , n    , 0.1f , 0.1f , n    , n    , 0.1f , 0.1f , n};
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

Map::TileColorGenerator IceABiome::getTileColorGenerator()
{
	sf::Color secondColorStart(getRockColor());
	sf::Color secondColorEnd(getRockColor());
	float startTransition = 300.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 600.f / static_cast<float>(m_mapSize.y);
	float endTransition = 900.f / static_cast<float>(m_mapSize.y);
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

sf::Color		IceABiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		IceABiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		IceABiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		IceABiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		IceABiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		IceABiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		IceABiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		IceABiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		IceABiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			IceABiome::getWind()
{
	return (m_wind);
}

void			IceABiome::setWind(float wind)
{
	m_wind = wind;
}

bool			IceABiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		IceABiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		IceABiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		IceABiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			IceABiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			IceABiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			IceABiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		IceABiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		IceABiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		IceABiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		IceABiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		IceABiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		IceABiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		IceABiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		IceABiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		IceABiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		IceABiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	IceABiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	IceABiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		IceABiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		IceABiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			IceABiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			IceABiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			IceABiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			IceABiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			IceABiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	IceABiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceABiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		IceABiome::getTreePositionX()
{
	return (m_treePos[m_indexTreePos++]);
}

sf::Vector2f	IceABiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		IceABiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		IceABiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				IceABiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			IceABiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	IceABiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		IceABiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			IceABiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			IceABiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	IceABiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		IceABiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		IceABiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	IceABiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	IceABiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	IceABiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	IceABiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	IceABiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			IceABiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	IceABiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		IceABiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		IceABiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			IceABiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	IceABiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		IceABiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	IceABiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	IceABiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	IceABiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		IceABiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		IceABiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			IceABiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	IceABiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		IceABiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		IceABiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			IceABiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	IceABiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		IceABiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		IceABiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			IceABiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	IceABiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceABiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		IceABiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			IceABiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			IceABiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			IceABiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		IceABiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		IceABiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		IceABiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			IceABiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	IceABiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	IceABiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	IceABiome::getType() const
{
	return m_type;
}


float			IceABiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				IceABiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			IceABiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			IceABiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				IceABiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	IceABiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		IceABiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		IceABiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
