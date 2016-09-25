#include "ASwimNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "CharacterOcto.hpp"
#include <Interpolations.hpp>

RandomGenerator ASwimNpc::m_generator("random");

ASwimNpc::ASwimNpc(ResourceKey const & npcId, bool isMeetable, bool isShift) :
	ANpc(npcId, isMeetable),
	m_waterLevel(0.f),
	m_isMet(false),
	m_isShift(isShift),
	m_velocity(m_generator.randomFloat(7.f, 13.f)),
	m_shift(m_generator.randomFloat(-50.f, 50.f), m_generator.randomFloat(-50.f, 50.f)),
	m_baseAngle(0.f)
{
	setupBox(this, static_cast<std::size_t>(GameObjectType::SwimNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	getBox()->setApplyGravity(false);
	setFollowOcto(true);
	if (m_generator.randomBool(0.5f))
		reverseSprite(true);
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
	if (!m_isMet)
		ANpc::setPosition(position);
}

void ASwimNpc::update(sf::Time frametime)
{
	computeBehavior(frametime);
	ANpc::update(frametime);
}

void ASwimNpc::computeBehavior(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();
	sf::Vector2f position = octo::linearInterpolation(m_octoPosition, box->getPosition(), 1.f - frametime.asSeconds());

	if (sprite.getCurrentEvent() == Special1)
	{
		float dist = std::sqrt(std::pow(position.x - m_octoPosition.x, 2u) + std::pow(position.y - m_octoPosition.y, 2u));
		if (position.y > box->getPosition().y && dist >= 100.f)
			box->setVelocity((position - box->getPosition()) * (dist / m_velocity));

		if (m_isShift)
		{
			float angle = octo::rad2Deg(std::atan2(box->getPosition().y - m_octoPosition.y, box->getPosition().x - m_octoPosition.x)) + m_baseAngle;
			if (angle < 0.f)
				angle += 360.f;
			sprite.setRotation(angle);
			//TODO : Clean this
			if (m_baseAngle == 0.f)
			{
				if (angle > 90.f && angle < 270.f)
					upSideDownSprite(true);
				else
					upSideDownSprite(false);
			}
		}
	}
	else if (m_isMet)
	{
		if (position.y > m_waterLevel + 200.f)
			box->setVelocity((position - box->getPosition()) * 20.f);
		if (sprite.getRotation() > 180.f)
		{
			upSideDownSprite(true);
			sprite.setRotation(octo::linearInterpolation(360.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
		}
		else
		{
			upSideDownSprite(false);
			sprite.setRotation(octo::linearInterpolation(0.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
		}
	}
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
	if (m_waterLevel == 0.f)
		m_waterLevel = octo->getWaterLevel();
}

void ASwimNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
}

void ASwimNpc::setVelocity(float velocity)
{
	m_velocity = velocity;
}

void ASwimNpc::setBaseAngle(float angle)
{
	m_baseAngle = angle;
}

float ASwimNpc::randomFloat(float min, float max)
{
	return m_generator.randomFloat(min, max);
}
