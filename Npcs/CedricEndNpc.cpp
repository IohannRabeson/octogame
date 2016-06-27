#include "CedricEndNpc.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include "ChallengeManager.hpp"
#include <ResourceManager.hpp>

CedricEndNpc::CedricEndNpc(ABiome::Type biomeType) :
	ANpc(CEDRIC_START_OSS, false)
{
	setSize(sf::Vector2f(50.f, 100.f));
	setOrigin(sf::Vector2f(60.f, 68.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(0.f, -50.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::Player) | static_cast<std::size_t>(GameObjectType::PlayerEvent));

	switch (biomeType)
	{
		case ABiome::Type::Ice:
			m_effect = ChallengeManager::Effect::Blur;
			break;
		case ABiome::Type::Jungle:
			m_effect = ChallengeManager::Effect::Duplicate;
			break;
		case ABiome::Type::Desert:
			m_effect = ChallengeManager::Effect::Persistence;
			break;
		case ABiome::Type::Water:
			m_effect = ChallengeManager::Effect::Displacement;
			break;
		default:
			m_effect = ChallengeManager::Effect::Pixelate;
			break;
	}

	setCurrentText(0u);
}

void CedricEndNpc::setup(void)
{
	setupIdleAnimation({
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 27u),
			FramePair(0.4f, 30u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 32u),
			FramePair(0.4f, 33u),
			FramePair(0.4f, 32u),
			FramePair(0.4f, 31u),
			FramePair(0.4f, 30u)}, octo::LoopMode::Loop);

	setupSpecial1Animation({
			FramePair(0.3f, 40u),
			FramePair(0.3f, 41u),
			FramePair(0.3f, 42u),
			FramePair(0.3f, 43u),
			FramePair(0.3f, 44u),
			FramePair(0.3f, 45u),
			FramePair(0.3f, 46u),
			FramePair(0.3f, 47u)}, octo::LoopMode::NoLoop);

	setupAnimation(m_idleDayAnimation, {
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 2u),
			FramePair(0.4f, 5u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 7u),
			FramePair(0.4f, 8u),
			FramePair(0.4f, 7u),
			FramePair(0.4f, 6u),
			FramePair(0.4f, 5u)}, octo::LoopMode::Loop);

	setupMachine();
}

void CedricEndNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					special1;
	StatePtr					idleDay;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1 = std::make_shared<State>("1", getSpecial1Animation(), getSprite());
	idleDay = std::make_shared<State>("2", m_idleDayAnimation, getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);

	machine.addTransition(Special1, idle, special1);

	machine.addTransition(IdleDay, idleDay, idleDay);
	machine.addTransition(IdleDay, special1, idleDay);

	setMachine(machine);
	setNextEvent(Idle);
}

void CedricEndNpc::stopBalle(void)
{
	if (!Progress::getInstance().isValidateChallenge(m_effect) && getSprite().getCurrentEvent() == Idle)
	{
		getSprite().setNextEvent(Special1);
		Progress::getInstance().validateChallenge(m_effect);
	}
}

void CedricEndNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updateState();

	sprite.update(frametime);
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();
	sprite.setPosition(bounds.left, bounds.top);

	updateText(frametime);

	resetVariables();
}

void CedricEndNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
	{
		sprite.setNextEvent(IdleDay);
	}
}
