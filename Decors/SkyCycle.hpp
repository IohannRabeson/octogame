#ifndef SKYCYCLE_HPP
# define SKYCYCLE_HPP

# include <SFML/Graphics/Color.hpp>
# include <SFML/System/Time.hpp>
# include <cstddef>

class ABiome;

class SkyCycle
{
public:
	SkyCycle(void);
	~SkyCycle(void) = default;

	float		getDayValue(void) const;
	float		getNightValue(void) const;
	float		getCycleValue(void) const;
	sf::Color	getSkyColorDown(void) const;
	sf::Color	getSkyColorUp(void) const;

	float		getWeatherValue(void) const;

	bool		isDay(void) const;
	bool		isNight(void) const;

	void		setDayDuration(sf::Time);
	void		setNightDuration(sf::Time);

	void		update(sf::Time frameTime, ABiome & biome);
	void		setup(ABiome & biome);

private:
	void		computeDayNight(sf::Time frameTime);
	void		newRainCycle(ABiome & biome);
	void		computeRain(sf::Time frameTime,
							ABiome & biome);

	sf::Time	m_timer;
	sf::Time	m_timerMax;
	bool		m_isDay;
	bool		m_isMidDay;
	sf::Time	m_timerDay;
	sf::Time	m_timerDayMax;
	bool		m_isMidNight;
	sf::Time	m_timerNight;
	sf::Time	m_timerNightMax;

	float		m_weather;
	std::size_t	m_rainDropPerSecond;
	sf::Time	m_sunnyTimer;
	sf::Time	m_sunnyTimerMax;
	sf::Time	m_rainingTimer;
	sf::Time	m_rainingTimerMax;
	bool		m_rainAppear;

	sf::Color	m_colorUpDay;
	sf::Color	m_colorUpNight;
	sf::Color	m_colorDownDay;
	sf::Color	m_colorDownNight;
	sf::Color	m_colorSkyUp;
	sf::Color	m_colorSkyDown;
};

#endif
