#include "GroundTransformNanoRobot.hpp"
#include "ResourceDefinitions.hpp"
#include "Progress.hpp"

GroundTransformNanoRobot::GroundTransformNanoRobot(void) :
	NanoRobot(sf::Vector2f(30 * 16.f, 800.f), NANO_GROUND_TRANSFORM_OSS, 4, 9854, sf::Vector2f(0.f, -26.f)),
	m_textTimer(sf::Time::Zero),
	m_textTimerMax(sf::seconds(3.f)),
	m_canSpeak(false),
	m_talkaboutshit(true),
	m_state(Walk)
{
	setup(this);
	if (Progress::getInstance().getNanoRobotCount() == 0u)
		m_state = FirstSpawn;
	else
		m_state = Walk;

	if (Progress::getInstance().getNanoRobotCount() != 0u)
	{
		m_talkaboutshit = false;
		m_state = None;
	}

	std::vector<sf::Vector2f> targets;
	targets.push_back(sf::Vector2f(143.f, 333.f));
	targets.push_back(sf::Vector2f(167.f, 330.f));
	targets.push_back(sf::Vector2f(172.f, 355.f));
	targets.push_back(sf::Vector2f(147.f, 358.f));
	setTargets(targets, 1.f);
}

void GroundTransformNanoRobot::update(sf::Time frameTime)
{
	NanoRobot::update(frameTime);

	switch (m_state)
	{
		case FirstSpawn:
			m_canSpeak = true;
			setTextIndex(0u);
			m_textTimer += frameTime;
			if (m_textTimer > m_textTimerMax)
			{
				m_canSpeak = false;
				m_state = Walk;
				m_textTimer = sf::Time::Zero;
			}
			break;
		case Walk:
			if (Progress::getInstance().getWalk())
			{
				m_textTimer = sf::Time::Zero;
				if (!Progress::getInstance().getMoveMap())
					m_state = MoveMap;
				else
					m_state = None;
			}
			else
			{
				m_textTimer += frameTime;
				m_canSpeak = false;
				if (m_textTimer > m_textTimerMax)
				{
					m_canSpeak = true;
					setTextIndex(1u);
				}
			}
			break;
		case MoveMap:
			if (Progress::getInstance().getMoveMap())
			{
				m_textTimer = sf::Time::Zero;
				if (!Progress::getInstance().getWalk())
					m_state = Walk;
				else
					m_state = None;
			}
			else
			{
				m_textTimer += frameTime;
				m_canSpeak = false;
				if (m_textTimer > m_textTimerMax)
				{
					m_canSpeak = true;
					setTextIndex(2u);
				}
			}
			break;
		case TalkAboutShit:
			m_textTimer += frameTime;
			m_canSpeak = true;
			setTextIndex(3u);
			if (m_textTimer > m_textTimerMax)
			{
				m_talkaboutshit = false;
				m_state = None;
			}
			break;
		case None:
			m_canSpeak = false;
			if (m_talkaboutshit)
			{
				m_textTimer += frameTime;
				if (m_textTimer > m_textTimerMax)
				{
					m_textTimer = sf::Time::Zero;
					m_state = TalkAboutShit;
				}
			}
			break;
		default:
			m_canSpeak = false;
			break;
	}
}

void GroundTransformNanoRobot::drawText(sf::RenderTarget& render, sf::RenderStates states) const
{
	if (m_canSpeak)
	{
		getCurrentText()->draw(render, states);
	}
}
