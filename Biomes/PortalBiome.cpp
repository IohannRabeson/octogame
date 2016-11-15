#include "PortalBiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

PortalBiome::PortalBiome() :
	m_name("Portal"),
	m_id(Level::Portal),
	m_seed("Portal"),
	m_mapSize(sf::Vector2u(1150u, 26u)),
	m_mapSeed(42u),
	m_octoStartPosition(53.f * 16.f, 60.f),
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
	m_nightLightColor(255, 255, 255, 150),
	m_dayLightColor(150, 150, 150, 150),
	m_sunsetLightColor(255, 255, 255, 200),
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
	m_rainbowCount(30u, 30u),
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
	m_type(ABiome::Type::Random),

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
	m_cloudMaxY(-700.f),
	m_cloudMinY(-2500.f),
	m_cloudSpeed(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 0.f)),
	m_cloudLifeTime(sf::seconds(60), sf::seconds(90)),
	m_cloudColor(255, 255, 255, 200),

	m_starSize(sf::Vector2f(5.f, 5.f), sf::Vector2f(25.f, 25.f)),
	m_starColor(255, 255, 255),
	m_starLifeTime(sf::seconds(15), sf::seconds(90)),

	m_sunSize(sf::Vector2f(140.f, 140.f), sf::Vector2f(150.f, 150.f)),
	m_sunPartCount(2u, 4u),
	m_sunColor(50, 50, 50),

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

	Progress & progress = Progress::getInstance();
	progress.setGameFinished(true);

	// Create a set a 20 colors for particles
	std::size_t colorCount = 20;
	float interpolateDelta = 1.f / 20.f;
	m_particleColor.resize(colorCount);
	m_particleColor[0] = m_rockColor;
	for (std::size_t i = 1; i < colorCount; i++)
		m_particleColor[i] = octo::linearInterpolation(m_tileStartColor, m_tileEndColor, i * interpolateDelta);

	m_secondStartColor = getRockColor();
	m_secondEndColor = getRockColor();

	m_gameObjects[100] = GameObjectType::DeepoNpc;

	m_interestPointPosX = 420;

	m_gameObjects[50] = GameObjectType::PortalRed;

	m_gameObjects[150] = GameObjectType::PortalSnow;
	m_gameObjects[200] = GameObjectType::PortalSnow;
	m_gameObjects[250] = GameObjectType::PortalSnow;
	m_gameObjects[300] = GameObjectType::PortalSnow;

	m_gameObjects[400] = GameObjectType::PortalDesert;
	m_gameObjects[450] = GameObjectType::PortalDesert;
	m_gameObjects[500] = GameObjectType::PortalDesert;
	m_gameObjects[550] = GameObjectType::PortalDesert;

	m_gameObjects[650] = GameObjectType::PortalJungle;
	m_gameObjects[700] = GameObjectType::PortalJungle;
	m_gameObjects[750] = GameObjectType::PortalJungle;
	m_gameObjects[800] = GameObjectType::PortalJungle;

	m_gameObjects[900] = GameObjectType::PortalWater;
	m_gameObjects[950] = GameObjectType::PortalWater;
	m_gameObjects[1000] = GameObjectType::PortalWater;
	m_gameObjects[1050] = GameObjectType::PortalWater;

	m_destinations.push_back(Level::Final);
	m_destinations.push_back(Level::IceA);
	m_destinations.push_back(Level::IceB);
	m_destinations.push_back(Level::IceC);
	m_destinations.push_back(Level::IceD);
	m_destinations.push_back(Level::DesertA);
	m_destinations.push_back(Level::DesertB);
	m_destinations.push_back(Level::DesertC);
	m_destinations.push_back(Level::DesertD);
	m_destinations.push_back(Level::JungleA);
	m_destinations.push_back(Level::JungleB);
	m_destinations.push_back(Level::JungleC);
	m_destinations.push_back(Level::JungleD);
	m_destinations.push_back(Level::WaterA);
	m_destinations.push_back(Level::WaterB);
	m_destinations.push_back(Level::WaterC);
	m_destinations.push_back(Level::WaterD);

	switch (progress.getLastDestination())
	{
		case Level::IceA:
			m_octoStartPosition.x = 153.f * 16.f;
			break;
		case Level::IceB:
			m_octoStartPosition.x = 203.f * 16.f;
			break;
		case Level::IceC:
			m_octoStartPosition.x = 253.f * 16.f;
			break;
		case Level::IceD:
			m_octoStartPosition.x = 303.f * 16.f;
			break;
		case Level::DesertA:
			m_octoStartPosition.x = 403.f * 16.f;
			break;
		case Level::DesertB:
			m_octoStartPosition.x = 453.f * 16.f;
			break;
		case Level::DesertC:
			m_octoStartPosition.x = 503.f * 16.f;
			break;
		case Level::DesertD:
			m_octoStartPosition.x = 553.f * 16.f;
			break;
		case Level::JungleA:
			m_octoStartPosition.x = 653.f * 16.f;
			break;
		case Level::JungleB:
			m_octoStartPosition.x = 703.f * 16.f;
			break;
		case Level::JungleC:
			m_octoStartPosition.x = 753.f * 16.f;
			break;
		case Level::JungleD:
			m_octoStartPosition.x = 803.f * 16.f;
			break;
		case Level::WaterA:
			m_octoStartPosition.x = 903.f * 16.f;
			break;
		case Level::WaterB:
			m_octoStartPosition.x = 953.f * 16.f;
			break;
		case Level::WaterC:
			m_octoStartPosition.x = 1003.f * 16.f;
			break;
		case Level::WaterD:
			m_octoStartPosition.x = 1053.f * 16.f;
			break;
		default:
			break;
	}
}

void			PortalBiome::setup(std::size_t seed)
{
	(void)seed;
}

Level			PortalBiome::getId()const
{
	return m_id;
}

std::string		PortalBiome::getName()const
{
	return (m_name);
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	PortalBiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		PortalBiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	PortalBiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	PortalBiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			PortalBiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				PortalBiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	PortalBiome::getGameObjects()
{
	return m_gameObjects;
}

Level	PortalBiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	PortalBiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	PortalBiome::getWaterColor()
{
	return m_waterColor;
}

sf::Color	PortalBiome::getSecondWaterColor()
{
	return m_secondWaterColor;
}

std::map<std::size_t, std::string> const & PortalBiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> PortalBiome::getLayers()
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

Map::MapSurfaceGenerator PortalBiome::getMapSurfaceGenerator()
{
	return [this](Noise & noise, float x, float y)
	{
		float floatMapSize = static_cast<float>(m_mapSize.x);
		float n = noise.fBm(x, y, 3, 3.f, 0.3f);
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

Map::TileColorGenerator PortalBiome::getTileColorGenerator()
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

sf::Color		PortalBiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		PortalBiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		PortalBiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		PortalBiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		PortalBiome::getStartDayDuration()
{
	return (m_startDayDuration);
}

sf::Color		PortalBiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		PortalBiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		PortalBiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color	PortalBiome::getDayLightColor()
{
	return (m_dayLightColor);
}

sf::Color		PortalBiome::getSunsetLightColor()
{
	return (m_sunsetLightColor);
}

float			PortalBiome::getWind()
{
	return (m_wind);
}

void			PortalBiome::setWind(float wind)
{
	m_wind = wind;
}

bool			PortalBiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		PortalBiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		PortalBiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		PortalBiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			PortalBiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			PortalBiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			PortalBiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		PortalBiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		PortalBiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		PortalBiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		PortalBiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		PortalBiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		PortalBiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		PortalBiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		PortalBiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		PortalBiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		PortalBiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	PortalBiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	PortalBiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		PortalBiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		PortalBiome::getTreeColor()
{
	return (randomColor(m_treeColor));
}

float			PortalBiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			PortalBiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			PortalBiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			PortalBiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			PortalBiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	PortalBiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		PortalBiome::getLeafColor()
{
	return (randomColor(m_leafColor));
}

std::size_t		PortalBiome::getTreePositionX()
{
	return randomFloat(1u, m_mapSize.x);
}

sf::Vector2f	PortalBiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		PortalBiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		PortalBiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				PortalBiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			PortalBiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	PortalBiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		PortalBiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			PortalBiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			PortalBiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	PortalBiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		PortalBiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		PortalBiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

float	PortalBiome::getGrassSizeY()
{
	return randomRangeFloat(m_grassSizeY);
}

float	PortalBiome::getGrassSizeX()
{
	return randomRangeFloat(m_grassSizeX);
}

sf::Color	PortalBiome::getGrassColor()
{
	return randomColor(m_grassColor);
}

std::size_t	PortalBiome::getGrassCount()
{
	return m_grassCount;
}

std::size_t	PortalBiome::getGrassPosX()
{
	m_grassIndex++;
	if (m_grassIndex >= m_mapSize.x)
		m_grassIndex = 0u;
	return m_grassIndex;
}

bool			PortalBiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	PortalBiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		PortalBiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		PortalBiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			PortalBiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	PortalBiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		PortalBiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

float	PortalBiome::getCloudMaxY()
{
	return (m_cloudMaxY);
}

float	PortalBiome::getCloudMinY()
{
	return (m_cloudMinY);
}

sf::Vector2f	PortalBiome::getCloudSpeed()
{
	return randomRangeVector2f(m_cloudSpeed);
}

sf::Time		PortalBiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		PortalBiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			PortalBiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	PortalBiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		PortalBiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		PortalBiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			PortalBiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	PortalBiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		PortalBiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		PortalBiome::getSunColor()
{
	return (randomColor(m_sunColor));
}

bool			PortalBiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	PortalBiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		PortalBiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		PortalBiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			PortalBiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			PortalBiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			PortalBiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		PortalBiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		PortalBiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		PortalBiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			PortalBiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}

bool	PortalBiome::canCreateGrass()
{
	return m_canCreateGrass;
}

float	PortalBiome::getWaterPersistence() const
{
	return m_waterPersistence;
}

ABiome::Type	PortalBiome::getType() const
{
	return m_type;
}


float			PortalBiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				PortalBiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			PortalBiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			PortalBiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				PortalBiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	PortalBiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		PortalBiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		PortalBiome::randomColor(sf::Color const & color)
{
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
