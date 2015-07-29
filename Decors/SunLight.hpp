#ifndef SUNLIGHT_HPP
# define SUNLIGHT_HPP

# include "ADecor.hpp"
# include "DecorAnimator.hpp"
# include <SFML/Graphics/Color.hpp>

class GameClock;

class SunLight : public ADecor
{
public:
	SunLight(void);
	SunLight(GameClock * clock);
	virtual ~SunLight(void) = default;

	virtual void setup(ABiome& biome);
	virtual void update(sf::Time frameTime,
						octo::VertexBuilder& builder,
						ABiome& biome);

private:
	void createSunLight(sf::Vector2f const & size,
						sf::Vector2f const & origin,
						octo::VertexBuilder& builder);

	void createBicolorQuad(sf::Vector2f const & upLeft,
							sf::Vector2f const & upRight,
							sf::Vector2f const & downRight,
							sf::Vector2f const & downLeft,
							sf::Color const & colorUp,
							sf::Color const & colorDown,
							octo::VertexBuilder & builder);

	void computeDayColorValue(sf::Time frameTime, ABiome & biome);

	sf::Vector2f	m_size;
	sf::Vector2f	m_dayPos;
	sf::Vector2f	m_nightPos;
	sf::Vector2f	m_sunsetPos;
	sf::Color		m_colorNight;
	sf::Color		m_colorSunset;
	sf::Color		m_colorDay;
	sf::Color		m_colorDayRaining;
	sf::Vector2f	m_cameraSize;

	sf::Time		m_timerRain;
	sf::Time		m_timerRainMax;

	GameClock *		m_clock;
};

#endif
