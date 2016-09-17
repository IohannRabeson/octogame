#include "DesertABiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

DesertABiome::DesertABiome() :
	m_name("Desert A"),
	m_id(Level::DesertA),
	m_seed("CaillouX"),
	m_mapSize(sf::Vector2u(540u, 128u)),
	m_mapSeed(42u),
	m_octoStartPosition(253.f * 16.f, 53.f * 16.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(245, 222, 130),
	m_tileEndColor(245, 243, 219),
	m_waterLevel(-1.f),
	m_waterColor(96, 204, 233, 180),
	m_secondWaterColor(m_waterColor),
	m_destinationIndex(0u),

	m_dayDuration(sf::seconds(100.f)),
	m_startDayDuration(sf::seconds(15.f)),
	m_skyDayColor(255, 150, 242),
	m_skyNightColor(166, 10, 92),
	m_nightLightColor(134, 63, 215, 130),
	m_dayLightColor(sf::Color::Transparent),
	m_SunsetLightColor(255, 59, 59, 130),
	m_wind(100.f),
	m_rainDropPerSecond(10u, 30u),
	m_sunnyTime(sf::seconds(10.f), sf::seconds(15.f)),
	m_rainingTime(sf::seconds(15.f), sf::seconds(20.f)),
	m_lightningSize(700.f, 1300.f),

	m_rockCount(10u, 20u),
	m_treeCount(13u, 13u),
	m_mushroomCount(3u, 40u),
	m_crystalCount(18u, 19u),
	m_starCount(500u, 800u),
	m_sunCount(1u, 1u),
	m_moonCount(2u, 3u),
	m_rainbowCount(1u, 2u),
	m_cloudCount(20u, 40u),
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
	m_canCreateCloud(false),
	m_canCreateStar(true),
	m_canCreateSun(true),
	m_canCreateMoon(true),
	m_canCreateRainbow(false),
	m_canCreateGrass(true),
	m_waterPersistence(0.f),
	m_type(ABiome::Type::Ice),

	m_rockSize(sf::Vector2f(15.f, 100.f), sf::Vector2f(30.f, 400.f)),
	m_rockPartCount(50.f, 80.f),
	m_rockColor(255, 232, 170),

	m_grassSizeY(30.f, 60.f),
	m_grassSizeX(14.f, 16.f),
	m_grassColor(46, 133, 84),
	m_grassCount(120u),

	m_treeDepth(6u, 8u),
	m_treeSize(sf::Vector2f(15.f, 100.f), sf::Vector2f(30.f, 150.f)),
	m_treeLifeTime(sf::seconds(30), sf::seconds(90)),
	m_treeColor(53, 44, 45),
	m_treeAngle(15.f, 75.f),
	m_treeBeatMouvement(0.06f),
	m_leafSize(sf::Vector2f(40.f, 40.f), sf::Vector2f(100.f, 100.f)),
	m_leafColor(46, 133, 84),

	m_mushroomSize(sf::Vector2f(20.f, 50.f), sf::Vector2f(40.f, 100.f)),
	m_mushroomColor(77, 142, 126),
	m_mushroomLifeTime(sf::seconds(10), sf::seconds(30)),

	m_crystalSize(sf::Vector2f(10.f, 50.f), sf::Vector2f(25.f, 100.f)),
	m_crystalPartCount(2u, 8u),
	m_crystalColor(18, 14, 66, 150),
	m_shineEffectSize(sf::Vector2f(100.f, 100.f), sf::Vector2f(200.f, 200.f)),
	m_shineEffectColor(255, 255, 255, 100),
	m_shineEffectRotateAngle(100.f, 200.f),

	m_cloudSize(sf::Vector2f(200.f, 100.f), sf::Vector2f(400.f, 200.f)),
	m_cloudPartCount(6u, 10u),
	m_cloudMaxY(-1000.f),
	m_cloudMinY(-4000.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
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
	if (progress.getLastDestination() == Level::DesertB)
		m_octoStartPosition = sf::Vector2f(373 * 16.f, -1130.f);
	if (progress.getLastDestination() == Level::Random)
		m_octoStartPosition = sf::Vector2f(510 * 16.f, -2700.f);

	// Define game objects
	m_instances[23] = MAP_DESERT_A_WAVE_OMP;
	m_gameObjects[70] = GameObjectType::TurbanNpc;
	m_gameObjects[220] = GameObjectType::JuNpc;
	m_instances[250] = MAP_DESERT_A_JUMP_OMP;
	m_gameObjects[250] = GameObjectType::PortalSnow;
	m_gameObjects[320] = GameObjectType::WindowGlitchNpc;
	m_gameObjects[370] = GameObjectType::PortalDesert;
	m_gameObjects[410] = GameObjectType::OldDesertStaticNpc;
	m_gameObjects[420] = GameObjectType::Tent;
	m_instances[440] = MAP_DESERT_A_SECRET_OMP;
	m_gameObjects[520] = GameObjectType::JuGlitchNpc;
	m_interestPointPosX = 50;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Random);
	m_destinations.push_back(Level::IceD);
	m_destinations.push_back(Level::DesertB);
}

void			DesertABiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		DesertABiome::getName()const
{
	return (m_name);
}

Level			DesertABiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	DesertABiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		DesertABiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	DesertABiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	DesertABiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			DesertABiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DesertABiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	DesertABiome::getGameObjects()
{
	return m_gameObjects;
}

Level	DesertABiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	DesertABiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	DesertABiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	DesertABiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & DesertABiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> DesertABiome::getLayers()
{
	sf::Vector2u const & mapSize = getMapSize();
	std::vector<ParallaxScrolling::ALayer *> vector;

	GenerativeLayer * layer = new GenerativeLayer(m_skyDayColor, sf::Vector2f(0.2f, 0.6f), mapSize, 8.f, -40, 0.1f, 0.8f, -1.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 2.f, y * 10.f);
		});
	vector.push_back(layer);
	layer = new GenerativeLayer(m_skyDayColor, sf::Vector2f(0.4f, 0.4f), mapSize, 10.f, -20, 0.3f, 0.9f, 11.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x, y);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator DesertABiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
		float m = n / 5.f;
		std::vector<float> pointX = {0.f, 250.f, 300.f, 335.f, 340.f    , 350.f    , 470.f    , 480.f    , 485.f, 515.f, 565.f};
		std::vector<float> pointY = {n  , n    , m    , m    , -0.2f + m, -1.9f + m, -1.9f + m, -0.2f + m, m    , m    , n};
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

Map::TileColorGenerator DesertABiome::getTileColorGenerator()
{
	sf::Color secondColorStart = getLeafColor();
	sf::Color secondColorEnd = m_particleColor[0];
	float start1 = -14700.f / static_cast<float>(m_mapSize.y);
	float start2 = -14000.f / static_cast<float>(m_mapSize.y);
	float middle1 = -13000.f / static_cast<float>(m_mapSize.y);
	float middle2 = -6300.f / static_cast<float>(m_mapSize.y);
	float end1 = 0.f / static_cast<float>(m_mapSize.y);
	float end2 = 9000.f / static_cast<float>(m_mapSize.y);
	return [this, secondColorStart, secondColorEnd, start1, start2, middle1, middle2, end1, end2](Noise & noise, float x, float y, float z)
	{
		float transition = (noise.noise(x / 10.f, y / 10.f, z / 10.f) + 1.f) / 2.f;
		if (y > start1 && y <= start2)
		{
			float ratio = (y - (start1)) / (start2 - start1);
			return octo::linearInterpolation(octo::linearInterpolation(m_tileStartColor, secondColorStart, ratio), m_tileEndColor, transition);
		}
		else if (y > start2 && y <= middle1)
		{
			float ratio = (y - (start2)) / (middle1 - start2);
			return octo::linearInterpolation(secondColorStart, octo::linearInterpolation(m_tileEndColor, secondColorEnd, ratio), transition);
		}
		else if (y > middle1 && y <= middle2)
		{
			return octo::linearInterpolation(secondColorStart, secondColorEnd, transition);
		}
		else if (y >= middle2 && y < end1)
		{
			float ratio = (y - (middle2)) / (end1 - middle2);
			return octo::linearInterpolation(octo::linearInterpolation(secondColorStart, m_tileStartColor, ratio), secondColorEnd, transition);
		}
		else if (y >= end1 && y < end2)
		{
			float ratio = (y - (end1)) / (end2 - end1);
			return octo::linearInterpolation(m_tileStartColor, octo::linearInterpolation(secondColorEnd, m_tileEndColor, ratio), transition);
		}
		return octo::linearInterpolation(m_tileStartColor, m_tileEndColor, transition);
	};
}

sf::Color		DesertABiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		DesertABiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		DesertABiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		DesertABiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		DesertABiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		DesertABiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		DesertABiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		DesertABiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	DesertABiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		DesertABiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			DesertABiome::getWind()
{
	return (m_wind);
}

void			DesertABiome::setWind(float wind)
{
	m_wind = wind;
}

bool			DesertABiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		DesertABiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		DesertABiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		DesertABiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			DesertABiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			DesertABiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			DesertABiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		DesertABiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		DesertABiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		DesertABiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		DesertABiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		DesertABiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		DesertABiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		DesertABiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		DesertABiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		DesertABiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		DesertABiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	DesertABiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	DesertABiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		DesertABiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		DesertABiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			DesertABiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			DesertABiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			DesertABiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			DesertABiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			DesertABiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	DesertABiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertABiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		DesertABiome::getTreePositionX()
{
	return randomInt(350u, 470u);
}

sf::Vector2f	DesertABiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		DesertABiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		DesertABiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				DesertABiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			DesertABiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	DesertABiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		DesertABiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			DesertABiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			DesertABiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	DesertABiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		DesertABiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		DesertABiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	DesertABiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	DesertABiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	DesertABiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	DesertABiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	DesertABiome::getGrassPosX()
{
	return randomInt(350u, 470u);
}

bool			DesertABiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	DesertABiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		DesertABiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		DesertABiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			DesertABiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	DesertABiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		DesertABiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	DesertABiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	DesertABiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	DesertABiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		DesertABiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		DesertABiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			DesertABiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	DesertABiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		DesertABiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		DesertABiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			DesertABiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	DesertABiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		DesertABiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		DesertABiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			DesertABiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	DesertABiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DesertABiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		DesertABiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			DesertABiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DesertABiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			DesertABiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		DesertABiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		DesertABiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		DesertABiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			DesertABiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	DesertABiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	DesertABiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	DesertABiome::getType() const
{
	return m_type;
}


float			DesertABiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				DesertABiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			DesertABiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			DesertABiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				DesertABiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	DesertABiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		DesertABiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		DesertABiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
