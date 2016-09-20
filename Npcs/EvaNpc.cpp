#include "EvaNpc.hpp"

EvaNpc::EvaNpc(sf::Color const & color) :
	AUniqueNpc(EVA_OSS)
{
	setSize(sf::Vector2f(60.f, 300.f));
	setOrigin(sf::Vector2f(62.f, 25.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(0.f, 0.f));
	setTimerMax(sf::seconds(8.0f));
	setup();

	m_particles.setColor(color);
}

void EvaNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {16u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {17u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {18u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {19u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupAnimation(m_startSpecial1, {
			FramePair(0.4f, 10u),
			FramePair(0.4f, 11u),
			FramePair(0.4f, 12u),
			FramePair(0.4f, 13u),
			FramePair(0.4f, 14u),
			FramePair(0.4f, 15u)},
			octo::LoopMode::NoLoop);

	setupAnimation(m_endSpecial1, {
			FramePair(0.4f, 15u),
			FramePair(0.4f, 14u),
			FramePair(0.4f, 13u),
			FramePair(0.4f, 12u),
			FramePair(0.4f, 11u),
			FramePair(0.4f, 10u)},
			octo::LoopMode::NoLoop);

	setupMachine();
}

void EvaNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					special1State;
	StatePtr					startSpecial1State;
	StatePtr					endSpecial1State;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1State = std::make_shared<State>("1", getSpecial1Animation(), getSprite());
	startSpecial1State = std::make_shared<State>("2", m_startSpecial1, getSprite());
	endSpecial1State = std::make_shared<State>("3", m_endSpecial1, getSprite());

	machine.setStart(special1State);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, endSpecial1State, idleState);

	machine.addTransition(StartSpecial1, idleState, startSpecial1State);

	machine.addTransition(Special1, startSpecial1State, special1State);
	machine.addTransition(Special1, special1State, special1State);

	machine.addTransition(EndSpecial1, special1State, endSpecial1State);

	setMachine(machine);
	setNextEvent(Special1);
}

void EvaNpc::update(sf::Time frameTime)
{
	octo::CharacterSprite & sprite = getSprite();
	m_particles.update(frameTime);
	m_particles.setEmitter(sprite.getPosition() + sf::Vector2f(45.f, 228.f));
	m_particles.canEmit(false);
	ANpc::update(frameTime);
}

void EvaNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (getSprite().getCurrentEvent() == Special1 && getCollideEventOcto())
	{
		getSprite().setNextEvent(EndSpecial1);
	}
	else if (sprite.getCurrentEvent() == StartSpecial1)
	{
		if (sprite.isTerminated())
			sprite.setNextEvent(Special1);
	}
	else if (sprite.getCurrentEvent() == EndSpecial1)
	{
		if (sprite.isTerminated())
			sprite.setNextEvent(Idle);
	}
	else if (sprite.getCurrentEvent() == Idle)
	{
		if (!getCollideEventOcto())
			sprite.setNextEvent(StartSpecial1);
	}
	else
		m_particles.canEmit(true);
}

void EvaNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
	m_particles.draw(render);
}
