#include "DemoWaterABiome.hpp"
#include "Tile.hpp"
#include "GenerativeLayer.hpp"
#include "ResourceDefinitions.hpp"
#include "AGameObject.hpp"
#include <Interpolations.hpp>

#include <limits>
#include <iostream>

DemoWaterABiome::DemoWaterABiome() :
	m_name("Water A"),
	m_id(Level::WaterA),
	m_seed("Vince"),
	m_mapSize(sf::Vector2u(1300u, 200u)),
	m_mapSeed(42u),
	m_octoStartPosition(40.f * 16.f, -1050.f),
	m_transitionDuration(0.5f),
	m_interestPointPosX(m_mapSize.x / 2.f),
	m_tileStartColor(255, 193, 177),
	m_tileEndColor(249, 237, 188),
	m_waterLevel(1.f),
	m_waterColor(3, 57, 108, 130),
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

	m_rockSize(sf::Vector2f(20.f, 100.f), sf::Vector2f(40.f, 300.f)),
	m_rockPartCount(10.f, 15.f),
	m_rockColor(60, 72, 84),

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
	m_instances[140] = MAP_WATER_A_TRAIL_SLOWFALL_OMP;
	m_instances[785] = MAP_WATER_A_PARA_SIGN_OMP;
//	m_instances[900] = MAP_WATER_A_PORTAL_OMP;
	m_gameObjects[40] = GameObjectType::Portal;
	m_gameObjects[75] = GameObjectType::BrayouNpc;
	m_gameObjects[149] = GameObjectType::EvaNpc;
	m_gameObjects[1050] = GameObjectType::JeffMouffyNpc;
//	m_gameObjects[668] = GameObjectType::PeaNpc;
//	m_gameObjects[710] = GameObjectType::WaterNanoRobot;
//	m_gameObjects[730] = GameObjectType::Concert;
	m_gameObjects[745] = GameObjectType::Portal;

	m_gameObjects[200] = GameObjectType::JellyfishNpc;
	m_gameObjects[270] = GameObjectType::JellyfishNpc;
	m_gameObjects[420] = GameObjectType::JellyfishNpc;
	m_gameObjects[540] = GameObjectType::JellyfishNpc;
	m_gameObjects[715] = GameObjectType::JellyfishNpc;
	m_gameObjects[810] = GameObjectType::JellyfishNpc;
	m_gameObjects[980] = GameObjectType::JellyfishNpc;

	m_gameObjects[1120] = GameObjectType::JellyfishNpc;
	m_gameObjects[1140] = GameObjectType::JellyfishNpc;
	m_gameObjects[1160] = GameObjectType::JellyfishNpc;
	m_gameObjects[1250] = GameObjectType::JellyfishNpc;

	m_interestPointPosX = 500;

	// Pour chaque Portal, ajouter une entré dans ce vecteur qui correspond à la destination
	m_destinations.push_back(Level::Default);
	m_destinations.push_back(Level::DemoJungleA);
}

void			DemoWaterABiome::setup(std::size_t seed)
{
	(void)seed;
}

std::string		DemoWaterABiome::getName()const
{
	return (m_name);
}

Level			DemoWaterABiome::getId()const
{
	return m_id;
}

//TODO:: We'll probably need a setter for mapSize
sf::Vector2u	DemoWaterABiome::getMapSize()
{
	return (m_mapSize);
}

std::size_t		DemoWaterABiome::getMapSeed()
{
	return m_mapSeed;
}

sf::Vector2f	DemoWaterABiome::getMapSizeFloat()
{
	return (sf::Vector2f(m_mapSize.x * Tile::TileSize, m_mapSize.y * Tile::TileSize));
}

sf::Vector2f	DemoWaterABiome::getOctoStartPosition()
{
	return m_octoStartPosition;
}

float			DemoWaterABiome::getTransitionDuration()
{
	return (m_transitionDuration);
}

int				DemoWaterABiome::getInterestPointPosX()
{
	return (m_interestPointPosX);
}

std::map<std::size_t, GameObjectType> const &	DemoWaterABiome::getGameObjects()
{
	return m_gameObjects;
}

Level	DemoWaterABiome::getDestination()
{
	return m_destinations[m_destinationIndex++];
}

float	DemoWaterABiome::getWaterLevel()
{
	return m_waterLevel;
}

sf::Color	DemoWaterABiome::getWaterColor()
{
	return m_waterColor;
}

std::map<std::size_t, std::string> const & DemoWaterABiome::getInstances()
{
	return m_instances;
}

std::vector<ParallaxScrolling::ALayer *> DemoWaterABiome::getLayers()
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
	layer = new GenerativeLayer(getParticleColorGround(), sf::Vector2f(0.6f, 0.2f), mapSize, 12.f, -10, 0.2f, 0.8f, 6.f);
	layer->setBackgroundSurfaceGenerator([](Noise & noise, float x, float y)
		{
			return noise.noise(x * 1.1f, y);
		});
	vector.push_back(layer);
	return vector;
}

Map::MapSurfaceGenerator DemoWaterABiome::getMapSurfaceGenerator()
{
	auto fun = [](float x, float a)
	{
		// n.b.: this Logistic Sigmoid has been normalized.

		float epsilon = 0.0001f;
		float min_param_a = 0.f + epsilon;
		float max_param_a = 1.f - epsilon;
		a = std::max(min_param_a, std::min(max_param_a, a));
		a = (1.f / (1.f - a) - 1.f);

		float A = 1.f / (1.f + std::exp(0 -((x-0.5)*a*2.0)));
		float B = 1.f / (1.f + std::exp(a));
		float C = 1.f / (1.f + std::exp(0-a));
		float y = (A-B)/(C-B);
		return y;
	};
	return [fun, this](Noise & noise, float x, float y)
	{
		static const float startHill1 = 40.f / static_cast<float>(m_mapSize.x);
		static const float middleHill1 = 140.f / static_cast<float>(m_mapSize.x);
		static const float endHill1 = 150.f / static_cast<float>(m_mapSize.x);
		static const float startHill2 = 620.f / static_cast<float>(m_mapSize.x);
		static const float endHill2 = 670.f / static_cast<float>(m_mapSize.x);
		static const float startHill3 = 700.f / static_cast<float>(m_mapSize.x);
		static const float endHill3 = 750.f / static_cast<float>(m_mapSize.x);
		static const float startHill4 = 1000.f / static_cast<float>(m_mapSize.x);
		static const float endHill4 = 1080.f / static_cast<float>(m_mapSize.x);
		static const float offset = 40.f / static_cast<float>(m_mapSize.x);
		static const float offsetSlim = 40.f / static_cast<float>(m_mapSize.x);
		static const float offsetSlim2 = 5.f / static_cast<float>(m_mapSize.x);
		static const float curveValue = 0.980f;

		float n = noise.fBm(x, y, 3, 3.f, 0.6f);
		float bot1 = n / 10.0f + 0.1f;
		float bot2 = n / 10.0f + 0.4f;
		float topHill1 = n / 10.0f - 1.45f;
		float topMiddleHill1 = n / 10.0f - 1.05f;
		float topHill2 = n / 15.0f - 1.6f;
		float topHill3 = n / 15.0f - 2.5f;
		float topHill4 = n / 15.0f - 1.15f;

		if (x > startHill1 - offset && x <= startHill1)
			return octo::cosinusInterpolation(n, topHill1, (x - startHill1 + offset) / offset);
		else if (x <= middleHill1 - offset)
			return topHill1;
		else if (x <= middleHill1)
			return octo::cosinusInterpolation(topHill1, topMiddleHill1, (x - middleHill1 + offset) / offset);
		else if (x <= endHill1)
			return topMiddleHill1;
		else if (x <= endHill1 + offsetSlim2)
			return bot1 - std::abs(bot1 - topMiddleHill1) * fun((endHill1 + offsetSlim2 - x) / offsetSlim2, 0.70f);

		else if (x <= startHill2)
			return bot1;
		else if (x <= startHill2 + offsetSlim)
			return bot1 - std::abs(bot1 - topHill2) * fun(1.f - ((startHill2 + offsetSlim - x)) / offsetSlim, curveValue);
		else if (x <= endHill2)
			return topHill2;
		else if (x <= endHill2 + offsetSlim)
			return bot2 - std::abs(bot2 - topHill2) * fun((endHill2 + offsetSlim - x) / offsetSlim, 0.87f);

		else if (x <= startHill3)
			return bot2;
		else if (x <= startHill3 + offsetSlim)
			return bot2 - std::abs(bot2 - topHill3) * fun(1.f - ((startHill3 + offsetSlim - x)) / offsetSlim, curveValue);
		else if (x <= endHill3)
			return topHill3;
		else if (x <= endHill3 + offsetSlim)
			return bot1 - std::abs(bot1 - topHill3) * fun((endHill3 + offsetSlim - x) / offsetSlim, curveValue);

		else if (x <= startHill4)
			return bot1;
		else if (x <= startHill4 + offsetSlim)
			return bot1 - std::abs(bot1 - topHill4) * fun(1.f - ((startHill4 + offsetSlim - x)) / offsetSlim, curveValue);
		else if (x <= endHill4)
			return topHill4;
		else if (x <= endHill4 + offsetSlim)
			return n - std::abs(n - topHill4) * fun((endHill4 + offsetSlim - x) / offsetSlim, curveValue);

		else
			return n;
	};
}

Map::TileColorGenerator DemoWaterABiome::getTileColorGenerator()
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

sf::Color		DemoWaterABiome::getParticleColorGround()
{
	std::size_t colorIndex = randomInt(0u, 19u);
	return (m_particleColor[colorIndex]);
}

sf::Color		DemoWaterABiome::getTileStartColor()
{
	return (m_tileStartColor);
}

sf::Color		DemoWaterABiome::getTileEndColor()
{
	return (m_tileEndColor);
}

sf::Time		DemoWaterABiome::getDayDuration()
{
	return (m_dayDuration);
}

sf::Time		DemoWaterABiome::getStartDayDuration()
{
	return (m_dayDuration);
}

sf::Color		DemoWaterABiome::getSkyDayColor()
{
	return (m_skyDayColor);
}

sf::Color		DemoWaterABiome::getSkyNightColor()
{
	return (m_skyNightColor);
}

sf::Color		DemoWaterABiome::getNightLightColor()
{
	return (m_nightLightColor);
}

sf::Color		DemoWaterABiome::getSunsetLightColor()
{
	return (m_SunsetLightColor);
}

float			DemoWaterABiome::getWind()
{
	return (m_wind);
}

void			DemoWaterABiome::setWind(float wind)
{
	m_wind = wind;
}

bool			DemoWaterABiome::canCreateRain()
{
	return (m_canCreateRain);
}

std::size_t		DemoWaterABiome::getRainDropPerSecond()
{
	std::size_t value = randomRangeSizeT(m_rainDropPerSecond);
	if (value <= m_rainDropPerSecondMax)
		return (value);
	else
		return (m_rainDropPerSecondMax);
}

sf::Time		DemoWaterABiome::getSunnyTime()
{
	return (randomRangeTime(m_sunnyTime));
}

sf::Time		DemoWaterABiome::getRainingTime()
{
	return (randomRangeTime(m_rainingTime));
}

bool			DemoWaterABiome::canCreateThunder()
{
	return (m_canCreateThunder);
}

float			DemoWaterABiome::getLightningSize()
{
	return (randomRangeFloat(m_lightningSize));
}

bool			DemoWaterABiome::canCreateSnow()
{
	return (m_canCreateSnow);
}

std::size_t		DemoWaterABiome::getRockCount()
{
	return (randomRangeSizeT(m_rockCount));
}

std::size_t		DemoWaterABiome::getTreeCount()
{
	return (randomRangeSizeT(m_treeCount));
}

std::size_t		DemoWaterABiome::getMushroomCount()
{
	return (randomRangeSizeT(m_mushroomCount));
}

std::size_t		DemoWaterABiome::getCrystalCount()
{
	return (randomRangeSizeT(m_crystalCount));
}

std::size_t		DemoWaterABiome::getStarCount()
{
	return (randomRangeSizeT(m_starCount));
}

std::size_t		DemoWaterABiome::getSunCount()
{
	return (randomRangeSizeT(m_sunCount));
}

std::size_t		DemoWaterABiome::getMoonCount()
{
	return (randomRangeSizeT(m_moonCount));
}

std::size_t		DemoWaterABiome::getRainbowCount()
{
	return (randomRangeSizeT(m_rainbowCount));
}

std::size_t		DemoWaterABiome::getCloudCount()
{
	return (randomRangeSizeT(m_cloudCount));
}

std::size_t		DemoWaterABiome::getGroundRockCount()
{
	return (randomRangeSizeT(m_groundRockCount));
}

std::size_t	DemoWaterABiome::getTreeDepth()
{
	return (randomRangeSizeT(m_treeDepth));
}

sf::Vector2f	DemoWaterABiome::getTreeSize()
{
	return (randomRangeVector2f(m_treeSize));
}

sf::Time		DemoWaterABiome::getTreeLifeTime()
{
	return (randomRangeTime(m_treeLifeTime));
}

sf::Color		DemoWaterABiome::getTreeColor()
{
	return (randomColorLeaf(m_treeColor));
}

float			DemoWaterABiome::getTreeAngle()
{
	return (randomRangeFloat(m_treeAngle));
}

bool			DemoWaterABiome::getTreeIsMoving()
{
	return (m_treeIsMoving);
}

float			DemoWaterABiome::getTreeBeatMouvement()
{
	return (m_treeBeatMouvement);
}

bool			DemoWaterABiome::canCreateTree()
{
	return (m_canCreateTree);
}

bool			DemoWaterABiome::canCreateLeaf()
{
	return (m_canCreateLeaf);
}

sf::Vector2f	DemoWaterABiome::getLeafSize()
{
	float tmp = randomFloat(m_leafSize.min.x, m_leafSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DemoWaterABiome::getLeafColor()
{
	return (randomColorLeaf(m_leafColor));
}

std::size_t		DemoWaterABiome::getTreePositionX()
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

sf::Vector2f	DemoWaterABiome::getCrystalSize()
{
	return (randomRangeVector2f(m_crystalSize));
}

std::size_t		DemoWaterABiome::getCrystalPartCount()
{
	return (randomRangeSizeT(m_crystalPartCount));
}

sf::Color		DemoWaterABiome::getCrystalColor()
{
	return (randomColor(m_crystalColor));
}

int				DemoWaterABiome::getCrystalPosX()
{
	int x = static_cast<int>(m_generator.randomPiecewise(m_mapSize.x));
	x += m_interestPointPosX - m_mapSize.x / 2.f;
	if (x > static_cast<int>(m_mapSize.x))
		x -= m_mapSize.x;
	else if (x < 0)
		x += m_mapSize.x;
	return (static_cast<int>(x));
}

bool			DemoWaterABiome::canCreateCrystal()
{
	return (m_canCreateCrystal);
}

sf::Vector2f	DemoWaterABiome::getShineEffectSize()
{
	return (randomRangeVector2f(m_shineEffectSize));
}

sf::Color		DemoWaterABiome::getShineEffectColor()
{
	return (randomColor(m_shineEffectColor));
}

float			DemoWaterABiome::getShineEffectRotateAngle()
{
	return (randomRangeFloat(m_shineEffectRotateAngle));
}

bool			DemoWaterABiome::canCreateShineEffect()
{
	return (m_canCreateShineEffect);
}

sf::Vector2f	DemoWaterABiome::getRockSize()
{
	return (randomRangeVector2f(m_rockSize));
}

std::size_t		DemoWaterABiome::getRockPartCount()
{
	return (randomRangeSizeT(m_rockPartCount));
}

sf::Color		DemoWaterABiome::getRockColor()
{
	return (randomColor(m_rockColor));
}

bool			DemoWaterABiome::canCreateRock()
{
	return (m_canCreateRock);
}

sf::Vector2f	DemoWaterABiome::getMushroomSize()
{
	return (randomRangeVector2f(m_mushroomSize));
}

sf::Color		DemoWaterABiome::getMushroomColor()
{
	return (randomColor(m_mushroomColor));
}

sf::Time		DemoWaterABiome::getMushroomLifeTime()
{
	return (randomRangeTime(m_mushroomLifeTime));
}

bool			DemoWaterABiome::canCreateMushroom()
{
	return (m_canCreateMushroom);
}

sf::Vector2f	DemoWaterABiome::getCloudSize()
{
	return (randomRangeVector2f(m_cloudSize));
}

std::size_t		DemoWaterABiome::getCloudPartCount()
{
	return (randomRangeSizeT(m_cloudPartCount));
}

sf::Time		DemoWaterABiome::getCloudLifeTime()
{
	return (randomRangeTime(m_cloudLifeTime));
}

sf::Color		DemoWaterABiome::getCloudColor()
{
	return (randomColor(m_cloudColor));
}

bool			DemoWaterABiome::canCreateCloud()
{
	return (m_canCreateCloud);
}

sf::Vector2f	DemoWaterABiome::getStarSize()
{
	return (randomRangeVector2f(m_starSize));
}

sf::Color		DemoWaterABiome::getStarColor()
{
	return (randomColor(m_starColor));
}

sf::Time		DemoWaterABiome::getStarLifeTime()
{
	return (randomRangeTime(m_starLifeTime));
}

bool			DemoWaterABiome::canCreateStar()
{
	return (m_canCreateStar);
}

sf::Vector2f 	DemoWaterABiome::getSunSize()
{
	float tmp = randomFloat(m_sunSize.min.x, m_sunSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

std::size_t		DemoWaterABiome::getSunPartCount()
{
	return (randomRangeSizeT(m_sunPartCount));
}

sf::Color		DemoWaterABiome::getSunColor()
{
	if (m_sunColor == sf::Color(255, 255, 255))
		return m_sunColor;
	return (randomColor(m_sunColor));
}

bool			DemoWaterABiome::canCreateSun()
{
	return (m_canCreateSun);
}

sf::Vector2f 	DemoWaterABiome::getMoonSize()
{
	float tmp = randomFloat(m_moonSize.min.x, m_moonSize.max.x);
	return (sf::Vector2f(tmp, tmp));
}

sf::Color		DemoWaterABiome::getMoonColor()
{
	return (randomColor(m_moonColor));
}

sf::Time		DemoWaterABiome::getMoonLifeTime()
{
	return (randomRangeTime(m_moonLifeTime));
}

bool			DemoWaterABiome::canCreateMoon()
{
	return (m_canCreateMoon);
}

float			DemoWaterABiome::getRainbowThickness()
{
	return (randomRangeFloat(m_rainbowThickness));
}

float			DemoWaterABiome::getRainbowPartSize()
{
	return (randomRangeFloat(m_rainbowPartSize));
}

std::size_t		DemoWaterABiome::getRainbowLoopCount()
{
	return (randomRangeSizeT(m_rainbowLoopCount));
}

sf::Time		DemoWaterABiome::getRainbowLifeTime()
{
	return (randomRangeTime(m_rainbowLifeTime));
}

sf::Time		DemoWaterABiome::getRainbowIntervalTime()
{
	return (randomRangeTime(m_rainbowIntervalTime));
}

bool			DemoWaterABiome::canCreateRainbow()
{
	return (m_canCreateRainbow);
}


float			DemoWaterABiome::randomFloat(float min, float max)
{
	return (m_generator.randomFloat(min, max));
}

int				DemoWaterABiome::randomInt(int min, int max)
{
	return (m_generator.randomInt(min, max));
}

bool			DemoWaterABiome::randomBool(float percent)
{
	return (m_generator.randomBool(percent));
}

float			DemoWaterABiome::randomRangeFloat(Range<float> const & range)
{
	return (randomFloat(range.min, range.max));
}

int				DemoWaterABiome::randomRangeSizeT(Range<std::size_t> const & range)
{
	return (randomInt(range.min, range.max));
}

sf::Vector2f	DemoWaterABiome::randomRangeVector2f(Range<sf::Vector2f> const & range)
{
	sf::Vector2f tmp;
	tmp.x = randomFloat(range.min.x, range.max.x);
	tmp.y = randomFloat(range.min.y, range.max.y);
	return tmp;
}

sf::Time		DemoWaterABiome::randomRangeTime(Range<sf::Time> const & range)
{

	return (sf::microseconds(randomInt(range.min.asMicroseconds(), range.max.asMicroseconds())));
}

sf::Color		DemoWaterABiome::randomColor(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-10.f, 10.f);
	tmp.Luminance += m_generator.randomFloat(-10.f, 10.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}

sf::Color		DemoWaterABiome::randomColorLeaf(sf::Color const & color)
{
	//TODO: Take time to make something good here. This is shit
	HSL tmp = TurnToHSL(color);
	tmp.Hue += m_generator.randomFloat(-180.f, 180.f);
	tmp.Luminance += m_generator.randomFloat(-20.f, 0.f);
	sf::Color newColor = tmp.TurnToRGB();
	newColor.a = color.a;
	return (newColor);

}
