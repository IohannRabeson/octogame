#include "AFishNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "CharacterOcto.hpp"
#include <Interpolations.hpp>

RandomGenerator AFishNpc::m_generator("random");

AFishNpc::AFishNpc(ResourceKey const & npcId, ABiome & biome, bool isMeetable) :
	ANpc(npcId, isMeetable),
	m_waterLevel(biome.getWaterLevel()),
	m_isMet(false),
	m_velocity(m_generator.randomFloat(7.f, 13.f)),
	m_shift(m_generator.randomFloat(-50.f, 50.f), m_generator.randomFloat(-50.f, 50.f))
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::SwimNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	getBox()->setApplyGravity(false);
	setScale(randomFloat(0.5f, 0.9f));
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setFollowOcto(false);
	reverseSprite(true);
}

void AFishNpc::setupMachine(void)
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

void AFishNpc::setPosition(sf::Vector2f const & position)
{
	if (getPosition().x == 0.f && getPosition().y == 0.f)
		ANpc::setPosition(position);
}

void AFishNpc::onTheFloor(void)
{
}

void AFishNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();
	sf::Vector2f position = octo::linearInterpolation(m_targetPosition, box->getPosition(), 1.f - frametime.asSeconds());
	float angle = octo::rad2Deg(std::atan2(box->getPosition().y - getOctoPosition().y, box->getPosition().x - getOctoPosition().x));

	if (sprite.getCurrentEvent() == Special1)
	{
		float dist = std::sqrt(std::pow(position.x - m_targetPosition.x, 2u) + std::pow(position.y - m_targetPosition.y, 2u));
		if (position.y > m_waterLevel + 100.f && dist >= 100.f)
			box->setVelocity((position - box->getPosition()) * (dist / m_velocity));
	}
	else if (position.y > m_waterLevel + 100.f)
	{
		if (m_isMet)
			box->setVelocity((position - box->getPosition()) * 20.f);
		box->setVelocity(box->getVelocity() + sf::Vector2f(0.f, -40.f));
	}

	if (sprite.getCurrentEvent() == Special1)
	{
		if (angle < 0.f)
			angle += 360.f;
		sprite.setRotation(angle);
	}
	else
	{
		if (sprite.getRotation() > 90.f && sprite.getRotation() < 270.f)
			sprite.setRotation(octo::linearInterpolation(180.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
		else
			sprite.setRotation(octo::linearInterpolation(0.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
	}

	if (angle > 90.f && angle < 270.f)
		upSideDownSprite(true);
	else
		upSideDownSprite(false);

	m_randomMoveTimer += frametime;
	m_movementTimer -= frametime;
	ANpc::update(frametime);
}

void AFishNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && m_randomMoveTimer >= m_randomMoveTimerMax)
	{
		m_randomMoveTimerMax = sf::seconds(randomFloat(2.f, 5.f));
		m_randomMoveTimer = sf::Time::Zero;
		m_movementTimer = sf::seconds(7.f);

		float targetX;
		float targetY;

		if (m_generator.randomBool(0.5f))
			targetX = randomFloat(-400.f, -300.f);
		else
			targetX = randomFloat(300.f, 400.f);
		if (m_generator.randomBool(0.5f))
			targetY = randomFloat(-400.f, -300.f);
		else
			targetY = randomFloat(100.f, 300.f);

		m_targetPosition = getPosition() + sf::Vector2f(targetX, targetY);
		sprite.setNextEvent(Special1);
	}
	else if (sprite.getCurrentEvent() == Idle && getCollideEventOcto() && m_octoPosition.y > m_waterLevel)
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && !getCollideEventOcto() && (m_movementTimer <= sf::Time::Zero || (getBox()->getVelocity().x > -2.f && getBox()->getVelocity().x < 2.f)))
		sprite.setNextEvent(Idle);
}

void AFishNpc::updatePhysics(void)
{
	getBox()->update();
}

void AFishNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	m_octoPosition = octo->getPosition() + m_shift;
	m_targetPosition = m_octoPosition;
	if (!m_isMet)
		m_isMet = true;
}

void AFishNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
}

void AFishNpc::setVelocity(float velocity)
{
	m_velocity = velocity;
}

sf::Vector2f const & AFishNpc::getOctoPosition(void)
{
	return m_targetPosition;
}

float AFishNpc::getWaterLevel(void)
{
	return m_waterLevel;
}

float AFishNpc::randomFloat(float min, float max)
{
	return m_generator.randomFloat(min, max);
}
