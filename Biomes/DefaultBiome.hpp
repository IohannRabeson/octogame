#ifndef DEFAULTBIOME_HPP
# define DEFAULTBIOME_HPP

# include "ABiome.hpp"
# include "RandomGenerator.hpp"

# include <cstddef>

# include <SFML/System/Vector2.hpp>


class DefaultBiome : public ABiome
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
	DefaultBiome();

	virtual void			setup(std::size_t seed);
	virtual std::string		getName()const;

	virtual sf::Vector2u	getMapSize();
	virtual sf::Vector2f	getMapSizeFloat();
	virtual float			getTransitionDuration();
	virtual int				getBossInstancePosX();

	virtual sf::Time		getDayDuration();
	virtual sf::Time		getNightDuration();
	virtual float			getWind();

	virtual std::size_t		getGroundDecorsCount();
	virtual std::size_t		getCrystalCount();
	virtual std::size_t		getSunCount();
	virtual std::size_t		getMoonCount();
	virtual std::size_t		getStarCount();
	virtual std::size_t		getRainbowCount();
	virtual std::size_t		getCloudCount();

	virtual std::size_t		getTreeDepth();
	virtual sf::Vector2f	getTreeSize();
	virtual sf::Time		getTreeLifeTime();
	virtual sf::Color		getTreeColor();
	virtual float			getTreeAngle();
	virtual bool			getTreeIsMoving();
	virtual bool			canCreateTree();
	virtual bool			canCreateLeaf();
	virtual sf::Vector2f	getLeafSize();
	virtual sf::Color		getLeafColor();

	virtual sf::Vector2f	getCrystalSize();
	virtual std::size_t		getCrystalPartCount();
	virtual sf::Color		getCrystalColor();
	virtual bool			canCreateCrystal();
	virtual sf::Vector2f	getShineEffectSize();
	virtual sf::Color		getShineEffectColor();
	virtual float			getShineEffectRotateAngle();
	virtual int				getCrystalPosX();
	virtual bool			canCreateShineEffect();

	virtual sf::Vector2f	getRockSize();
	virtual std::size_t		getRockPartCount();
	virtual sf::Color		getRockColor();
	virtual bool			canCreateRock();

	virtual sf::Vector2f	getCloudSize();
	virtual std::size_t		getCloudPartCount();
	virtual sf::Time		getCloudLifeTime();
	virtual sf::Color		getCloudColor();
	virtual bool			canCreateCloud();

	virtual sf::Vector2f	getStarSize();
	virtual sf::Color		getStarColor();
	virtual sf::Time		getStarLifeTime();
	virtual bool			canCreateStar();

	virtual sf::Vector2f 	getSunSize();
	virtual std::size_t		getSunPartCount();
	virtual sf::Color		getSunColor();
	virtual bool			canCreateSun();

	virtual sf::Vector2f 	getMoonSize();
	virtual sf::Color		getMoonColor();
	virtual sf::Time		getMoonLifeTime();
	virtual bool			canCreateMoon();

	virtual float			getRainbowThickness();
	virtual float			getRainbowPartSize();
	virtual std::size_t		getRainbowLoopCount();
	virtual sf::Time		getRainbowGrowTime();
	virtual bool			canCreateRainbow();

	virtual float			randomFloat(float min, float max);
	virtual int				randomInt(int min, int max);
	virtual bool			randomBool(float percent);

private:
	RandomGenerator		m_generator;
	std::string			m_name;

	sf::Vector2u		m_mapSize;
	float				m_transitionDuration;
	std::size_t			m_bossInstancePosX;

	sf::Time			m_dayDuration;
	sf::Time			m_nightDuration;
	Range<float>		m_wind;

	Range<std::size_t>	m_groundDecorsCount;
	Range<std::size_t>	m_crystalCount;
	Range<std::size_t>	m_starCount;
	Range<std::size_t>	m_sunCount;
	Range<std::size_t>	m_moonCount;
	Range<std::size_t>	m_rainbowCount;
	Range<std::size_t>	m_cloudCount;

	bool				m_canCreateTree;
	bool				m_canCreateLeaf;
	bool				m_treeIsMoving;
	bool				m_canCreateCrystal;
	bool				m_canCreateShineEffect;
	bool				m_canCreateRock;
	bool				m_canCreateCloud;
	bool				m_canCreateStar;
	bool				m_canCreateSun;
	bool				m_canCreateMoon;
	bool				m_canCreateRainbow;

	Range<std::size_t>	m_treeDepth;
	Range<sf::Vector2f>	m_treeSize;
	Range<sf::Time>		m_treeLifeTime;
	sf::Color			m_treeColor;
	Range<float>		m_treeAngle;
	Range<sf::Vector2f>	m_leafSize;
	sf::Color			m_leafColor;

	Range<sf::Vector2f>	m_crystalSize;
	Range<std::size_t>	m_crystalPartCount;
	sf::Color			m_crystalColor;
	Range<sf::Vector2f>	m_shineEffectSize;
	sf::Color			m_shineEffectColor;
	Range<float>		m_shineEffectRotateAngle;

	Range<sf::Vector2f>	m_rockSize;
	Range<std::size_t>	m_rockPartCount;
	sf::Color			m_rockColor;

	Range<sf::Vector2f>	m_cloudSize;
	Range<std::size_t>	m_cloudPartCount;
	Range<sf::Time>		m_cloudLifeTime;
	sf::Color			m_cloudColor;

	Range<sf::Vector2f>	m_starSize;
	sf::Color			m_starColor;
	Range<sf::Time>		m_starLifeTime;

	Range<sf::Vector2f>	m_sunSize;
	Range<std::size_t>	m_sunPartCount;
	sf::Color			m_sunColor;

	Range<sf::Vector2f>	m_moonSize;
	sf::Color			m_moonColor;
	Range<sf::Time>		m_moonLifeTime;

	Range<float>		m_rainbowThickness;
	Range<float>		m_rainbowPartSize;
	Range<std::size_t>	m_rainbowLoopCount;
	Range<sf::Time>		m_rainbowGrowTime;

	float				randomRangeFloat(Range<float> const & range);
	int					randomRangeInt(Range<std::size_t> const & range);
	sf::Vector2f		randomRangeVector2f(Range<sf::Vector2f> const & range);
	sf::Time			randomRangeTime(Range<sf::Time> const & range);
};

#endif
