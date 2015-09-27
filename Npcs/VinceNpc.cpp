#include "VinceNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"

VinceNpc::VinceNpc(void) :
	ANpc(VINCE_OSS)
{
	setSize(sf::Vector2f(1.f, 75.f));
	setOrigin(sf::Vector2f(180.f, 380.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(-60.f, -120.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::VinceNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void VinceNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void VinceNpc::setupMachine(void)
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

void VinceNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	sprite.update(frametime);
	sf::Vector2f const & center = getBox()->getRenderPosition();
	sprite.setPosition(center);

	updateText(frametime);
	resetVariables();
}

