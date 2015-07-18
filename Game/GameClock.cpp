#include "GameClock.hpp"
#include "ABiome.hpp"

GameClock::GameClock(void) :
	m_isDay(true),
	m_isNight(false)
{
}

float GameClock::getDayValue(void) const
{
	return m_timerDay / m_timerDayMax;
}

float GameClock::getNightValue(void) const
{
	return m_timerNight / m_timerNightMax;
}

float GameClock::getCycleValue(void) const
{
	if (m_isDay)
		return (m_timerDay / (m_timerDayMax + m_timerNightMax));
	else if (m_isNight)
		return ((m_timerDayMax + m_timerNight) / (m_timerDayMax + m_timerNightMax));
	return 0.f;
}

bool GameClock::isDay(void) const
{
	return m_isDay;
}

bool GameClock::isNight(void) const
{
	return m_isNight;
}

void GameClock::setup(ABiome & biome)
{
	m_timerDayMax = biome.getDayDuration();
	m_timerNightMax = biome.getNightDuration();
}

void GameClock::update(sf::Time frameTime)
{
	if (m_isDay)
	{
		m_timerDay += frameTime;
		if (m_timerDay >= m_timerDayMax)
		{
			m_timerDay = sf::Time::Zero;
			m_isNight = true;
			m_isDay = false;
		}
	}
	else if (m_isNight)
	{
		m_timerNight += frameTime;
		if (m_timerNight >= m_timerNightMax)
		{
			m_timerNight = sf::Time::Zero;
			m_isNight = false;
			m_isDay = true;
		}
	}
}
