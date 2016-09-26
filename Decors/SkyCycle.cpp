#include "SkyCycle.hpp"
#include "ABiome.hpp"
#include <Interpolations.hpp>
#include <Application.hpp>
#include <ResourceManager.hpp>
#include "ResourceDefinitions.hpp"

SkyCycle::SkyCycle(void) :
	m_isDay(true),
	m_isMidDay(false),
	m_isMidNight(false),
	m_weather(0.f),
	m_dropPerSecond(0u),
	m_dropAppear(true),
	m_generator("random"),
	m_thunder(0.f),
	m_thunderState(0u),
	m_boolThunder(false)
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
	return m_dropPerSecond;
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

	if (biome.getStartDayDuration() == sf::Time::Zero)
		m_timerStart = sf::seconds(m_generator.randomFloat(0.f, m_timerMax.asSeconds()));
	else
	{
		sf::Time x = sf::Time::Zero;

		m_timerStart = biome.getStartDayDuration();
		while (x < m_timerStart)
		{
			computeDayNight(sf::seconds(0.1f));
			x += sf::seconds(0.1f);
		}
	}

	newDropCycle(biome);
	newThunderCycle(biome);

	m_colorUpDay = biome.getSkyDayColor();
	m_colorUpNight = biome.getSkyNightColor();
	m_colorDownDay = sf::Color(255, 255, 255);
	m_colorDownNight = sf::Color(50, 50, 50);

	octo::ResourceManager const & resource = octo::Application::getResourceManager();
	if (biome.canCreateRain())
	{
		m_rainSound = octo::Application::getAudioManager().playSound(resource.getSound(RAIN_STEREO_OGG), 0.f, 1.f);
		m_rainSound->setLoop(true);
	}
}

void SkyCycle::computeDayNight(sf::Time frameTime)
{
	sf::Time const quarterTimerCycle = m_timerMax / 4.f;
	float speedCoef = m_timerMax.asSeconds() / 100.f * m_timerStart.asSeconds();

	if (speedCoef >= 1.f)
		frameTime *= speedCoef;
	m_timerStart -= frameTime;
	m_timer += frameTime;
	if (m_timer >= m_timerMax)
		m_timer = sf::Time::Zero;
	if (m_timer < quarterTimerCycle * 2.f)
	{
		m_isDay = true;
		if (m_timer < quarterTimerCycle)
		{
			m_isMidDay = false;
			m_timerDay += frameTime;
			if (m_timerDay >= m_timerDayMax)
				m_timerDay = m_timerDayMax;
		}
		else
		{
			m_isMidDay = true;
			m_timerDay -= frameTime;
			if (m_timerDay <= sf::Time::Zero)
				m_timerDay = sf::Time::Zero;
		}
	}
	else
	{
		m_isDay = false;
		if (m_timer < quarterTimerCycle * 3.f)
		{
			m_isMidNight = false;
			m_timerNight += frameTime;
			if (m_timerNight >= m_timerNightMax)
				m_timerNight = m_timerNightMax;
		}
		else
		{
			m_isMidNight = true;
			m_timerNight -= frameTime;
			if (m_timerNight <= sf::Time::Zero)
				m_timerNight = sf::Time::Zero;
		}
	}
}

void SkyCycle::newThunderCycle(ABiome & biome)
{
	m_thunder = 0.f;
	m_thunderState = 0u;
	m_thunderTimer = sf::Time::Zero;
	m_thunderTimerMax = sf::seconds(0.5f);
	m_thunderTimerStart = sf::seconds(biome.randomFloat(0.f, m_dropTimerMax.asSeconds()));
	m_boolThunder = true;
}

void SkyCycle::newDropCycle(ABiome & biome)
{
	m_weather = 0.f;
	m_sunnyTimer = sf::Time::Zero;
	m_sunnyTimerMax = biome.getSunnyTime();
	m_dropTimer = sf::Time::Zero;
	m_dropTimerMax = biome.getRainingTime() / 2.f;
	m_dropAppear = true;
	m_dropPerSecond = biome.getRainDropPerSecond();
}

void SkyCycle::computeThunder(sf::Time frameTime, ABiome & biome)
{
	//TODO: Create a potion: If the thunderState == 0u is in the switch we have a big storm
	// Find a proper way to do it when the potion system will be ther
	if (m_thunderState == 0u)
	{
		m_thunderTimer += frameTime;
		if (m_thunderTimer >= m_thunderTimerStart)
		{
			m_thunderState = 1u;
			m_thunderTimer = sf::Time::Zero;
		}
	}
	switch (m_thunderState)
	{
		case 1u:
		{
			m_thunderTimer += frameTime;
			if (m_thunderTimer >= m_thunderTimerMax)
			{
				m_thunderTimer = m_thunderTimerMax;
				m_thunderState = 2u;
			}
			break;
		}
		case 2u:
		{
			m_thunderTimer -= frameTime;
			if (m_thunderTimer < m_thunderTimerMax / 2.f)
				m_thunderState = 3u;
			break;
		}
		case 3u:
		{
			m_thunderTimer += frameTime;
			if (m_thunderTimer >= m_thunderTimerMax)
			{
				m_thunderTimer = m_thunderTimerMax;
				m_thunderState = 4u;
			}
			break;
		}
		case 4u:
		{
			m_thunderTimer -= frameTime;
			if (m_thunderTimer <= sf::Time::Zero)
			{
				m_thunderTimer = sf::Time::Zero;
				m_thunderState = 5u;
			}
			break;
		}
		default:
		{
			newThunderCycle(biome);
			break;
		}
	}
	m_thunder = m_thunderTimer / m_thunderTimerMax;
}

void SkyCycle::computeDrop(sf::Time frameTime, ABiome & biome)
{
	if (m_sunnyTimer <= m_sunnyTimerMax)
		m_sunnyTimer += frameTime;
	else if (m_dropTimer <= m_dropTimerMax && m_dropAppear)
	{
		m_dropTimer += frameTime;
		m_weather = m_dropTimer / m_dropTimerMax * m_dropPerSecond;
	}
	else if (m_dropTimer >= sf::Time::Zero)
	{
		m_dropAppear = false;
		m_weather = m_dropTimer / m_dropTimerMax * m_dropPerSecond;
		m_dropTimer -= frameTime;
	}
	else
		newDropCycle(biome);
}

void SkyCycle::playSound(ABiome & biome)
{
	if (m_boolThunder && m_thunder)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();
		std::size_t thunderType = m_generator.randomInt(0u, 2u);
		if (thunderType == 0u)
			audio.playSound(resources.getSound(THUNDER1_STEREO_OGG), 0.4f, biome.randomFloat(0.5f, 1.5f));
		else if (thunderType == 1u)
			audio.playSound(resources.getSound(THUNDER2_STEREO_OGG), 0.4f, biome.randomFloat(0.5f, 1.5f));
		else
			audio.playSound(resources.getSound(THUNDER3_STEREO_OGG), 0.4f, biome.randomFloat(0.5f, 1.5f));
		m_boolThunder = false;
	}
}

void SkyCycle::update(sf::Time frameTime, ABiome & biome)
{
	computeDayNight(frameTime);
	if (biome.canCreateRain() || biome.canCreateSnow())
		computeDrop(frameTime, biome);
	if (m_rainSound && biome.canCreateRain() && !biome.canCreateSnow())
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		float volume = m_weather / m_dropTimerMax.asSeconds();
		if (volume > 1.f)
			volume = 1.f;
		m_rainSound->setVolume(volume * audio.getSoundVolume() * 0.1f);
	}
	if ((m_weather || m_thunderTimer != sf::Time::Zero) && biome.canCreateThunder())
	{
		computeThunder(frameTime, biome);
		playSound(biome);
	}
	float interpolateValue = getNightValue() * 2.f;
	if (interpolateValue > 1.f)
		interpolateValue = 1.f;
	m_colorSkyDown = octo::linearInterpolation(m_colorDownDay, m_colorUpNight, interpolateValue);
	m_colorSkyUp = octo::linearInterpolation(m_colorUpDay, m_colorDownNight, interpolateValue);
}
