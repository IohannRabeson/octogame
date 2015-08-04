#include "SkyCycle.hpp"
#include "ABiome.hpp"
#include <Interpolations.hpp>

SkyCycle::SkyCycle(void) :
	m_isDay(true),
	m_isMidDay(false),
	m_isMidNight(false),
	m_weather(0.f),
	m_rainDropPerSecond(0u),
	m_rainAppear(true),
	m_thunder(0.f),
	m_thunderState(0u)
{
}

float SkyCycle::getDayValue(void) const
{
	return m_timerDay / m_timerDayMax;
}

float SkyCycle::getNightValue(void) const
{
	return m_timerNight / m_timerNightMax;
}

float SkyCycle::getCycleValue(void) const
{
	return m_timer / m_timerMax;
}

float SkyCycle::getWeatherValue(void) const
{
	return m_weather;
}

std::size_t SkyCycle::getDropPerSecond(void) const
{
	return m_rainDropPerSecond;
}

float SkyCycle::getThunderValue(void) const
{
	return m_thunder;
}

sf::Color SkyCycle::getSkyColorUp(void) const
{
	return m_colorSkyUp;
}

sf::Color SkyCycle::getSkyColorDown(void) const
{
	return m_colorSkyDown;
}

bool SkyCycle::isDay(void) const
{
	return m_isDay;
}

bool SkyCycle::isNight(void) const
{
	if (m_isDay)
		return false;
	else
		return true;
}

void SkyCycle::setup(ABiome & biome)
{
	m_timerMax = biome.getDayDuration();
	m_timerDayMax = m_timerMax / 4.f;
	m_timerNightMax = m_timerMax / 4.f;
	newRainCycle(biome);
	newThunderCycle(biome);

	m_colorUpDay = biome.getSkyDayColor();
	m_colorUpNight = biome.getSkyNightColor();
	m_colorDownDay = sf::Color(255, 255, 255);
	m_colorDownNight = sf::Color(50, 50, 50);
}

void SkyCycle::computeDayNight(sf::Time frameTime)
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

void SkyCycle::newThunderCycle(ABiome & biome)
{
	m_thunder = 0.f;
	m_thunderState = 0u;
	m_thunderTimer = sf::Time::Zero;
	m_thunderTimerMax = sf::seconds(0.5f);
	m_thunderTimerStart = sf::seconds(biome.randomFloat(0.f, m_rainingTimerMax.asSeconds()));
}

void SkyCycle::newRainCycle(ABiome & biome)
{
	m_weather = 0.f;
	m_sunnyTimer = sf::Time::Zero;
	m_sunnyTimerMax = biome.getSunnyTime();
	m_rainingTimer = sf::Time::Zero;
	m_rainingTimerMax = biome.getRainingTime() / 2.f;
	m_rainAppear = true;
	m_rainDropPerSecond = biome.getRainDropPerSecond();
}

void SkyCycle::computeThunder(sf::Time frameTime, ABiome & biome)
{
	if (m_thunderState == 0u)
	{
		m_thunderTimer += frameTime;
		if (m_thunderTimer >= m_thunderTimerStart)
		{
			m_thunderState = 1u;
			m_thunderTimer = sf::Time::Zero;
		}
	}
	if (m_thunderState == 1u)
	{
		m_thunderTimer += frameTime;
		if (m_thunderTimer >= m_thunderTimerMax)
		{
			m_thunderTimer = m_thunderTimerMax;
			m_thunderState = 2u;
		}
	}
	else if (m_thunderState == 2u)
	{
		m_thunderTimer -= frameTime;
		if (m_thunderTimer < m_thunderTimerMax / 2.f)
			m_thunderState = 3u;
	}
	else if (m_thunderState == 3u)
	{
		m_thunderTimer += frameTime;
		if (m_thunderTimer >= m_thunderTimerMax)
		{
			m_thunderTimer = m_thunderTimerMax;
			m_thunderState = 4u;
		}
	}
	else if (m_thunderState == 4u)
	{
		m_thunderTimer -= frameTime;
		if (m_thunderTimer <= sf::Time::Zero)
		{
			m_thunderTimer = sf::Time::Zero;
			m_thunderState = 5u;
		}
	}
	else
		newThunderCycle(biome);
	m_thunder = m_thunderTimer / m_thunderTimerMax;
}

void SkyCycle::computeRain(sf::Time frameTime, ABiome & biome)
{
	if (m_sunnyTimer <= m_sunnyTimerMax)
		m_sunnyTimer += frameTime;
	else if (m_rainingTimer <= m_rainingTimerMax && m_rainAppear)
	{
		m_rainingTimer += frameTime;
		m_weather = m_rainingTimer / m_rainingTimerMax * m_rainDropPerSecond;
	}
	else if (m_rainingTimer >= sf::Time::Zero)
	{
		m_rainAppear = false;
		m_weather = m_rainingTimer / m_rainingTimerMax * m_rainDropPerSecond;
		m_rainingTimer -= frameTime;
	}
	else
		newRainCycle(biome);
}

void SkyCycle::update(sf::Time frameTime, ABiome & biome)
{
	computeDayNight(frameTime);
	if (biome.canCreateRain())
		computeRain(frameTime, biome);
	if ((m_weather || m_thunderTimer != sf::Time::Zero) && biome.canCreateThunder())
		computeThunder(frameTime, biome);
	float interpolateValue = getNightValue() * 2.f;
	if (interpolateValue > 1.f)
		interpolateValue = 1.f;
	m_colorSkyDown = octo::linearInterpolation(m_colorDownDay, m_colorDownNight, interpolateValue);
	m_colorSkyUp = octo::linearInterpolation(m_colorUpDay, m_colorUpNight, interpolateValue);
}
