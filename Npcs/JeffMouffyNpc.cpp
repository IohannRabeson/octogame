#include "JeffMouffyNpc.hpp"
#include "RectangleShape.hpp"
#include "Progress.hpp"
#include <Interpolations.hpp>
#include <list>

JeffMouffyNpc::JeffMouffyNpc(void) :
	ASpecialNpc(JEFF_MOUFFY_OSS, false),
	m_startTimer(false),
	m_animationEnd(false),
	m_isMet(false)
{
	setType(GameObjectType::JeffMouffyNpc);
	setSize(sf::Vector2f(174.f, 186.f));
	setOrigin(sf::Vector2f(82.f, 100.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(100.f, 0.f));
	setTimerMax(sf::seconds(5.f));
	setup();
}

void JeffMouffyNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.2f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {11u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {12u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {13u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {14u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {15u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {19u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {20u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {21u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {22u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {23u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
	getBox()->setApplyGravity(false);

	std::list<GameObjectType> const & npcMet = Progress::getInstance().getNpcMet();
	auto it = std::find(npcMet.begin(), npcMet.end(), GameObjectType::JeffMouffyNpc);

	if (it != npcMet.end())
		m_isMet = true;
}

void JeffMouffyNpc::setPosition(sf::Vector2f const & position)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1)
		return;
	ASpecialNpc::setPosition(position);
}

void JeffMouffyNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle)
	{
		if (!m_startTimer && getCollideEventOcto())
		{
			m_startTimer = true;
			setTimer(sf::Time::Zero);
		}
		if (m_startTimer || m_isMet)
		{
			if (getTimer() > getTimerMax())
			{
				setDisplayText(false);
				sprite.setNextEvent(Special1);
				m_startPosition = getPosition();
				setTimer(sf::Time::Zero);
			}
		}
	}
}

void JeffMouffyNpc::updatePhysics(void)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1)
	{
		if (!sprite.isTerminated())
			setTimer(sf::Time::Zero);
		else if (sprite.isTerminated())
		{
			if (m_animationEnd == false)
			{
				RectangleShape * box = getBox();
				box->setPosition(octo::linearInterpolation(m_startPosition, m_startPosition + sf::Vector2f(0.f, -2000.f), getTimer() / getTimerMax()));
				box->update();
			}
			if (getTimer() > getTimerMax())
			{
				setCurrentText(1u);
				setTextOffset(sf::Vector2f(0.f, -25.f));
				setDisplayText(true);
				m_animationEnd = true;
			}
		}
	}
}
