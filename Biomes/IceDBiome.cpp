#include "IceDBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

IceDBiome::IceDBiome() :
	m_name("Ice D"),
	m_id(Level::IceD),
	m_seed("Level_One"),
	m_mapSize(sf::Vector2u(750u, 36u)),
	m_mapSeed(42u),
	m_octoStartPosition(16.f * 33.f, 80.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(227, 227, 227),
	m_tileEndColor(27, 79, 101),
	m_waterLevel(1000.f),
	m_waterColor(255, 255, 255, 200),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(65.f)),
	m_startDayDuration(sf::seconds(9.f)),
	m_skyDayColor(8, 20, 26),
	m_skyNightColor(8, 20, 26),
	m_nightLightColor(8, 20, 26, 80),
	m_sunsetLightColor(sf::Color::Transparent),
	m_wind(200.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(7u, 8u),
	m_treeCount(10u, 10u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(10u, 20u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 3u),
	m_moonCount(2u, 2u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(40u, 60u),
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
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Ice),

	m_rockSize(sf::Vector2f(30.f, 100.f), sf::Vector2f(70.f, 200.f)),
	m_rockPartCount(3.f, 10.f),
	m_rockColor(0, 31, 63),

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

	m_crystalSize(sf::Vector2f(20.f, 100.f), sf::Vector2f(50.f, 250.f)),
	m_crystalPartCount(3u, 6u),
	m_crystalColor(227, 227, 227, 150),
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

	m_sunSize(sf::Vector2f(60.f, 60.f), sf::Vector2f(150.f, 150.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 200),

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

	m_interestPointPosX = 530;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::DesertA)
		m_octoStartPosition = sf::Vector2f(257.f * 16.f, -93.f * 16.f);
	else if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(124.f * 16.f, -113.f * 16.f);

	m_gameObjects[30] = GameObjectType::PortalSnow;
	m_gameObjects[100] = GameObjectType::HouseFlatSnow;
	m_instances[139] = MAP_ICE_D_SECRET_WAY_OMP;
	m_instances[118] = MAP_ICE_D_PORTAL_RANDOM_OMP;
	m_gameObjects[190] = GameObjectType::BirdBlueNpc;
	m_instances[220] = MAP_ICE_D_ELEVATOR_OMP;
	m_gameObjects[250] = GameObjectType::EngineSnow;
	m_gameObjects[360] = GameObjectType::StrangerSnowNpc;
	m_gameObjects[340] = GameObjectType::BirdBlueNpc;
	m_gameObjects[396] = GameObjectType::Pedestal;
	m_gameObjects[400] = GameObjectType::RepairNanoRobot;
	m_gameObjects[450] = GameObjectType::WeirdHouseSnow;
	m_gameObjects[510] = GameObjectType::BirdBlueNpc;
	m_instances[551] = MAP_ICE_D_TRAIL_OMP;
	m_gameObjects[630] = GameObjectType::StrangerGirlSnowNpc;
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::DesertA);
	m_destinations.push_back(Level::IceC);
}

void			IceDBiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			IceDBiome::getId()const
{
	return m_id;
}

std::string		IceDBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	IceDBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		IceDBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	IceDBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	IceDBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			IceDBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				IceDBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	IceDBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	IceDBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	IceDBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	IceDBiome::getWaterColor()
{
	return m_waterColor;
}

bool		IceDBiome::isDeadlyWater()
{
	return true;
}

std::map<std::size_t, std::string> const & IceDBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> IceDBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -10, 0.1f, 0.9f, 11.f);
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

Map::MapSurfaceGenerator IceDBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		std::vector<float> pointX = {/*house*/50.f, 100.f, 127.f, 132.f, 138.f, 160.f, 166.f, 171.f, 220.f, 260.f,/*lake*/500.f, 550.f, 551.f, 620.f, 621.f, 740.f};
		std::vector<float> pointY = {/*house*/n   , -1.f , -1.f , 5.f  , -1.f , -1.f , 5.f  , -1.f , -1.f , n    ,/*lake*/n    , 0.8f  , 4.f , 4.f  , 2.f  , n    };
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

Map::TileColorGenerator IceDBiome::getTileColorGenerator()
{
	sf::Color secondColorStart(getRockColor());
	sf::Color secondColorEnd(getRockColor());
	float startTransition = 1000.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 2500.f / static_cast<float>(m_mapSize.y);
	float endTransition = 4500.f / static_cast<float>(m_mapSize.y);
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

sf::Color		IceDBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		IceDBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		IceDBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		IceDBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		IceDBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		IceDBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		IceDBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		IceDBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		IceDBiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			IceDBiome::getWind()
{
	return (m_wind);
}

void			IceDBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			IceDBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		IceDBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		IceDBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		IceDBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			IceDBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			IceDBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			IceDBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		IceDBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		IceDBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		IceDBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		IceDBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		IceDBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		IceDBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		IceDBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		IceDBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		IceDBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		IceDBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	IceDBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	IceDBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		IceDBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		IceDBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			IceDBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			IceDBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			IceDBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			IceDBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			IceDBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	IceDBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceDBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		IceDBiome::getTreePositionX()
{
	return (randomInt(1u, m_mapSize.x - 1u));
}

sf::Vector2f	IceDBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		IceDBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		IceDBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				IceDBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			IceDBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	IceDBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		IceDBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			IceDBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			IceDBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	IceDBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		IceDBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		IceDBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

bool			IceDBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	IceDBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		IceDBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		IceDBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			IceDBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	IceDBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		IceDBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		IceDBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		IceDBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			IceDBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	IceDBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		IceDBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		IceDBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			IceDBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	IceDBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		IceDBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		IceDBiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			IceDBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	IceDBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		IceDBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		IceDBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			IceDBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			IceDBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			IceDBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		IceDBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		IceDBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		IceDBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			IceDBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

float	IceDBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	IceDBiome::getType() const
{
	return m_type;
}


float			IceDBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				IceDBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			IceDBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			IceDBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				IceDBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	IceDBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		IceDBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		IceDBiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
