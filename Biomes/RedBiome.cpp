#include "RedBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

RedBiome::RedBiome() :
	m_name("Red"),
	m_id(Level::Red),
	m_seed("Red"),
	m_mapSize(sf::Vector2u(1000u, 26u)),
	m_mapSeed(42u),
	m_octoStartPosition(136.f * 16.f, 60.f),
	m_transitionDuration(2.0f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(150, 150, 150),
	m_tileEndColor(100, 100, 100),
	m_waterLevel(-1.f),
	m_waterColor(255, 255, 255, 200),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(40.f)),
	m_startDayDuration(sf::seconds(19.f)),
	m_skyDayColor(108, 108, 108),
	m_skyNightColor(78, 78, 78),
	m_nightLightColor(155, 0, 0, 150),
	m_dayLightColor(155, 0, 0),
	m_sunsetLightColor(155, 0, 0, 200),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(27u, 28u),
	m_treeCount(40u, 40u),
	m_mushroomCount(50u, 50u),
	m_crystalCount(30u, 30u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 2u),
	m_rainbowCount(80u, 80u),
	m_cloudCount(20u, 40u),
	m_groundRockCount(100u, 200u),

	m_canCreateRain(false),
	m_canCreateThunder(false),
	m_canCreateSnow(true),
	m_canCreateRock(true),
	m_canCreateTree(false),
	m_canCreateLeaf(false),
	m_treeIsMoving(false),
	m_canCreateMushroom(true),
	m_canCreateCrystal(true),
	m_canCreateShineEffect(false),
	m_canCreateCloud(true),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(true),
	m_canCreateGrass(true),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Ice),

	m_rockSize(sf::Vector2f(5.f, 50.f), sf::Vector2f(20.f, 70.f)),
	m_rockPartCount(2.f, 10.f),
	m_rockColor(0, 0, 0),

	m_grassSizeY(30.f, 60.f),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(m_tileStartColor),
	m_grassCount(m_mapSize.x),
	m_grassIndex(0u),

	m_treeDepth(6u, 7u),
	m_treeSize(sf::Vector2f(15.f, 60.f), sf::Vector2f(30.f, 150.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(188, 188, 188),
	m_treeAngle(10.f, 80.f),
	m_treeBeatMouvement(0.f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(100.f, 100.f)),
	m_leafColor(143, 143, 143),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 77, 77),
	m_mushroomLifeTime(sf::seconds(20), sf::seconds(60)),

	m_crystalSize(sf::Vector2f(5.f, 25.f), sf::Vector2f(10.f, 50.f)),
	m_crystalPartCount(2u, 3u),
	m_crystalColor(227, 227, 227, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(200.f, 100.f), sf::Vector2f(400.f, 200.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudMaxY(-1300.f),
	m_cloudMinY(-2500.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 200),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(25.f, 25.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(140.f, 140.f), sf::Vector2f(150.f, 150.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(128, 50, 50),

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
	m_mapSeed = 42u;

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	m_interestPointPosX = 420;

	Progress & progress = Progress::getInstance();
	if (progress.getLastDestination() == Level::IceB)
		m_octoStartPosition = sf::Vector2f(423 * 16.f, 0.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(404.f * 16.f, -1250.f);
}

void			RedBiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			RedBiome::getId()const
{
	return m_id;
}

std::string		RedBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	RedBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		RedBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	RedBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	RedBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			RedBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				RedBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	RedBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	RedBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	RedBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	RedBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	RedBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & RedBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> RedBiome::getLayers()
{
	sf::Vector2u const & mapSize = sf::Vector2u(getMapSize().x, getMapSize().y * 4u);
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround() - sf::Color(130, 130, 130, 150), sf::Vector2f(1.4f, 1.4f), mapSize, 10.f, 10, 0.1f, 0.4f, 1.5f, 1000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.fBm(x / 5.f, y, 3, 3.f, 1.5f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround() - sf::Color(130, 130, 130, 170), sf::Vector2f(1.5f, 1.3f), mapSize, 15.f, 15, 0.2f, 0.4f, 1.5f, 1000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.fBm(x / 5.f, y, 3, 3.f, 1.5f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround() - sf::Color(130, 130, 130, 190), sf::Vector2f(1.5f, 1.3f), mapSize, 20.f, 15, 0.2f, 0.4f, 1.5f, 1000.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.fBm(x / 5.f, y, 3, 3.f, 1.5f);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator RedBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 2.0f);
		std::vector<float> pointX = {0.f, 1000.f};
		std::vector<float> pointY = {n  , n};
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

Map::TileColorGenerator RedBiome::getTileColorGenerator()
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

sf::Color		RedBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		RedBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		RedBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		RedBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		RedBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		RedBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		RedBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		RedBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	RedBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		RedBiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			RedBiome::getWind()
{
	return (m_wind);
}

void			RedBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			RedBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		RedBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		RedBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		RedBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			RedBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			RedBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			RedBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		RedBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		RedBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		RedBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		RedBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		RedBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		RedBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		RedBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		RedBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		RedBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		RedBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	RedBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	RedBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		RedBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		RedBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			RedBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			RedBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			RedBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			RedBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			RedBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	RedBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		RedBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		RedBiome::getTreePositionX()
{
	return randomFloat(1u, m_mapSize.x);
}

sf::Vector2f	RedBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		RedBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		RedBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				RedBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			RedBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	RedBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		RedBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			RedBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			RedBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	RedBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		RedBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		RedBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	RedBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	RedBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	RedBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	RedBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	RedBiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			RedBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	RedBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		RedBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		RedBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			RedBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	RedBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		RedBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	RedBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	RedBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	RedBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		RedBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		RedBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			RedBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	RedBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		RedBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		RedBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			RedBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	RedBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		RedBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		RedBiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			RedBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	RedBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		RedBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		RedBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			RedBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			RedBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			RedBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		RedBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		RedBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		RedBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			RedBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	RedBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	RedBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	RedBiome::getType() const
{
	return m_type;
}


float			RedBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				RedBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			RedBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			RedBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				RedBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	RedBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		RedBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		RedBiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
