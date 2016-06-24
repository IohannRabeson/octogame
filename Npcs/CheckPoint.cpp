#include "CheckPoint.hpp"
#include "CharacterOcto.hpp"
#include "Progress.hpp"

CheckPoint::CheckPoint(void) :
	ANpc(CHECKPOINT_OSS),
	m_count(100),
	m_used(0u),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(3.f)),
	m_vertices(new sf::Vertex[m_count]),
	m_size(150.f, 150.f),
	m_color(255, 255, 255, 200),
	m_firstFrame(true),
	m_isValidated(false)
{
	setSize(sf::Vector2f(30.f, 85.f));
	setOrigin(sf::Vector2f(27.f, 275.f));
	setScale(0.8f);
	setTextOffset(sf::Vector2f(-20.f, -10.f));
	setTimerMax(sf::seconds(8.0f));
	setup();

	m_builder = octo::VertexBuilder(m_vertices.get(), m_count);
	setupBox(this, static_cast<std::size_t>(GameObjectType::Npc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
	setTimer(sf::Time::Zero);
	setTimerMax(sf::seconds(4.f));
}

void CheckPoint::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {7u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {8u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {9u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void CheckPoint::setupMachine(void)
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
	machine.addTransition(Special1, special1State, special1State);

	setMachine(machine);
	setNextEvent(Idle);
}

void CheckPoint::createLosange(sf::Vector2f const & size, sf::Vector2f const & origin, sf::Color const & color, octo::VertexBuilder& builder)
{
	sf::Vector2f up(0.f, -size.y);
	sf::Vector2f down(0.f, size.y);
	sf::Vector2f left(-size.x, 0.f);
	sf::Vector2f right(size.x, 0.f);

	up += origin;
	down += origin;
	left += origin;
	right += origin;

	builder.createTriangle(left, up, right, color);
	builder.createTriangle(left, down, right, color);
}

void CheckPoint::createEffect(sf::Vector2f const & size, sf::Vector2f const & origin, float glowingCoef, sf::Color color, octo::VertexBuilder& builder)
{
	color.a = color.a * (1 - glowingCoef);
	createLosange(size * glowingCoef, origin, color, builder);
}

void CheckPoint::update(sf::Time frametime)
{
	ANpc::update(frametime);
	if (m_firstFrame)
	{
		m_firstFrame = false;
		m_startPosition = getPosition();
	}
	if (m_isValidated)
	{
		m_timer += frametime;
		if (m_timer > m_timerMax)
			m_timer = sf::Time::Zero;
		m_builder.clear();
		createEffect(m_size, getPosition() + sf::Vector2f(15.f, -200.f), std::pow(m_timer / m_timerMax, 0.867f), m_color, m_builder);
		createEffect(m_size, getPosition() + sf::Vector2f(15.f, -200.f), std::pow(m_timer / m_timerMax, 0.12f), m_color, m_builder);
		m_used = m_builder.getUsed();
	}
}

void CheckPoint::updateState(void)
{
}

void CheckPoint::collideOctoEvent(CharacterOcto *)
{
	octo::CharacterSprite & sprite = getSprite();
	m_isValidated = true;
	if (sprite.getCurrentEvent() == Idle)
		sprite.setNextEvent(Special1);
	Progress::getInstance().setCheckPointPosition(m_startPosition);
}

void CheckPoint::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	render.draw(m_vertices.get(), m_used, sf::Triangles, states);
	ANpc::draw(render, states);
}
