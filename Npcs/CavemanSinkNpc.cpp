#include "CavemanSinkNpc.hpp"
#include "RectangleShape.hpp"

#include <iostream>

CavemanSinkNpc::CavemanSinkNpc(float waterLevel, sf::Color waterColor) :
	ASinkNpc(CAVEMAN_SINKING_OSS, waterLevel, waterColor),
	m_fallingWater(true),
	m_startSinkTimerMax(sf::seconds(1.5f)),
	m_fadeTimerMax(sf::seconds(3.f))
{
	setType(GameObjectType::CavemanSinkingNpc);
	setSize(sf::Vector2f(34.f, 88.f));
	setOrigin(sf::Vector2f(73.f, 56.f));
	setScale(0.8f);
	setTimerMax(sf::seconds(8.0f));
	reverseSprite(true);
	setup();
}

void CavemanSinkNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.1f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.1f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.1f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.2f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.3f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {10u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void CavemanSinkNpc::update(sf::Time frametime)
{
	ASinkNpc::update(frametime);
	if (isCollide())
	{
		if (m_startSinkTimer < m_startSinkTimerMax)
			m_startSinkTimer += frametime;
		else
			m_fadeTimer += frametime;
	}
}

void CavemanSinkNpc::updatePhysics(void)
{
	float			posY = getPosition().y + getBox()->getSize().y;
	sf::Vector2f	velocity;

	if (posY >= getWaterLevel())
	{
		if (isCollide() && m_startSinkTimer >= m_startSinkTimerMax)
		{
			velocity = sf::Vector2f(0.f, -500.f);
			if (m_fadeTimer < m_fadeTimerMax)
			{
				if (getPosition().y > getWaterLevel() + 40.f)
					getSmoke().setCanEmit(true);
				getSprite().setColor(sf::Color(255, 255, 255, 255 * (1.f - (m_fadeTimer / m_fadeTimerMax))));
			}
			else
				getSmoke().setCanEmit(false);
		}
		else if (m_fallingWater == true)
		{
			velocity = sf::Vector2f(0.f, -580.f);
			if (posY >= getWaterLevel() + 5.f)
				m_fallingWater = false;
		}
		else
		{
			velocity = sf::Vector2f(0.f, -610.f);
			if (posY <= getWaterLevel() + 2.f)
				m_fallingWater = true;
		}
	}
	getBox()->setVelocity(velocity + sf::Vector2f(-20.f, 0.f));
}
