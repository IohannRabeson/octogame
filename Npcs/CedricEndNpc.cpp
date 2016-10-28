#include "CedricEndNpc.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include "ChallengeManager.hpp"
#include "CharacterOcto.hpp"
#include <ResourceManager.hpp>
#include <Application.hpp>
#include <AudioManager.hpp>

CedricEndNpc::CedricEndNpc(ABiome::Type biomeType) :
	AUniqueNpc(CEDRIC_END_OSS, false)
{
	setType(GameObjectType::CedricEndNpc);
	setSize(sf::Vector2f(31.f, 174.f));
	setOrigin(sf::Vector2f(85.f, 19.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, -10.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));

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

void CedricEndNpc::collideOctoEvent(CharacterOcto * octo)
{
	AUniqueNpc::collideOctoEvent(octo);
	stopBalle();

	octo::CharacterSprite & sprite = getSprite();
	sf::Vector2f const & size = sprite.getLocalSize();

	if (octo->getPosition().x < getPosition().x)
	{
		sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
		sprite.setScale(-getScale(), getScale());
	}
	else
	{
		sprite.setOrigin(getOrigin().x, getOrigin().y);
		sprite.setScale(getScale(), getScale());
	}
}

void CedricEndNpc::stopBalle(void)
{
	if (ChallengeManager::getInstance().getEffect(m_effect).enable() && !Progress::getInstance().isValidateChallenge(m_effect) && getSprite().getCurrentEvent() == Idle)
	{
		octo::AudioManager& audio = octo::Application::getAudioManager();
		octo::ResourceManager& resources = octo::Application::getResourceManager();

		audio.playSound(resources.getSound(OCTO_SOUND_STOP_BALLE_OGG), 1.f);
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

void CedricEndNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	AUniqueNpc::draw(render, states);
}

