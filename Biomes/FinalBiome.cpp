#include "FinalBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

FinalBiome::FinalBiome() :
	m_name("Final"),
	m_id(Level::Final),
	m_seed("Final"),
	m_mapSize(sf::Vector2u(1400u, 16u)),
	m_mapSeed(42u),
	//m_octoStartPosition(750.f * 16.f, -50.f),
	m_octoStartPosition(1300.f * 16.f, -50.f),
	m_transitionDuration(3.0f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(58, 0, 92),
	m_tileEndColor(109, 0, 179),
	m_waterLevel(-1.f),
	m_waterColor(255, 255, 255, 100),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(90.f)),
	m_startDayDuration(sf::Time::Zero),
	m_skyDayColor(52, 247, 61),
	m_skyNightColor(52, 247, 61),
	m_nightLightColor(103, 0, 154, 140),
	m_dayLightColor(52, 247, 61, 100),
	m_SunsetLightColor(52, 247, 61, 180),
	m_wind(30.f),
	m_rainDropPerSecond(20u, 35u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 2500.f),

	m_rockCount(20u, 20u),
	m_treeCount(20u, 20u),
	m_mushroomCount(100u, 100u),
	m_crystalCount(20u, 30u),
	m_starCount(530u, 550u),
	m_sunCount(3u, 3u),
	m_moonCount(3u, 4u),
	m_rainbowCount(0u, 0u),
	m_cloudCount(150u, 150u),
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
	m_canCreateGrass(true),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Water),

	m_rockSize(sf::Vector2f(20.f, 200.f), sf::Vector2f(40.f, 300.f)),
	m_rockPartCount(10.f, 16.f),
	m_rockColor(sf::Color::Black),

	m_grassSizeY(40.f, 150.f),
	m_grassSizeX(10.f, 30.f),
	m_grassColor(250, 240, 250),
	m_grassCount(m_mapSize.x),

	m_treeDepth(8u, 8u),
	m_treeSize(sf::Vector2f(30.f, 80.f), sf::Vector2f(70.f, 300.f)),
	m_treeLifeTime(sf::seconds(20.f), sf::seconds(50.f)),
	m_treeColor(58, 0, 92),
	m_treeAngle(-100.f, 100.f),
	m_treeBeatMouvement(0.04f),
	m_leafSize(sf::Vector2f(70.f, 10.f), sf::Vector2f(150.f, 150.f)),
	m_leafColor(212, 5, 238),

	m_mushroomSize(sf::Vector2f(40.f, 50.f), sf::Vector2f(100.f, 500.f)),
	m_mushroomColor(58, 0, 92),
	m_mushroomLifeTime(sf::seconds(5), sf::seconds(20)),

	m_crystalSize(sf::Vector2f(2.f, 100.f), sf::Vector2f(70.f, 250.f)),
	m_crystalPartCount(6u, 10u),
	m_crystalColor(253, 255, 0, 140),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 130),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(100.f, 150.f), sf::Vector2f(400.f, 800.f)),
	m_cloudPartCount(3u, 5u),
	m_cloudMaxY(1000.f),
	m_cloudMinY(-1000.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 100),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(35.f, 35.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(200.f, 200.f), sf::Vector2f(300.f, 300.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(255, 255, 255),

	m_moonSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_moonColor(255, 255, 255),
	m_moonLifeTime(sf::seconds(15.f), sf::seconds(30.f)),

	m_rainbowThickness(60.f, 100.f),
	m_rainbowPartSize(30.f, 100.f),
	m_rainbowLoopCount(1u, 4u),
	m_rainbowLifeTime(sf::seconds(5.f), sf::seconds(10.f)),
	m_rainbowIntervalTime(sf::seconds(5.f), sf::seconds(10.f))
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
	m_instances[800] = MAP_FINAL_PYRAMID_OMP;
	m_instances[300] = MAP_FINAL_BOTTOM_OMP;
	m_instances[50] = MAP_FINAL_LEFT_OMP;
	m_instances[1300] = MAP_FINAL_RIGHT_OMP;

	std::vector<GameObjectType> object = {GameObjectType::ForestSpirit1Npc, GameObjectType::ForestSpirit2Npc, GameObjectType::FranGlitchNpc, GameObjectType::JuGlitchNpc, GameObjectType::LuGlitchNpc, GameObjectType::WindowGlitchNpc};

	for (std::size_t i = 0; i < 40; i++)
	{
		m_gameObjects[randomInt(300u, 1300u)] = object[randomInt(0u, object.size())];
	}

	m_interestPointPosX = 500;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Red);
	m_destinations.push_back(Level::Blue);
}

void			FinalBiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		FinalBiome::getName()const
{
	return (m_name);
}

Level			FinalBiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	FinalBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		FinalBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	FinalBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	FinalBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			FinalBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				FinalBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	FinalBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	FinalBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	FinalBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	FinalBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	FinalBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & FinalBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> FinalBiome::getLayers()
{
	std::vector<ParallaxScrolling::ALayer *> vector;

	return vector;
}

Map::MapSurfaceGenerator FinalBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		float m = n * 5.f;
		std::vector<float> pointX = {0.f  , 299.f, 300.f, 350.f, 750.f, 805.f, 945.f, 1000.f, 1250.f, 1299.f, 1300.f, 1400.f};
		std::vector<float> pointY = {900.f, 900.f, 1.1f , n    , m    , -3.4f, -3.5f, m     , n     , 1.1f  , 900.f , 900.f };
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

Map::TileColorGenerator FinalBiome::getTileColorGenerator()
{
	sf::Color secondColorStart = getLeafColor();
	sf::Color secondColorEnd = getLeafColor();
	float startTransition = 200.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 800.f / static_cast<float>(m_mapSize.y);
	float endTransition = 1700.f / static_cast<float>(m_mapSize.y);
	return [this, secondColorStart, secondColorEnd, startTransition, endTransition, middleTransition](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		sf::Color color;

		if (y > startTransition && y <= middleTransition)
		{
			float ratio = (y - (startTransition)) / (middleTransition - startTransition);
			color = octo::linearInterpolation(octo::linearInterpolation(m_tileStartColor, secondColorStart, ratio), m_tileEndColor, transition);
		}
		else if (y > middleTransition && y <= endTransition)
		{
			float ratio = (y - (middleTransition)) / (endTransition - middleTransition);
			color = octo::linearInterpolation(secondColorStart, octo::linearInterpolation(m_tileEndColor, secondColorEnd, ratio), transition);
		}
		else if (y > endTransition)
			color = octo::linearInterpolation(secondColorStart, secondColorEnd, transition);
		else
			color = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);

		if (x > 1200.f && x <= 1400.f)
		{
			float ratio = (x - 1200.f) / (1400.f - 1200.f);
			color = octo::linearInterpolation(octo::linearInterpolation(color, secondColorStart, ratio), color, transition);
		}
		return color;
	};
}

sf::Color		FinalBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		FinalBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		FinalBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		FinalBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		FinalBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		FinalBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		FinalBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		FinalBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	FinalBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		FinalBiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			FinalBiome::getWind()
{
	return (m_wind);
}

void			FinalBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			FinalBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		FinalBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		FinalBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		FinalBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			FinalBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			FinalBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			FinalBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		FinalBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		FinalBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		FinalBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		FinalBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		FinalBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		FinalBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		FinalBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		FinalBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		FinalBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		FinalBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	FinalBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	FinalBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		FinalBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		FinalBiome::getTreeColor()
{
	return (randomColorLeaf(m_treeColor));
}

float			FinalBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			FinalBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			FinalBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			FinalBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			FinalBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	FinalBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		FinalBiome::getLeafColor()
{
	return (randomColorLeaf(m_leafColor));
}

std::size_t		FinalBiome::getTreePositionX()
{
	int x;

	if (randomBool(0.75f))
	{
		x = static_cast<int>(m_generator.randomPiecewise(500.f)) + 325.f;
	}
	else
	{
		x = static_cast<int>(m_generator.randomPiecewise(350.f)) + 925.f;
	}
	return (static_cast<int>(x));
}

sf::Vector2f	FinalBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		FinalBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		FinalBiome::getCrystalColor()
{
	return (randomColorLeaf(m_crystalColor));
}

int				FinalBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			FinalBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	FinalBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		FinalBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			FinalBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			FinalBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	FinalBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		FinalBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		FinalBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	FinalBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	FinalBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	FinalBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	FinalBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	FinalBiome::getGrassPosX()
{
	return (randomInt(1u, m_mapSize.x));
}

bool			FinalBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	FinalBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		FinalBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		FinalBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			FinalBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	FinalBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		FinalBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	FinalBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	FinalBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	FinalBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		FinalBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		FinalBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			FinalBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	FinalBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		FinalBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		FinalBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			FinalBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	FinalBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		FinalBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		FinalBiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			FinalBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	FinalBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		FinalBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		FinalBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			FinalBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			FinalBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			FinalBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		FinalBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		FinalBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		FinalBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			FinalBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	FinalBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	FinalBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	FinalBiome::getType() const
{
	return m_type;
}


float			FinalBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				FinalBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			FinalBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			FinalBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				FinalBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	FinalBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		FinalBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		FinalBiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}

sf::Color		FinalBiome::randomColorLeaf(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);
}
