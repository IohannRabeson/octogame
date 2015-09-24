#include "GroundTransformNanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

GroundTransformNanoRobot::GroundTransformNanoRobot(void) :
	NanoRobot(sf::Vector2f(30 * 16.f, 800.f), NANO_GROUND_TRANSFORM_OSS, 4, 9854),
	m_textTimer(sf::Time::Zero),
	m_textTimerMax(sf::seconds(5.f)),
	m_canSpeak(false),
	m_firstSpawn(false)
{
	setup(this);
	if (Progress::getInstance().getNanoRobotCount() == 0u)
		m_firstSpawn = true;
}

void GroundTransformNanoRobot::update(sf::Time frameTime)
{
	NanoRobot::update(frameTime);

	if (m_firstSpawn)
	{
		m_textTimer += frameTime;
		if (m_textTimer > m_textTimerMax)
		{
			m_textTimer = sf::Time::Zero;
			m_firstSpawn = false;
		}
		m_canSpeak = true;
		setTextIndex(0u);
	}
	else if (!Progress::getInstance().getWalk())
	{
		m_textTimer += frameTime;
		if (m_textTimer > m_textTimerMax)
		{
			m_textTimer = sf::Time::Zero;
			m_canSpeak = true;
		}
		setTextIndex(1u);
	}
	else if (!Progress::getInstance().getMoveMap())
	{
		m_textTimer += frameTime;
		if (m_textTimer > m_textTimerMax)
		{
			m_textTimer = sf::Time::Zero;
			m_canSpeak = true;
		}
		setTextIndex(2u);
	}
	else
	{
		m_textTimer = sf::Time::Zero;
		m_canSpeak = false;
	}
}

void GroundTransformNanoRobot::drawText(sf::RenderTarget& render, sf::RenderStates states) const
{
	if (isTravelling())
		return;
	if (m_canSpeak)
	{
		getCurrentText()->draw(render, states);
	}
}
