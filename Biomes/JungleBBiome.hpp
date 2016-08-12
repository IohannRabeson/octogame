#ifndef JUGNLEBBIOME_HPP
# define JUGNLEBBIOME_HPP

# include "ABiome.hpp"
# include "RandomGenerator.hpp"
# include "HSL.hpp"

# include <cstddef>

# include <SFML/System/Vector2.hpp>


class JungleBBiome : public ABiome
{
template<class T>
struct Range
{
	Range(T minRange, T maxRange) :
		min(minRange),
		max(maxRange)
	{
	}
	T min;
	T max;
};

public:
	JungleBBiome();

	virtual void										setup(std::size_t seed);
	virtual std::string									getName()const;
	virtual Level										getId()const;

	virtual sf::Vector2u								getMapSize();
	virtual sf::Vector2f								getMapSizeFloat();
	virtual sf::Vector2f								getOctoStartPosition();
	virtual std::size_t									getMapSeed();
	virtual float										getTransitionDuration();
	virtual int											getInterestPointPosX();
	virtual std::map<std::size_t, GameObjectType> const & getGameObjects();
	virtual Level										getDestination();
	virtual float										getWaterLevel();
	virtual sf::Color									getWaterColor();
	virtual sf::Color									getSecondWaterColor();
	virtual bool										isDeadlyWater();

	virtual std::map<std::size_t, std::string> const &	getInstances();
	virtual std::vector<ParallaxScrolling::ALayer *>	getLayers();
	virtual Map::MapSurfaceGenerator					getMapSurfaceGenerator();
	virtual Map::TileColorGenerator						getTileColorGenerator();
	virtual sf::Color									getParticleColorGround();
	virtual sf::Color									getTileStartColor();
	virtual sf::Color									getTileEndColor();

	virtual sf::Time									getDayDuration();
	virtual sf::Time									getStartDayDuration();
	virtual sf::Color									getSkyDayColor();
	virtual sf::Color									getSkyNightColor();
	virtual sf::Color									getNightLightColor();
	virtual sf::Color									getSunsetLightColor();

	virtual float										getWind();
	virtual void										setWind(float wind);
	virtual bool										canCreateRain();
	virtual std::size_t									getRainDropPerSecond();
	virtual sf::Time									getSunnyTime();
	virtual sf::Time									getRainingTime();
	virtual bool										canCreateThunder();
	virtual float										getLightningSize();
	virtual bool										canCreateSnow();

	virtual std::size_t									getRockCount();
	virtual std::size_t									getTreeCount();
	virtual std::size_t									getMushroomCount();
	virtual std::size_t									getCrystalCount();
	virtual std::size_t									getCloudCount();
	virtual std::size_t									getSunCount();
	virtual std::size_t									getMoonCount();
	virtual std::size_t									getStarCount();
	virtual std::size_t									getRainbowCount();
	virtual std::size_t									getGroundRockCount();

	virtual sf::Vector2f								getRockSize();
	virtual std::size_t									getRockPartCount();
	virtual sf::Color									getRockColor();

	virtual float										getGrassSizeY();
	virtual float									getGrassSizeX();
	virtual sf::Color									getGrassColor();
	virtual std::size_t									getGrassCount();
	virtual std::size_t									getGrassPosX();
	virtual bool										canCreateRock();

	virtual std::size_t									getTreeDepth();
	virtual sf::Vector2f								getTreeSize();
	virtual sf::Time									getTreeLifeTime();
	virtual sf::Color									getTreeColor();
	virtual float										getTreeAngle();
	virtual bool										getTreeIsMoving();
	virtual float										getTreeBeatMouvement();
	virtual bool										canCreateTree();
	virtual bool										canCreateLeaf();
	virtual sf::Vector2f								getLeafSize();
	virtual sf::Color									getLeafColor();
	virtual std::size_t									getTreePositionX();

	virtual sf::Vector2f								getMushroomSize();
	virtual sf::Color									getMushroomColor();
	virtual sf::Time									getMushroomLifeTime();
	virtual bool										canCreateMushroom();

	virtual sf::Vector2f								getCrystalSize();
	virtual std::size_t									getCrystalPartCount();
	virtual sf::Color									getCrystalColor();
	virtual bool										canCreateCrystal();
	virtual sf::Vector2f								getShineEffectSize();
	virtual sf::Color									getShineEffectColor();
	virtual float										getShineEffectRotateAngle();
	virtual int											getCrystalPosX();
	virtual bool										canCreateShineEffect();

	virtual sf::Vector2f								getCloudSize();
	virtual std::size_t									getCloudPartCount();
	virtual sf::Time									getCloudLifeTime();
	virtual sf::Color									getCloudColor();
	virtual bool										canCreateCloud();

	virtual sf::Vector2f								getStarSize();
	virtual sf::Color									getStarColor();
	virtual sf::Time									getStarLifeTime();
	virtual bool										canCreateStar();

	virtual sf::Vector2f 								getSunSize();
	virtual std::size_t									getSunPartCount();
	virtual sf::Color									getSunColor();
	virtual bool										canCreateSun();

	virtual sf::Vector2f 								getMoonSize();
	virtual sf::Color									getMoonColor();
	virtual sf::Time									getMoonLifeTime();
	virtual bool										canCreateMoon();

	virtual float										getRainbowThickness();
	virtual float										getRainbowPartSize();
	virtual std::size_t									getRainbowLoopCount();
	virtual sf::Time									getRainbowLifeTime();
	virtual sf::Time									getRainbowIntervalTime();
	virtual bool										canCreateRainbow();
	virtual bool									canCreateGrass();
	virtual float									getWaterPersistence() const;
	virtual ABiome::Type									getType() const;

	virtual float										randomFloat(float min, float max);
	virtual int											randomInt(int min, int max);
	virtual bool										randomBool(float percent);

private:
	RandomGenerator										m_generator;
	std::string											m_name;
	Level												m_id;
	std::string											m_seed;

	sf::Vector2u										m_mapSize;
	std::size_t											m_mapSeed;
	sf::Vector2f										m_octoStartPosition;
	float												m_transitionDuration;
	std::size_t											m_interestPointPosX;
	std::vector<sf::Color>								m_particleColor;
	sf::Color											m_tileStartColor;
	sf::Color											m_tileEndColor;
	float												m_waterLevel;
	sf::Color											m_waterColor;
	sf::Color											m_secondWaterColor;

	std::map<std::size_t, std::string>					m_instances;
	std::map<std::size_t, GameObjectType>				m_gameObjects;
	std::vector<Level>									m_destinations;
	std::size_t											m_destinationIndex;

	sf::Time											m_dayDuration;
	sf::Time											m_startDayDuration;
	sf::Color											m_skyDayColor;
	sf::Color											m_skyNightColor;
	sf::Color											m_nightLightColor;
	sf::Color											m_SunsetLightColor;
	float												m_wind;
	Range<std::size_t>									m_rainDropPerSecond;
	static constexpr std::size_t						m_rainDropPerSecondMax = 127u;
	Range<sf::Time>										m_sunnyTime;
	Range<sf::Time>										m_rainingTime;
	Range<float>										m_lightningSize;

	Range<std::size_t>									m_rockCount;
	Range<std::size_t>									m_treeCount;
	Range<std::size_t>									m_mushroomCount;
	Range<std::size_t>									m_crystalCount;
	Range<std::size_t>									m_starCount;
	Range<std::size_t>									m_sunCount;
	Range<std::size_t>									m_moonCount;
	Range<std::size_t>									m_rainbowCount;
	Range<std::size_t>									m_cloudCount;
	Range<std::size_t>									m_groundRockCount;

	bool												m_canCreateRain;
	bool												m_canCreateThunder;
	bool												m_canCreateSnow;
	bool												m_canCreateRock;
	bool												m_canCreateTree;
	bool												m_canCreateLeaf;
	bool												m_treeIsMoving;
	bool												m_canCreateMushroom;
	bool												m_canCreateCrystal;
	bool												m_canCreateShineEffect;
	bool												m_canCreateCloud;
	bool												m_canCreateStar;
	bool												m_canCreateSun;
	bool												m_canCreateMoon;
	bool												m_canCreateRainbow;
	bool												m_canCreateGrass;
	float												m_waterPersistence;
	ABiome::Type										m_type;

	Range<sf::Vector2f>									m_rockSize;
	Range<std::size_t>									m_rockPartCount;
	sf::Color											m_rockColor;

	Range<float>										m_grassSizeY;
	Range<float>											m_grassSizeX;
	sf::Color											m_grassColor;
	std::size_t											m_grassCount;
	std::size_t											m_grassIndex;

	Range<std::size_t>									m_treeDepth;
	Range<sf::Vector2f>									m_treeSize;
	Range<sf::Time>										m_treeLifeTime;
	sf::Color											m_treeColor;
	Range<float>										m_treeAngle;
	float												m_treeBeatMouvement;
	Range<sf::Vector2f>									m_leafSize;
	sf::Color											m_leafColor;

	Range<sf::Vector2f>									m_mushroomSize;
	sf::Color											m_mushroomColor;
	Range<sf::Time>										m_mushroomLifeTime;

	Range<sf::Vector2f>									m_crystalSize;
	Range<std::size_t>									m_crystalPartCount;
	sf::Color											m_crystalColor;
	Range<sf::Vector2f>									m_shineEffectSize;
	sf::Color											m_shineEffectColor;
	Range<float>										m_shineEffectRotateAngle;

	Range<sf::Vector2f>									m_cloudSize;
	Range<std::size_t>									m_cloudPartCount;
	Range<sf::Time>										m_cloudLifeTime;
	sf::Color											m_cloudColor;

	Range<sf::Vector2f>									m_starSize;
	sf::Color											m_starColor;
	Range<sf::Time>										m_starLifeTime;

	Range<sf::Vector2f>									m_sunSize;
	Range<std::size_t>									m_sunPartCount;
	sf::Color											m_sunColor;

	Range<sf::Vector2f>									m_moonSize;
	sf::Color											m_moonColor;
	Range<sf::Time>										m_moonLifeTime;

	Range<float>										m_rainbowThickness;
	Range<float>										m_rainbowPartSize;
	Range<std::size_t>									m_rainbowLoopCount;
	Range<sf::Time>										m_rainbowLifeTime;
	Range<sf::Time>										m_rainbowIntervalTime;

	float												randomRangeFloat(Range<float> const & range);
	int													randomRangeSizeT(Range<std::size_t> const & range);
	sf::Vector2f										randomRangeVector2f(Range<sf::Vector2f> const & range);
	sf::Time											randomRangeTime(Range<sf::Time> const & range);
	sf::Color											randomColor(sf::Color const & color);
};

#endif
