#include "WaterABiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

WaterABiome::WaterABiome() :
	m_name("Water A"),
	m_id(Level::WaterA),
	m_seed("Water A"),
	m_mapSize(sf::Vector2u(700u, 32u)),
	m_mapSeed(42u),
	m_octoStartPosition(40.f * 16.f, -1050.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(255, 193, 177),
	m_tileEndColor(249, 237, 188),
	m_waterLevel(300.f),
	m_waterColor(3, 57, 108, 130),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(90.f)),
	m_startDayDuration(sf::seconds(15.f)),
	m_skyDayColor(153, 204, 255),
	m_skyNightColor(255, 90, 61),
	m_nightLightColor(255, 90, 61, 130),
	m_SunsetLightColor(255, 147, 46, 130),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 15u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(30u, 35u),
	m_treeCount(30u, 30u),
	m_mushroomCount(390u, 400u),
	m_crystalCount(70u, 100u),
	m_starCount(500u, 800u),
	m_sunCount(3u, 3u),
	m_moonCount(1u, 1u),
	m_rainbowCount(4u, 5u),
	m_cloudCount(10u, 15u),
	m_groundRockCount(200u, 400u),

	m_canCreateRain(true),
	m_canCreateThunder(false),
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
	m_canCreateRainbow(true),
	m_canCreateGrass(false),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Water),

	m_rockSize(sf::Vector2f(20.f, 100.f), sf::Vector2f(40.f, 300.f)),
	m_rockPartCount(10.f, 15.f),
	m_rockColor(60, 72, 84),

	m_grassSizeY(30.f, 60.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),
	m_grassIndex(0u),

	m_treeDepth(6u, 7u),
	m_treeSize(sf::Vector2f(5.f, 160.f), sf::Vector2f(20.f, 161.f)),
	m_treeLifeTime(sf::seconds(20.f), sf::seconds(50.f)),
	m_treeColor(0, 255, 159),
	m_treeAngle(-180.f, 180.f),
	m_treeBeatMouvement(0.1f),
	m_leafSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(40.f, 40.f)),
	m_leafColor(0, 255, 159, 150.f),

	m_mushroomSize(sf::Vector2f(10.f, 20.f), sf::Vector2f(20.f, 50.f)),
	m_mushroomColor(60, 0, 53, 150.f),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(5.f, 30.f), sf::Vector2f(20.f, 80.f)),
	m_crystalPartCount(15u, 25u),
	m_crystalColor(162, 121, 143, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(400.f, 200.f), sf::Vector2f(800.f, 400.f)),
	m_cloudPartCount(3u, 4u),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(15.f, 15.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 255),

	m_moonSize(sf::Vector2f(300.f, 300.f), sf::Vector2f(400.f, 400.f)),
	m_moonColor(255, 255, 255),
	m_moonLifeTime(sf::seconds(15.f), sf::seconds(30.f)),

	m_rainbowThickness(70.f, 120.f),
	m_rainbowPartSize(50.f, 150.f),
	m_rainbowLoopCount(1u, 4u),
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
	m_instances[140] = MAP_WATER_A_TRAIL_OMP;

	m_interestPointPosX = 500;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::JungleC);
	m_destinations.push_back(Level::WaterB);
	m_destinations.push_back(Level::WaterB);
}

void			WaterABiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		WaterABiome::getName()const
{
	return (m_name);
}

Level			WaterABiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	WaterABiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		WaterABiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	WaterABiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	WaterABiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			WaterABiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				WaterABiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	WaterABiome::getGameObjects()
{
	return m_gameObjects;
}

Level	WaterABiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	WaterABiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	WaterABiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	WaterABiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & WaterABiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> WaterABiome::getLayers()
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

Map::MapSurfaceGenerator WaterABiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		std::vector<float> pointX = {0.f, 140.f, m_mapSize.x * 1.f};
		std::vector<float> pointY = {n  , 0.f  , n};
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

Map::TileColorGenerator WaterABiome::getTileColorGenerator()
{
	sf::Color secondColorStart(143, 93, 195);
	sf::Color secondColorEnd(61, 14, 111);
	float startTransition = 1000.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 5000.f / static_cast<float>(m_mapSize.y);
	float endTransition = 8000.f / static_cast<float>(m_mapSize.y);
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

sf::Color		WaterABiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		WaterABiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		WaterABiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		WaterABiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		WaterABiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		WaterABiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		WaterABiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		WaterABiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		WaterABiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			WaterABiome::getWind()
{
	return (m_wind);
}

void			WaterABiome::setWind(float wind)
{
	m_wind = wind;
}

bool			WaterABiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		WaterABiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		WaterABiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		WaterABiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			WaterABiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			WaterABiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			WaterABiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		WaterABiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		WaterABiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		WaterABiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		WaterABiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		WaterABiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		WaterABiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		WaterABiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		WaterABiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		WaterABiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		WaterABiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	WaterABiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	WaterABiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		WaterABiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		WaterABiome::getTreeColor()
{
	return (randomColorLeaf(m_treeColor));
}

float			WaterABiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			WaterABiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			WaterABiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			WaterABiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			WaterABiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	WaterABiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterABiome::getLeafColor()
{
	return (randomColorLeaf(m_leafColor));
}

std::size_t		WaterABiome::getTreePositionX()
{
	std::size_t pos;
	bool isValidPos = false;
	while (isValidPos == false)
	{
		pos = randomInt(1u, m_mapSize.x - 1u);
			if ((pos >= 0.f && pos <= 150.f + 20.f) || (pos >= 620.f - 20.f && pos <= 750.f + 20.f) || (pos >= 1000.f - 20.f && pos <= 1080.f + 30.f))
				isValidPos = false;
			else
				isValidPos = true;
	}
	return pos;
}

sf::Vector2f	WaterABiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		WaterABiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		WaterABiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				WaterABiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			WaterABiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	WaterABiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		WaterABiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			WaterABiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			WaterABiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	WaterABiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		WaterABiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		WaterABiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	WaterABiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

sf::Color	WaterABiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	WaterABiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	WaterABiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			WaterABiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	WaterABiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		WaterABiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		WaterABiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			WaterABiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	WaterABiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		WaterABiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		WaterABiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		WaterABiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			WaterABiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	WaterABiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		WaterABiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		WaterABiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			WaterABiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	WaterABiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		WaterABiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		WaterABiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			WaterABiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	WaterABiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		WaterABiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		WaterABiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			WaterABiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			WaterABiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			WaterABiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		WaterABiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		WaterABiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		WaterABiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			WaterABiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	WaterABiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	WaterABiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	WaterABiome::getType() const
{
	return m_type;
}


float			WaterABiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				WaterABiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			WaterABiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			WaterABiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				WaterABiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	WaterABiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		WaterABiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		WaterABiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}

sf::Color		WaterABiome::randomColorLeaf(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-180.f, 180.f);
	tmp.Luminance += m_generator.randomFloat(-20.f, 0.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
