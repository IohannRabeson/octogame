#include "DesertBBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

DesertBBiome::DesertBBiome() :
	m_name("Desert B"),
	m_id(Level::DesertB),
	m_seed("Cailloux"),
	m_mapSize(sf::Vector2u(650u, 64u)),
	m_mapSeed(42u),
	m_octoStartPosition(63.f * 16.f, -200.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(245, 222, 130),
	m_tileEndColor(245, 243, 249),
	m_waterLevel(300.f),
	m_waterColor(96, 204, 233, 0),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(100.f)),
	m_startDayDuration(sf::seconds(15.f)),
	m_skyDayColor(255, 150, 242),
	m_skyNightColor(166, 10, 92),
	m_nightLightColor(134, 63, 215, 130),
	m_SunsetLightColor(255, 59, 59, 130),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(10u, 20u),
	m_treeCount(13u, 13u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(130u, 170u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 3u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(20u, 40u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(false),
	m_canCreateRock(false),
	m_canCreateTree(true),
	m_canCreateLeaf(false),
	m_treeIsMoving(false),
	m_canCreateMushroom(false),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(true),
	m_canCreateCloud(false),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_waterPersistence(1.f),
	m_type(ABiome::Type::Desert),

	m_rockSize(sf::Vector2f(15.f, 100.f), sf::Vector2f(30.f, 400.f)),
	m_rockPartCount(50.f, 80.f),
	m_rockColor(255, 232, 170),

	m_treeDepth(6u, 8u),
	m_treeSize(sf::Vector2f(15.f, 30.f), sf::Vector2f(30.f, 60.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(18, 14, 66),
	m_treeAngle(15.f, 75.f),
	m_treeBeatMouvement(0.1f),
	m_leafSize(sf::Vector2f(80.f, 80.f), sf::Vector2f(150.f, 150.f)),
	m_leafColor(240, 110, 110),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(10), sf::seconds(30)),

	m_crystalSize(sf::Vector2f(10.f, 80.f), sf::Vector2f(25.f, 150.f)),
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

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::JungleA)
		m_octoStartPosition = sf::Vector2f(602.f * 16.f, -3150.f);

	m_gameObjects[60] = GameObjectType::PortalDesert;
	m_gameObjects[125] = GameObjectType::FabienNpc;
	m_gameObjects[208] = GameObjectType::CheckPoint;
	m_instances[230] = MAP_DESERT_B_TRAIL_A_OMP;
	m_instances[280] = MAP_DESERT_B_TRAIL_B_OMP;
	m_instances[312] = MAP_DESERT_B_TRAIL_C_OMP;
	m_instances[415] = MAP_DESERT_B_TRAIL_D_OMP;
	m_instances[525] = MAP_DESERT_B_TRAIL_E_OMP;

	m_interestPointPosX = 500;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::JungleA);
	m_destinations.push_back(Level::DesertC);
}

void			DesertBBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		DesertBBiome::getName()const
{
	return (m_name);
}

Level			DesertBBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	DesertBBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		DesertBBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	DesertBBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	DesertBBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			DesertBBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DesertBBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	DesertBBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	DesertBBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	DesertBBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	DesertBBiome::getWaterColor()
{
	return m_waterColor;
}

bool		DesertBBiome::isDeadlyWater()
{
	return true;
}

std::map<std::size_t, std::string> const & DesertBBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> DesertBBiome::getLayers()
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

Map::MapSurfaceGenerator DesertBBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		std::vector<float> pointX = { 0.f, 50.f, 55.f    , 90.f    , 95.f, 115.f, 120.f   , 135.f   , 140.f, 160.f, 165.f   , 175.f   , 185.f, 200.f, 205.f   , 210.f   , 215.f, 750.f};
		std::vector<float> pointY = { n  , n   , n - 0.8f, n - 0.8f, n   , n    , n - 1.0f, n - 1.0f, n    , n    , n - 1.2f, n - 1.2f, n    , n    , n - 1.4f, n - 1.4f, n    , n};
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

Map::TileColorGenerator DesertBBiome::getTileColorGenerator()
{
	sf::Color secondColorStart = m_skyDayColor;
	sf::Color secondColorEnd = m_skyNightColor;
	float startTransition = 800.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 1600.f / static_cast<float>(m_mapSize.y);
	float endTransition = 3000.f / static_cast<float>(m_mapSize.y);
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

sf::Color		DesertBBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		DesertBBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		DesertBBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		DesertBBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		DesertBBiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		DesertBBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		DesertBBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		DesertBBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		DesertBBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			DesertBBiome::getWind()
{
	return (m_wind);
}

void			DesertBBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			DesertBBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		DesertBBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		DesertBBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		DesertBBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			DesertBBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			DesertBBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			DesertBBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		DesertBBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		DesertBBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		DesertBBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		DesertBBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		DesertBBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		DesertBBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		DesertBBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		DesertBBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		DesertBBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		DesertBBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	DesertBBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	DesertBBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		DesertBBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		DesertBBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			DesertBBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			DesertBBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			DesertBBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			DesertBBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			DesertBBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	DesertBBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertBBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		DesertBBiome::getTreePositionX()
{
	return randomInt(10u, m_mapSize.x - 1u);
}

sf::Vector2f	DesertBBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		DesertBBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		DesertBBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				DesertBBiome::getCrystalPosX()
{
	int pos = randomInt(10u, m_mapSize.x - 1u);
	if ((pos >= 55 && pos <= 90) || (pos >= 120 && pos <= 135) || (pos >= 165 && pos <= 175) || (pos >= 200 && pos <= 205))
		return randomInt(10u, m_mapSize.x - 1u);
	return pos;
}

bool			DesertBBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	DesertBBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		DesertBBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			DesertBBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			DesertBBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	DesertBBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		DesertBBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		DesertBBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

bool			DesertBBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	DesertBBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		DesertBBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		DesertBBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			DesertBBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	DesertBBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		DesertBBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		DesertBBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		DesertBBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			DesertBBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	DesertBBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		DesertBBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		DesertBBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			DesertBBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	DesertBBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		DesertBBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		DesertBBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			DesertBBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	DesertBBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertBBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		DesertBBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			DesertBBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DesertBBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			DesertBBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		DesertBBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		DesertBBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		DesertBBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			DesertBBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

float	DesertBBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	DesertBBiome::getType() const
{
	return m_type;
}


float			DesertBBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				DesertBBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			DesertBBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			DesertBBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				DesertBBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	DesertBBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		DesertBBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		DesertBBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
