#ifndef GAMECLOCK_HPP
# define GAMECLOCK_HPP

#include <SFML/System/Time.hpp>

class ABiome;

class GameClock
{
public:
	GameClock(void);
	~GameClock(void) = default;

	float		getDayValue(void) const;
	float		getNightValue(void) const;
	float		getCycleValue(void) const;

	bool		isDay(void) const;
	bool		isNight(void) const;

	void		setDayDuration(sf::Time);
	void		setNightDuration(sf::Time);

	void		update(sf::Time frameTime);
	void		setup(ABiome & biome);

private:
	sf::Time	m_timer;
	sf::Time	m_timerMax;
	bool		m_isDay;
	bool		m_isMidDay;
	sf::Time	m_timerDay;
	sf::Time	m_timerDayMax;
	bool		m_isMidNight;
	sf::Time	m_timerNight;
	sf::Time	m_timerNightMax;
};

#endif
