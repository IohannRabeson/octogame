#include "OctoDeathNpc.hpp"
#include "RectangleShape.hpp"

#include <iostream>

OctoDeathNpc::OctoDeathNpc(float waterLevel) :
	ANpc(OCTO_DEATH_HELMET_OSS, false),
	m_isCollide(false),
	m_waterLevel(waterLevel),
	m_fallingWater(true)
{
	setSize(sf::Vector2f(10.f, 15.f));
	setOrigin(sf::Vector2f(27.f, 10.f));
	setScale(0.6f);
	setTimerMax(sf::seconds(8.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void OctoDeathNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();

	m_smoke.setup(sf::Vector2f(1.5f, 1.5f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -40.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(5.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(154, 213, 141, 150));
	m_smoke.setCanEmit(false);
}

void OctoDeathNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					special1State;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1State = std::make_shared<State>("1", getSpecial1Animation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, special1State, idleState);

	machine.addTransition(Special1, idleState, special1State);

	setMachine(machine);
	setNextEvent(Idle);
}

void OctoDeathNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updatePhysics();
	updateState();

	if (getCollideEventOcto())
		m_isCollide = true;

	sprite.update(frametime);
	sprite.setPosition(getBox()->getRenderPosition());
	//sprite.setColor(sf::Color(255, 255, 255, m_transparency));
	m_smoke.setPosition(getBox()->getRenderPosition());
	m_smoke.update(frametime);

	updateText(frametime);
	resetVariables();
}

void OctoDeathNpc::updatePhysics(void)
{
	if (getPosition().y >= m_waterLevel - 20.f)
	{
		if (m_isCollide)
		{
			getBox()->setVelocity(getBox()->getVelocity() - sf::Vector2f(0.f, 500.f));
			if (getPosition().y > m_waterLevel + 40.f)
				m_smoke.setCanEmit(true);
			return;
		}

		if (m_fallingWater == true && getPosition().y >= m_waterLevel - 10.f)
			m_fallingWater = false;
		else if (getPosition().y <= m_waterLevel - 15.f)
			m_fallingWater = true;

		if (m_fallingWater)
			getBox()->setVelocity(getBox()->getVelocity() - sf::Vector2f(0.f, 520.f));
		else
			getBox()->setVelocity(getBox()->getVelocity() - sf::Vector2f(0.f, 610.f));
	}
}

void OctoDeathNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (m_isCollide && sprite.getCurrentEvent() == Idle)
		sprite.setNextEvent(Special1);
}

void OctoDeathNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
	m_smoke.draw(render);
}
