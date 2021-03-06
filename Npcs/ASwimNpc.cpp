#include "ASwimNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "CharacterOcto.hpp"
#include <Interpolations.hpp>

RandomGenerator ASwimNpc::m_generator("random");

ASwimNpc::ASwimNpc(ResourceKey const & npcId, ABiome & biome, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_waterLevel(biome.getWaterLevel()),
	m_isMet(false),
	m_velocity(m_generator.randomFloat(7.f, 13.f)),
	m_shift(m_generator.randomFloat(-50.f, 50.f), m_generator.randomFloat(-50.f, 50.f))
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::SwimNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	getBox()->setApplyGravity(false);
	setFollowOcto(true);
}

void ASwimNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					special1;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1 = std::make_shared<State>("3", getSpecial1Animation(), getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, special1, idle);

	machine.addTransition(Special1, special1, special1);
	machine.addTransition(Special1, idle, special1);

	setMachine(machine);
	setNextEvent(Idle);
}

void ASwimNpc::setPosition(sf::Vector2f const & position)
{
	if (getPosition().x == 0.f && getPosition().y == 0.f)
		ANpc::setPosition(position);
}

void ASwimNpc::onTheFloor(void)
{
	m_additionalVelocity = sf::Vector2f(0.f, -50.f);
}

void ASwimNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();
	sf::Vector2f position = octo::linearInterpolation(m_octoPosition, box->getPosition(), 1.f - frametime.asSeconds());

	if (sprite.getCurrentEvent() == Special1)
	{
		float dist = std::sqrt(std::pow(position.x - m_octoPosition.x, 2u) + std::pow(position.y - m_octoPosition.y, 2u));
		if (position.y > box->getPosition().y && dist >= 100.f)
			box->setVelocity((position - box->getPosition()) * (dist / m_velocity));
	}
	else if (position.y > m_waterLevel + 100.f)
	{
		if (m_isMet)
			box->setVelocity((position - box->getPosition()) * 20.f);
		box->setVelocity(box->getVelocity() + m_additionalVelocity);
	}

	ANpc::update(frametime);
}

void ASwimNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && getCollideEventOcto() && m_octoPosition.y > m_waterLevel)
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && !getCollideEventOcto())
		sprite.setNextEvent(Idle);
}

void ASwimNpc::updatePhysics(void)
{
	getBox()->update();
}

void ASwimNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	m_octoPosition = octo->getPosition() + m_shift;
	if (!m_isMet)
		m_isMet = true;
}

void ASwimNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
}

void ASwimNpc::setVelocity(float velocity)
{
	m_velocity = velocity;
}

sf::Vector2f const & ASwimNpc::getOctoPosition(void)
{
	return m_octoPosition;
}

float ASwimNpc::getWaterLevel(void)
{
	return m_waterLevel;
}

float ASwimNpc::randomFloat(float min, float max)
{
	return m_generator.randomFloat(min, max);
}
