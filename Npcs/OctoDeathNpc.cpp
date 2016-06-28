#include "OctoDeathNpc.hpp"
#include "RectangleShape.hpp"

OctoDeathNpc::OctoDeathNpc(void) :
	ANpc(OCTO_DEATH_HELMET_OSS, false),
	m_isCollide(false)
{
	setSize(sf::Vector2f(10.f, 15.f));
	setOrigin(sf::Vector2f(27.f, 10.f));
	setScale(0.6f);
	setTimerMax(sf::seconds(8.0f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void OctoDeathNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::NoLoop);

	setupMachine();
}

void OctoDeathNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					special1State;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1State = std::make_shared<State>("1", getSpecial1Animation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, special1State, idleState);

	machine.addTransition(Special1, idleState, special1State);

	setMachine(machine);
	setNextEvent(Idle);
}

void OctoDeathNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	updatePhysics();
	updateState();

	if (getCollideEventOcto())
		m_isCollide = true;

	sprite.update(frametime);
	sprite.setPosition(getBox()->getRenderPosition());
	//sprite.setColor(sf::Color(255, 255, 255, m_transparency));

	updateText(frametime);
	resetVariables();
}

void OctoDeathNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (m_isCollide && sprite.getCurrentEvent() == Idle)
		sprite.setNextEvent(Special1);
}
