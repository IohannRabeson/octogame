#include "CharacterNpc.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <Camera.hpp>

CharacterNpc::CharacterNpc() :
	m_box(PhysicsEngine::getShapeBuilder().createRectangle(false)),
	m_pixelSecondWalk(230),
	m_originMove(false)
{
	octo::ResourceManager&		resources = octo::Application::getResourceManager();

	m_box->setGameObject(this);
	m_box->setSize(sf::Vector2f(100.f / 2.f, 150.f));
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Npc));
	m_box->setCollisionMask(0u);
	m_sprite.setSpriteSheet(resources.getSpriteSheet(OCTO_COMPLETE_OSS));
	setupAnimation();
	setupMachine();
	m_timeEventIdle = sf::Time::Zero;
	m_sprite.restart();
	m_sprite.setNextEvent(Idle);
	m_sprite.setColor(sf::Color::Green);
	m_area = sf::FloatRect(0, 0, 0, 0);
}

void	CharacterNpc::setup(sf::Vector2f const & pos, sf::FloatRect const & rect)
{
	m_area = rect;
	m_box->setPosition(pos.x, pos.y - m_box->getSize().y);
}

void	CharacterNpc::setupAnimation()
{
	typedef octo::CharacterAnimation::Frame			Frame;

	m_idleAnimation.setFrames({
			Frame(sf::seconds(0.4f), {10, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {11, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {12, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {13, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {14, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {15, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {16, sf::FloatRect(), sf::Vector2f()})
			});
	m_idleAnimation.setLoop(octo::LoopMode::Loop);

	m_walkAnimation.setFrames({
			Frame(sf::seconds(0.4f), {0, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {6, sf::FloatRect(), sf::Vector2f()})
			});
	m_walkAnimation.setLoop(octo::LoopMode::Loop);
}

void	CharacterNpc::setupMachine()
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					state0;
	StatePtr					state1;
	StatePtr					state2;

	state0 = std::make_shared<State>("0", m_idleAnimation, m_sprite);
	state1 = std::make_shared<State>("1", m_walkAnimation, m_sprite);
	state2 = std::make_shared<State>("2", m_walkAnimation, m_sprite);

	machine.setStart(state0);
	machine.addTransition(Idle, state0, state0);
	machine.addTransition(Idle, state1, state0);
	machine.addTransition(Idle, state2, state0);

	machine.addTransition(Left, state0, state1);

	machine.addTransition(Right, state0, state2);
	m_sprite.setMachine(machine);
}

void	CharacterNpc::update(sf::Time frameTime)
{
	timeEvent(frameTime);
	updateState();
	updatePhysics(frameTime);
	m_sprite.update(frameTime);
	commitPhysicsToGraphics();
}

void	CharacterNpc::draw(sf::RenderTarget& render, sf::RenderStates states)const
{
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Blue);
	rect.setPosition(sf::Vector2f(m_area.left, m_area.top));
	rect.setSize(sf::Vector2f(m_area.width, m_area.height));
	render.draw(rect);
	m_sprite.draw(render, states);
}

void	CharacterNpc::timeEvent(sf::Time frameTime)
{
	switch (m_sprite.getCurrentEvent())
	{
		case Idle:
			m_timeEventIdle += frameTime;
			break;
		default:
			m_timeEventIdle = sf::Time::Zero;
	}
}

void	CharacterNpc::updateState()
{
	sf::FloatRect const& bounds = m_box->getGlobalBounds();
	if ((bounds.left + bounds.width) <= (m_area.left + m_area.width) && m_sprite.getCurrentEvent() == Idle && canWalk())
	{
		m_sprite.setNextEvent(Right);
		if (m_originMove)
		{
			m_sprite.setScale(1, 1);
			m_sprite.setOrigin(m_sprite.getOrigin().x - 177, 0);
			m_originMove = false;
		}
	}
	else if (bounds.left >= m_area.left && m_sprite.getCurrentEvent() == Idle && canWalk())
	{
		m_sprite.setNextEvent(Left);
		if (!m_originMove)
		{
			m_sprite.setScale(-1, 1);
			m_sprite.setOrigin(m_sprite.getOrigin().x + 177, 0);
			m_originMove = true;
		}
	}
	else if ((bounds.left <= m_area.left || (bounds.left + bounds.width) >= (m_area.left + m_area.width))
			&& m_sprite.getCurrentEvent() != Idle && canWalk())
	{
		m_sprite.setNextEvent(Idle);
	}
}

void	CharacterNpc::updatePhysics(sf::Time frameTime)
{
	sf::Vector2f	velocity = m_box->getVelocity();
	if (m_sprite.getCurrentEvent() == Left)
	{
		velocity.x = (-1.f * m_pixelSecondWalk) * frameTime.asSeconds();
	}
	else if (m_sprite.getCurrentEvent() == Right)
	{
		velocity.x = m_pixelSecondWalk * frameTime.asSeconds();
	}
	m_box->setVelocity(velocity);
}

void	CharacterNpc::commitPhysicsToGraphics()
{
	sf::FloatRect const& bounds = m_box->getGlobalBounds();

	// TODO fix that
	m_sprite.setPosition(bounds.left - 65, bounds.top);
}

bool	CharacterNpc::canWalk()
{
	if (m_timeEventIdle > sf::seconds(1.4f) || m_timeEventIdle == sf::Time::Zero)
		return true;
	return false;
}

sf::Vector2f	CharacterNpc::getBubblePosition() const
{
	return (m_box->getBaryCenter() + sf::Vector2f(-40.f, -40.f));
}

