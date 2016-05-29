#include "DesertCBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

DesertCBiome::DesertCBiome() :
	m_name("Desert B"),
	m_id(Level::DesertC),
	m_seed("Cailloux"),
	m_mapSize(sf::Vector2u(500u, 180u)),
	m_mapSeed(42u),
	m_octoStartPosition(250.f * 16.f, -1800.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(255, 245, 217),
	m_tileEndColor(255, 252, 181),
	m_waterLevel(3000.f),
	m_waterColor(240, 110, 110, 180),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(100.f)),
	m_startDayDuration(sf::seconds(15.f)),
	m_skyDayColor(255,156,103),
	m_skyNightColor(8, 20, 26),
	m_nightLightColor(0, 197, 255, 130),
	m_SunsetLightColor(238, 173, 181, 130),
	m_wind(50.f),
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
	m_cloudCount(40u, 60u),
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
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_type(ABiome::Type::Desert),

	m_rockSize(sf::Vector2f(15.f, 100.f), sf::Vector2f(30.f, 400.f)),
	m_rockPartCount(50.f, 80.f),
	m_rockColor(240, 110, 110),

	m_treeDepth(6u, 8u),
	m_treeSize(sf::Vector2f(15.f, 50.f), sf::Vector2f(30.f, 80.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(53, 44, 45),
	m_treeAngle(15.f, 75.f),
	m_treeBeatMouvement(0.1f),
	m_leafSize(sf::Vector2f(80.f, 80.f), sf::Vector2f(150.f, 150.f)),
	m_leafColor(46, 133, 84),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(10), sf::seconds(30)),

	m_crystalSize(sf::Vector2f(40.f, 80.f), sf::Vector2f(70.f, 150.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(18, 14, 66, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(400.f, 200.f), sf::Vector2f(1200.f, 600.f)),
	m_cloudPartCount(6u, 15u),
	m_cloudLifeTime(sf::seconds(2), sf::seconds(20)),
	m_cloudColor(255, 255, 255, 80),

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
		m_octoStartPosition = sf::Vector2f(9800, -4950.f);

	m_gameObjects[20] = GameObjectType::Portal;
//	m_instances[70] = MAP_DESERT_B_BRIDGE_OMP;
//	m_gameObjects[90] = GameObjectType::Portal;
//	m_instances[110] = MAP_DESERT_B_CAVE_OMP;
//	m_instances[240] = MAP_DESERT_B_TRAIL_A_OMP;
//	m_instances[290] = MAP_DESERT_B_TRAIL_B_OMP;
//	m_instances[415] = MAP_DESERT_B_TRAIL_C_OMP;
//	m_instances[535] = MAP_DESERT_B_TRAIL_D_OMP;

	m_interestPointPosX = 500;

	m_treePos = {28, 35, 42, 50, 56, 61, 139, 147, 152, 167, 181, 194, 214};

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::DesertC);
	m_destinations.push_back(Level::DesertB);
	m_destinations.push_back(Level::JungleA);
	m_destinations.push_back(Level::Default);
}

void			DesertCBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		DesertCBiome::getName()const
{
	return (m_name);
}

Level			DesertCBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	DesertCBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		DesertCBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	DesertCBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	DesertCBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			DesertCBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DesertCBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	DesertCBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	DesertCBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	DesertCBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	DesertCBiome::getWaterColor()
{
	return m_waterColor;
}

std::map<std::size_t, std::string> const & DesertCBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> DesertCBiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(octo::linearInterpolation(m_particleColor[0u], m_particleColor[1u], 0.5f), sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -20, 0.1f, 0.7f, -1.f);
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

Map::MapSurfaceGenerator DesertCBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		std::vector<float> pointX = {0.f    , 130.f   , 150.f   , 350.f   , 330.f  };
		std::vector<float> pointY = {2.f + n, 2.f + n , -2.f - n, -2.f - n, 2.f + n};
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

Map::TileColorGenerator DesertCBiome::getTileColorGenerator()
{
	return [this](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		DesertCBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		DesertCBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		DesertCBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		DesertCBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		DesertCBiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		DesertCBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		DesertCBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		DesertCBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		DesertCBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			DesertCBiome::getWind()
{
	return (m_wind);
}

void			DesertCBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			DesertCBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		DesertCBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		DesertCBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		DesertCBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			DesertCBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			DesertCBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			DesertCBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		DesertCBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		DesertCBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		DesertCBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		DesertCBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		DesertCBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		DesertCBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		DesertCBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		DesertCBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		DesertCBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		DesertCBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	DesertCBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	DesertCBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		DesertCBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		DesertCBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			DesertCBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			DesertCBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			DesertCBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			DesertCBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			DesertCBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	DesertCBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertCBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		DesertCBiome::getTreePositionX()
{
	return m_treePos[m_indexTreePos++];
}

sf::Vector2f	DesertCBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		DesertCBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		DesertCBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				DesertCBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			DesertCBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	DesertCBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		DesertCBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			DesertCBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			DesertCBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	DesertCBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		DesertCBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		DesertCBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

bool			DesertCBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	DesertCBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		DesertCBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		DesertCBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			DesertCBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	DesertCBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		DesertCBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		DesertCBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		DesertCBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			DesertCBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	DesertCBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		DesertCBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		DesertCBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			DesertCBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	DesertCBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		DesertCBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		DesertCBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			DesertCBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	DesertCBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertCBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		DesertCBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			DesertCBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DesertCBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			DesertCBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		DesertCBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		DesertCBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		DesertCBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			DesertCBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

ABiome::Type	DesertCBiome::getType() const
{
	return m_type;
}


float			DesertCBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				DesertCBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			DesertCBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			DesertCBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				DesertCBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	DesertCBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		DesertCBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		DesertCBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
