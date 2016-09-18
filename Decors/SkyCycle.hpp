#ifndef SKYCYCLE_HPP
# define SKYCYCLE_HPP

# include <AudioManager.hpp>
# include <SFML/Graphics/Color.hpp>
# include <SFML/System/Time.hpp>
# include <cstddef>
# include <random>

# include "RandomGenerator.hpp"

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
	std::size_t	getDropPerSecond(void) const;
	float		getThunderValue(void) const;

	bool		isDay(void) const;
	bool		isNight(void) const;

	void		setDayDuration(sf::Time);
	void		setNightDuration(sf::Time);

	void		update(sf::Time frameTime, ABiome & biome);
	void		setup(ABiome & biome);

private:
	void		computeDayNight(sf::Time frameTime);
	void		newThunderCycle(ABiome & biome);
	void		newDropCycle(ABiome & biome);
	void		computeThunder(sf::Time frameTime,
							ABiome & biome);
	void		computeDrop(sf::Time frameTime,
							ABiome & biome);
	void		playSound(ABiome & biome);

	sf::Time							m_timer;
	sf::Time							m_timerMax;
	sf::Time							m_timerStart;
	bool								m_isDay;
	bool								m_isMidDay;
	sf::Time							m_timerDay;
	sf::Time							m_timerDayMax;
	bool								m_isMidNight;
	sf::Time							m_timerNight;
	sf::Time							m_timerNightMax;

	sf::Color							m_colorUpDay;
	sf::Color							m_colorUpNight;
	sf::Color							m_colorDownDay;
	sf::Color							m_colorDownNight;
	sf::Color							m_colorSkyUp;
	sf::Color							m_colorSkyDown;

	float								m_weather;
	std::size_t							m_dropPerSecond;
	sf::Time							m_sunnyTimer;
	sf::Time							m_sunnyTimerMax;
	sf::Time							m_dropTimer;
	sf::Time							m_dropTimerMax;
	bool								m_dropAppear;

	RandomGenerator						m_generator;
	float								m_thunder;
	sf::Time							m_thunderTimerStart;
	sf::Time							m_thunderTimer;
	sf::Time							m_thunderTimerMax;
	std::size_t							m_thunderState;

	bool								m_boolThunder;
	octo::AudioManager::SoundPtr		m_rainSound;
};

#endif
