#include "CedricEndNpc.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include "ChallengeManager.hpp"
#include <ResourceManager.hpp>

CedricEndNpc::CedricEndNpc(ABiome::Type biomeType) :
	ANpc(CEDRIC_END_OSS)
{
	setSize(sf::Vector2f(200.f, 100.f));
	setOrigin(sf::Vector2f(-30.f, 68.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(80.f, -50.f));
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

	setupWalkAnimation({
			FramePair(0.2f, 0u),
			FramePair(0.2f, 1u),
			FramePair(0.2f, 0u),
			FramePair(0.2f, 3u),
			FramePair(0.2f, 4u),
			FramePair(0.2f, 3u),
			FramePair(0.2f, 2u)}, octo::LoopMode::Loop);

	setupSpecial1Animation({
			FramePair(0.2f, 10u),
			FramePair(0.2f, 11u),
			FramePair(0.2f, 12u),
			FramePair(0.2f, 13u),
			FramePair(0.2f, 14u)}, octo::LoopMode::NoLoop);

	setupSpecial2Animation({
			FramePair(0.2f, 15u),
			FramePair(0.2f, 16u),
			FramePair(0.2f, 17u),
			FramePair(0.2f, 18u),
			FramePair(0.2f, 19u),
			FramePair(0.2f, 20u),
			FramePair(0.2f, 21u),
			FramePair(0.2f, 22u)}, octo::LoopMode::NoLoop);

	//setupAnimation(m_idleAnimationNight, {
	//		FramePair(0.4f, 27u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 31u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 27u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 31u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 27u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 31u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 27u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 31u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 27u),
	//		FramePair(0.4f, 30u),
	//		FramePair(0.4f, 31u),
	//		FramePair(0.4f, 32u),
	//		FramePair(0.4f, 33u),
	//		FramePair(0.4f, 32u),
	//		FramePair(0.4f, 31u),
	//		FramePair(0.4f, 30u)}, octo::LoopMode::Loop);

	//setupAnimation(m_walkAnimationNight, {
	//		FramePair(0.2f, 25u),
	//		FramePair(0.2f, 26u),
	//		FramePair(0.2f, 25u),
	//		FramePair(0.2f, 28u),
	//		FramePair(0.2f, 29u),
	//		FramePair(0.2f, 28u),
	//		FramePair(0.2f, 27u)}, octo::LoopMode::Loop);

	//setupAnimation(m_special1AnimationNight, {
	//		FramePair(0.2f, 35u),
	//		FramePair(0.2f, 36u),
	//		FramePair(0.2f, 37u),
	//		FramePair(0.2f, 38u),
	//		FramePair(0.2f, 39u)}, octo::LoopMode::NoLoop);

	//setupAnimation(m_special2AnimationNight, {
	//		FramePair(0.2f, 40u),
	//		FramePair(0.2f, 41u),
	//		FramePair(0.2f, 42u),
	//		FramePair(0.2f, 43u),
	//		FramePair(0.2f, 44u),
	//		FramePair(0.2f, 45u),
	//		FramePair(0.2f, 46u),
	//		FramePair(0.2f, 47u)}, octo::LoopMode::NoLoop);

	setupMachine();
}

void CedricEndNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					special1;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1 = std::make_shared<State>("1", getSpecial1Animation(), getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, idle, special1);

	machine.addTransition(Special1, special1, idle);

	setMachine(machine);
	setNextEvent(Idle);
}

void CedricEndNpc::stopBalle(void)
{
	Progress::getInstance().validateChallenge(m_effect);
}

void CedricEndNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updateState();

	sprite.update(frametime);
	sf::FloatRect const & bounds = getBox()->getGlobalBounds();
	sprite.setPosition(bounds.left, bounds.top);

	updateText(frametime);

	//if (m_id == 0u && ChallengeManager::getInstance().getEffect(ChallengeManager::Effect::Duplicate).enable())
	//	setCurrentText(1u);
	resetVariables();
}

void CedricEndNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();
	if (sprite.getCurrentEvent() == Idle)
	{
		sprite.setNextEvent(Special1);
	}
}
