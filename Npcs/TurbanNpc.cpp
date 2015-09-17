#include "TurbanNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

TurbanNpc::TurbanNpc(void) :
	ANpc(NPC_TURBAN_OSS)
{
	setSize(sf::Vector2f(1.f, 75.f));
	setOrigin(sf::Vector2f(110.f, 130.f));
	setScale(0.6f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-20.f, -80.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::TurbanNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void TurbanNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()})
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void TurbanNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);

	setMachine(machine);
	setNextEvent(Idle);
}

void TurbanNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updateState();
	updatePhysics();

	sprite.update(frametime);
	sf::Vector2f const & center = getBox()->getRenderPosition();
	sprite.setPosition(center);

	updateText(frametime);
	resetVariables();
}

void TurbanNpc::updateState(void)
{
}

void TurbanNpc::updatePhysics(void)
{
}

void TurbanNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
}

