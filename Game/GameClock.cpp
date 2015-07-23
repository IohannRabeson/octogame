#include "GameClock.hpp"
#include "ABiome.hpp"

GameClock::GameClock(void) :
	m_isDay(true),
	m_isMidDay(false),
	m_isMidNight(false)
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
	return m_timer / m_timerMax;
}

bool GameClock::isDay(void) const
{
	return m_isDay;
}

bool GameClock::isNight(void) const
{
	if (m_isDay)
		return false;
	else
		return true;
}

void GameClock::setup(ABiome & biome)
{
	m_timerMax = biome.getDayDuration();
	m_timerDayMax = m_timerMax / 4.f;
	m_timerNightMax = m_timerMax / 4.f;
}

void GameClock::update(sf::Time frameTime)
{
	m_timer += frameTime;
	if (m_timer >= m_timerMax)
		m_timer = sf::Time::Zero;
	if (m_isDay)
	{
		if (m_isMidDay == false)
		{
			m_timerDay += frameTime;
			if (m_timerDay >= m_timerDayMax)
				m_isMidDay = true;
		}
		else
		{
			m_timerDay -= frameTime;
			if (m_timerDay <= sf::Time::Zero)
			{
				m_timerDay = sf::Time::Zero;
				m_isMidDay = false;
				m_isDay = false;
			}
		}
	}
	else
	{
		if (m_isMidNight == false)
		{
			m_timerNight += frameTime;
			if (m_timerNight >= m_timerNightMax)
				m_isMidNight = true;
		}
		else
		{
			m_timerNight -= frameTime;
			if (m_timerNight <= sf::Time::Zero)
			{
				m_timerNight = sf::Time::Zero;
				m_isMidNight = false;
				m_isDay = true;
			}
		}
	}
}
