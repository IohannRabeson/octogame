#include "LaboBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

LaboBiome::LaboBiome() :
	m_name("Labo"),
	m_id(Level::Labo),
	m_seed("Levelne"),
	m_mapSize(sf::Vector2u(610u, 16u)),
	m_mapSeed(42u),
	m_octoStartPosition(136.f * 16.f, 20.f * 16.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(227, 227, 227),
	m_tileEndColor(137, 189, 211),
	m_waterLevel(-1.f),
	m_waterColor(255, 255, 255, 200),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(120.f)),
	m_startDayDuration(sf::seconds(90.f)),
	m_skyDayColor(8, 20, 26),
	m_skyNightColor(78, 47, 4, 130),
	m_nightLightColor(8, 20, 26, 50),
	m_dayLightColor(sf::Color::Transparent),
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
	m_canCreateThunder(true),
	m_canCreateSnow(true),
	m_canCreateRock(true),
	m_canCreateTree(true),
	m_canCreateLeaf(true),
	m_treeIsMoving(true),
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

	m_treeDepth(8u, 8u),
	m_treeSize(sf::Vector2f(50.f, 50.f), sf::Vector2f(50.f, 50.f)),
	m_treeLifeTime(sf::seconds(300), sf::seconds(400)),
	m_treeColor(188, 206, 213),
	m_treeAngle(15.f, 75.f),
	m_treeBeatMouvement(0.05f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(40.f, 40.f)),
	m_leafColor(152, 111, 149),

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
	m_cloudMinY(-6000.f),
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
	Progress & progress = Progress::getInstance();
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	if (progress.isIntro())
	{
		m_startDayDuration = sf::seconds(90.f);
		m_dayDuration = sf::seconds(120.f);
	}

	m_secondStartColor = getRockColor();
	m_secondEndColor = getRockColor();

	m_gameObjects[420] = GameObjectType::PortalSnow;
	m_instances[470] = MAP_ICE_A_SECRET_OMP;
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::IceB);

	m_interestPointPosX = 420;

	m_treePos = {156, 300, 306, 309, 320, 329, 340, 354, 359, 375, 450, 459, 463, 469, 485, 501, 510, 523, 550};
}

void			LaboBiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			LaboBiome::getId()const
{
	return m_id;
}

std::string		LaboBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	LaboBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		LaboBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	LaboBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	LaboBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			LaboBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				LaboBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	LaboBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	LaboBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	LaboBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	LaboBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	LaboBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & LaboBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> LaboBiome::getLayers()
{
	sf::Vector2u const & mapSize = sf::Vector2u(getMapSize().x, getMapSize().y * 4u);
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(getParticleColorGround() - sf::Color(130, 130, 130, 0), sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, 10, 0.1f, 0.9f, 11.f, 400.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(getParticleColorGround() - sf::Color(130, 130, 130, 0), sf::Vector2f(0.5f, 0.3f), mapSize, 15.f, 15, 0.2f, 0.8f, 6.f, 400.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.perlin(x, y, 3, 2.f);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator LaboBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		m_pointX = {0.f    , 20.f, 70.f, 95.f, 120.f, 125.f, 165.f, 166.f, 195.f, 220.f, 270.f, 290.f, 350.f, 369.f, 377.f, 396.f, 450.f, 469.f, 477.f, 496.f, 590.f, 610.f  };
		m_pointY = {n / 5.f, 0.f , 0.f , 0.f , 0.f  , 2.4f , 2.4f , 0.f  , n    , 0.f  , 0.f  , n    , n    , 0.1f , 0.1f , n    , n    , 0.1f , 0.1f , n    , n    , n / 5.f};
		for (std::size_t i = 0u; i < m_pointX.size(); i++)
			m_pointX[i] /= floatMapSize;

		for (std::size_t i = 0u; i < m_pointX.size() - 1u; i++)
		{
			if (x >= m_pointX[i] && x < m_pointX[i + 1])
			{
				float coef = (x - m_pointX[i]) / (m_pointX[i + 1] - m_pointX[i]);
				return octo::cosinusInterpolation(m_pointY[i], m_pointY[i + 1], coef);
			}
		}
		return n;
	};
}

Map::TileColorGenerator LaboBiome::getTileColorGenerator()
{
	float startTransition = 300.f / static_cast<float>(m_mapSize.y);
	float middleTransition = 600.f / static_cast<float>(m_mapSize.y);
	float endTransition = 900.f / static_cast<float>(m_mapSize.y);
	return [this, startTransition, endTransition, middleTransition](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		if (y > startTransition && y <= middleTransition)
		{
			float ratio = (y - (startTransition)) / (middleTransition - startTransition);
			return octo::linearInterpolation(octo::linearInterpolation(m_tileStartColor, m_secondStartColor, ratio), m_tileEndColor, transition);
		}
		else if (y > middleTransition && y <= endTransition)
		{
			float ratio = (y - (middleTransition)) / (endTransition - middleTransition);
			return octo::linearInterpolation(m_secondStartColor, octo::linearInterpolation(m_tileEndColor, m_secondEndColor, ratio), transition);
		}
		if (y > endTransition)
			return octo::linearInterpolation(m_secondStartColor, m_secondEndColor, transition);
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		LaboBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		LaboBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		LaboBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		LaboBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		LaboBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		LaboBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		LaboBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		LaboBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	LaboBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		LaboBiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			LaboBiome::getWind()
{
	return (m_wind);
}

void			LaboBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			LaboBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		LaboBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		LaboBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		LaboBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			LaboBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			LaboBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			LaboBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		LaboBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		LaboBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		LaboBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		LaboBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		LaboBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		LaboBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		LaboBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		LaboBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		LaboBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		LaboBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	LaboBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	LaboBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		LaboBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		LaboBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			LaboBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			LaboBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			LaboBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			LaboBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			LaboBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	LaboBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		LaboBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		LaboBiome::getTreePositionX()
{
	return (m_treePos[m_indexTreePos++]);
}

sf::Vector2f	LaboBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		LaboBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		LaboBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				LaboBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			LaboBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	LaboBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		LaboBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			LaboBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			LaboBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	LaboBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		LaboBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		LaboBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	LaboBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	LaboBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	LaboBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	LaboBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	LaboBiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			LaboBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	LaboBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		LaboBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		LaboBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			LaboBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	LaboBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		LaboBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	LaboBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	LaboBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	LaboBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		LaboBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		LaboBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			LaboBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	LaboBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		LaboBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		LaboBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			LaboBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	LaboBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		LaboBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		LaboBiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			LaboBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	LaboBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		LaboBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		LaboBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			LaboBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			LaboBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			LaboBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		LaboBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		LaboBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		LaboBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			LaboBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	LaboBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	LaboBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	LaboBiome::getType() const
{
	return m_type;
}


float			LaboBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				LaboBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			LaboBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			LaboBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				LaboBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	LaboBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		LaboBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		LaboBiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
