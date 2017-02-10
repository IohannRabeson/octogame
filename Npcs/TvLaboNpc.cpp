#include "TvLaboNpc.hpp"

TvLaboNpc::TvLaboNpc(ResourceKey key) :
	ANpc(key, false),
	m_generator("random"),
	m_firstUpdate(false)
{
    setType(GameObjectType::TvLaboNpc);
	setSize(sf::Vector2f(107.f, 76.f));
	setOrigin(sf::Vector2f(50.f, 12.f));
	setScale(1.f);
	setup();
}

void TvLaboNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;
	std::vector<Frame>								frames;

	frames.push_back(Frame(sf::seconds(m_generator.randomFloat(0.1f, 0.2f)), {static_cast<std::size_t>(0u), sf::FloatRect(), sf::Vector2f()}));
	frames.push_back(Frame(sf::seconds(m_generator.randomFloat(0.1f, 0.2f)), {static_cast<std::size_t>(1u), sf::FloatRect(), sf::Vector2f()}));
	frames.push_back(Frame(sf::seconds(m_generator.randomFloat(0.1f, 0.2f)), {static_cast<std::size_t>(0u), sf::FloatRect(), sf::Vector2f()}));
	frames.push_back(Frame(sf::seconds(m_generator.randomFloat(0.1f, 0.2f)), {static_cast<std::size_t>(1u), sf::FloatRect(), sf::Vector2f()}));
	frames.push_back(Frame(sf::seconds(m_generator.randomFloat(5.0f, 10.f)), {static_cast<std::size_t>(2u), sf::FloatRect(), sf::Vector2f()}));

	getIdleAnimation().setFrames(frames);
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void TvLaboNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;

	idleState = std::make_shared<State>("0", getIdleAnimation(), getSprite());

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);

	setMachine(machine);
	setNextEvent(Idle);
}

void TvLaboNpc::updateState(void)
{
	m_firstUpdate = true;
}


void TvLaboNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	if (m_firstUpdate)
		ANpc::draw(render, states);
}
