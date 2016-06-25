#include "CedricStartNpc.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include "ChallengeManager.hpp"
#include <ResourceManager.hpp>

CedricStartNpc::CedricStartNpc(ABiome::Type biomeType) :
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

void CedricStartNpc::setup(void)
{
	setupIdleAnimation({
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

	setupSpecial1Animation({
			FramePair(0.3f, 10u),
			FramePair(0.3f, 11u),
			FramePair(0.3f, 12u),
			FramePair(0.3f, 13u),
			FramePair(0.8f, 14u),
			FramePair(0.3f, 17u),
			FramePair(0.3f, 18u),
			FramePair(0.3f, 19u),
			FramePair(0.3f, 20u),
			FramePair(0.3f, 21u),
			FramePair(0.3f, 22u)}, octo::LoopMode::NoLoop);

	setupAnimation(m_idleNightAnimation, {
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

	setupAnimation(m_special2NightAnimation, {
			FramePair(0.3f, 40u),
			FramePair(0.3f, 41u),
			FramePair(0.3f, 42u),
			FramePair(0.3f, 43u),
			FramePair(0.3f, 44u),
			FramePair(0.3f, 45u),
			FramePair(0.3f, 46u),
			FramePair(0.3f, 47u)}, octo::LoopMode::NoLoop);

	setupMachine();
}

void CedricStartNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					special1;
	StatePtr					idleNight;
	StatePtr					special2Night;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1 = std::make_shared<State>("1", getSpecial1Animation(), getSprite());
	idleNight = std::make_shared<State>("2", m_idleNightAnimation, getSprite());
	special2Night = std::make_shared<State>("3", m_special2NightAnimation, getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, special2Night, idle);

	machine.addTransition(Special1, idle, special1);

	machine.addTransition(IdleNight, special1, idleNight);

	machine.addTransition(Special2Night, idleNight, special2Night);

	setMachine(machine);
	setNextEvent(Idle);
}

bool CedricStartNpc::startBalle(void)
{
	if (!ChallengeManager::getInstance().getEffect(m_effect).enable() && !Progress::getInstance().isValidateChallenge(m_effect) && getSprite().getCurrentEvent() == Idle)
	{
		ChallengeManager::getInstance().getEffect(m_effect).start();
		getSprite().setNextEvent(Special1);
		return (true);
	}
	return (false);
}

void CedricStartNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updateState();

	sprite.update(frametime);
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();
	sprite.setPosition(bounds.left, bounds.top);

	updateText(frametime);

	if (!ChallengeManager::getInstance().getEffect(m_effect).enable() && !Progress::getInstance().isValidateChallenge(m_effect) && sprite.getCurrentEvent() == IdleNight)
		sprite.setNextEvent(Special2Night);

	resetVariables();
}

void CedricStartNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Special1 && sprite.isTerminated())
	{
		sprite.setNextEvent(IdleNight);
	}
	else if (sprite.getCurrentEvent() == Special2Night && sprite.isTerminated())
	{
		sprite.setNextEvent(Idle);
	}
}
