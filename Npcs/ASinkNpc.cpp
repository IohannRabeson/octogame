#include "ASinkNpc.hpp"
#include "RectangleShape.hpp"

#include <iostream>

ASinkNpc::ASinkNpc(ResourceKey const & npcId, float waterLevel, sf::Color waterColor) :
	ANpc(npcId, false),
	m_isCollide(false),
	m_waterLevel(waterLevel),
	m_waterColor(waterColor),
	m_fallingWater(true),
	m_velocity(0.f, 0.f)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::SinkNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	setupSmoke();
}

void ASinkNpc::setupSmoke(void)
{
	m_smoke.setup(sf::Vector2f(1.5f, 1.5f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -40.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(5.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(m_waterColor);
	m_smoke.setCanEmit(false);
}

void ASinkNpc::setVelocity(sf::Vector2f const & velocity)
{
	m_velocity = velocity;
}

void ASinkNpc::setupMachine(void)
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

void ASinkNpc::update(sf::Time frametime)
{
	if (getCollideEventOcto())
		m_isCollide = true;

	m_smoke.setPosition(getBox()->getRenderPosition());
	m_smoke.update(frametime);

	ANpc::update(frametime);
}

void ASinkNpc::updatePhysics(void)
{
	float			posY = getPosition().y + getBox()->getSize().y;
	sf::Vector2f	velocity;

	if (posY >= m_waterLevel && m_waterLevel > 0 && m_waterColor.a > 0)
	{
		if (m_isCollide)
		{
			velocity = sf::Vector2f(0.f, -500.f);
			if (getPosition().y > m_waterLevel + 40.f)
				m_smoke.setCanEmit(true);
		}
		else if (m_fallingWater == true)
		{
			velocity = sf::Vector2f(0.f, -580.f);
			if (posY >= m_waterLevel + 5.f)
				m_fallingWater = false;
		}
		else
		{
			velocity = sf::Vector2f(0.f, -610.f);
			if (posY <= m_waterLevel + 2.f)
				m_fallingWater = true;
		}
	}
	getBox()->setVelocity(velocity + m_velocity);
}

void ASinkNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (m_isCollide && sprite.getCurrentEvent() == Idle)
		sprite.setNextEvent(Special1);
}

void ASinkNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
	m_smoke.draw(render);
}
