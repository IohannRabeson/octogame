#include "JungleABiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

JungleABiome::JungleABiome() :
	m_name("Jungle A"),
	m_id(Level::JungleA),
	m_seed("sdf"),
	m_mapSize(sf::Vector2u(1100u, 128u)),
	m_mapSeed(42u),
	m_octoStartPosition(43.f * 16.f, 650.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(0, 76, 54),
	m_tileEndColor(0, 124, 104),
	m_waterLevel(1500.f),
	m_waterColor(0, 189, 168, 150),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(80.f)),
	m_startDayDuration(sf::seconds(15.f)),
	m_skyDayColor(252, 252, 190),
	m_skyNightColor(0, 0, 0),
	m_nightLightColor(0, 0, 0, 130),
	m_SunsetLightColor(252, 252, 190, 130),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(10u, 20u),
	m_treeCount(100u, 101u),
	m_mushroomCount(39u, 40u),
	m_crystalCount(10u, 15u),
	m_starCount(500u, 800u),
	m_sunCount(4u, 5u),
	m_moonCount(2u, 3u),
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

	m_rockSize(sf::Vector2f(15.f, 60.f), sf::Vector2f(30.f, 100.f)),
	m_rockPartCount(6.f, 15.f),
	m_rockColor(56, 50, 72),

	m_treeDepth(4u, 5u),
	m_treeSize(sf::Vector2f(30.f, 300.f), sf::Vector2f(200.f, 300.f)),
	m_treeLifeTime(sf::seconds(90), sf::seconds(180)),
	m_treeColor(40, 37, 44),
	m_treeAngle(5.f, 15.f),
	m_treeBeatMouvement(0.05f),
	m_leafSize(sf::Vector2f(20.f, 20.f), sf::Vector2f(250.f, 250.f)),
	m_leafColor(0, 90, 67),

	m_mushroomSize(sf::Vector2f(20.f, 300.f), sf::Vector2f(300.f, 500.f)),
	m_mushroomColor(255, 182, 0),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(40.f, 100.f), sf::Vector2f(80.f, 200.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(134, 160, 191, 150),
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
	m_mapSeed = 42u;//m_generator.randomInt(0, std::numeric_limits<int>::max());
#endif

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	// Define game objects
	m_instances[1070] = MAP_JUNGLE_A_ELEVATOR_OMP;
	m_instances[1019] = MAP_JUNGLE_A_LUCIEN_OMP;
	m_instances[765] = MAP_JUNGLE_A_VILLAGE_OMP;
	m_instances[670] = MAP_JUNGLE_A_SECRET_LEFT_VILLAGE_OMP;
	m_instances[2] = MAP_JUNGLE_A_CEDRIC_OMP; //2 to 102
	m_instances[120] = MAP_JUNGLE_A_TRAIL_OMP; //150 to 450
	m_instances[500] = MAP_JUNGLE_A_DOUBLE_JUMP_OMP; //500 to 600
	m_gameObjects[40] = GameObjectType::Portal;
	m_gameObjects[500] = GameObjectType::Portal;
	m_gameObjects[300] = GameObjectType::VinceNpc;
	m_gameObjects[745] = GameObjectType::AmandineNpc;
	m_gameObjects[769] = GameObjectType::FaustNpc;
	m_gameObjects[780] = GameObjectType::ConstanceNpc;
	m_gameObjects[80] = GameObjectType::PierreNpc;
	m_gameObjects[470] = GameObjectType::CanouilleNpc;

	m_gameObjects[170] = GameObjectType::BirdNpc;
	m_gameObjects[180] = GameObjectType::BirdNpc;
	m_gameObjects[240] = GameObjectType::BirdNpc;
	m_gameObjects[270] = GameObjectType::BirdNpc;
	for (std::size_t i = 285; i < 291; i++)
		m_gameObjects[i] = GameObjectType::BirdNpc;
	m_gameObjects[700] = GameObjectType::BirdNpc;
	for (std::size_t i = 830; i < 837; i++)
		m_gameObjects[i] = GameObjectType::BirdNpc;
	m_gameObjects[870] = GameObjectType::BirdNpc;
	m_gameObjects[900] = GameObjectType::BirdNpc;
	m_gameObjects[940] = GameObjectType::BirdNpc;
	m_gameObjects[1000] = GameObjectType::BirdNpc;
	for (std::size_t i = 1000; i < 1005; i++)
		m_gameObjects[i] = GameObjectType::BirdNpc;

	m_interestPointPosX = 500;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Default);
	m_destinations.push_back(Level::DesertA);
	m_destinations.push_back(Level::WaterA);

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::WaterA)
		m_octoStartPosition = sf::Vector2f(490.f * 16.f, 2400.f);
}

void			JungleABiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		JungleABiome::getName()const
{
	return (m_name);
}

Level			JungleABiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	JungleABiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		JungleABiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	JungleABiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	JungleABiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			JungleABiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				JungleABiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	JungleABiome::getGameObjects()
{
	return m_gameObjects;
}

Level	JungleABiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	JungleABiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	JungleABiome::getWaterColor()
{
	return m_waterColor;
}

std::map<std::size_t, std::string> const & JungleABiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> JungleABiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -20, 0.1f, 1.f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x * 1.f, y, 2, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -10, 0.1f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
		return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	//layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.6f, 0.2f), mapSize, 12.f, -10, 0.2f, 0.8f, 6.f);
	//layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
	//	{
	//		return noise.noise(x * 1.1f, y);
	//	});
	//vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator JungleABiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float start = 400.f / static_cast<float>(m_mapSize.x);
		float middle1 = 700.f / static_cast<float>(m_mapSize.x);
		float end = 800.f / static_cast<float>(m_mapSize.x);
		float offset = 130.f / static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		float bot = n / 1.5f + 1.6f;
		float top = n / 1.5f - 1.6f;

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

Map::TileColorGenerator JungleABiome::getTileColorGenerator()
{
	sf::Color secondColorStart(76, 70, 102);
	sf::Color secondColorEnd(56, 50, 72);
	float startTransition = 9500.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 12000.f / static_cast<float>(m_mapSize.y);
	float endTransition = 14000.f / static_cast<float>(m_mapSize.y);
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

sf::Color		JungleABiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		JungleABiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		JungleABiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		JungleABiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		JungleABiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		JungleABiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		JungleABiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		JungleABiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		JungleABiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			JungleABiome::getWind()
{
	return (m_wind);
}

void			JungleABiome::setWind(float wind)
{
	m_wind = wind;
}

bool			JungleABiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		JungleABiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		JungleABiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		JungleABiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			JungleABiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			JungleABiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			JungleABiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		JungleABiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		JungleABiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		JungleABiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		JungleABiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		JungleABiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		JungleABiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		JungleABiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		JungleABiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		JungleABiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		JungleABiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	JungleABiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	JungleABiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		JungleABiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		JungleABiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			JungleABiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			JungleABiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			JungleABiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			JungleABiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			JungleABiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	JungleABiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleABiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		JungleABiome::getTreePositionX()
{
	return randomInt(1u, m_mapSize.x - 1u);
}

sf::Vector2f	JungleABiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		JungleABiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		JungleABiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				JungleABiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			JungleABiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	JungleABiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		JungleABiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			JungleABiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			JungleABiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	JungleABiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		JungleABiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		JungleABiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

bool			JungleABiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	JungleABiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		JungleABiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		JungleABiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			JungleABiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	JungleABiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		JungleABiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		JungleABiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		JungleABiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			JungleABiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	JungleABiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		JungleABiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		JungleABiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			JungleABiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	JungleABiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		JungleABiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		JungleABiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			JungleABiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	JungleABiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		JungleABiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		JungleABiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			JungleABiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			JungleABiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			JungleABiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		JungleABiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		JungleABiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		JungleABiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			JungleABiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}


float			JungleABiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				JungleABiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			JungleABiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			JungleABiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				JungleABiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	JungleABiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		JungleABiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		JungleABiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
