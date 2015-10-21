#include "WellKeeperNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "Progress.hpp"

WellKeeperNpc::WellKeeperNpc(void) :
	ANpc(NPC_WELL_KEEPER_OSS)
{
	setSize(sf::Vector2f(50.f, 150.f));
	setOrigin(sf::Vector2f(70.f, 70.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(20.f, 0.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::LucienNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void WellKeeperNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
	setNextEvent(Idle);
	if (Progress::getInstance().canUseWaterJump())
		setCurrentText(1u);
}

void WellKeeperNpc::setupMachine(void)
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

void WellKeeperNpc::updateState(void)
{
	if (Progress::getInstance().canUseWaterJump())
	{
		octo::CharacterSprite & sprite = getSprite();
		sf::Vector2f const & size = sprite.getLocalSize();
		sprite.setOrigin(size.x - getOrigin().x, getOrigin().y);
		sprite.setScale(-getScale(), getScale());
	}
}
