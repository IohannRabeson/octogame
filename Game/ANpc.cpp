#include "ANpc.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

ANpc::ANpc(ResourceKey const & npcId) :
	m_box(PhysicsEngine::getShapeBuilder().createRectangle(false)),
	m_velocity(200.f)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(npcId));

	m_box->setGameObject(this);
	setBoxCollision(static_cast<std::size_t>(GameObjectType::Npc), 0u);
}

ANpc::~ANpc(void)
{
	PhysicsEngine::getInstance().unregisterShape(m_box);
}

void ANpc::setupIdleAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	setupAnimation(m_idleAnimation, list, loopMode);
}

void ANpc::setupWalkAnimation(std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	setupAnimation(m_walkAnimation, list, loopMode);
}

void ANpc::setupAnimation(octo::CharacterAnimation & animation, std::initializer_list<FramePair> list, octo::LoopMode loopMode)
{
	typedef octo::CharacterAnimation::Frame		Frame;
	std::vector<Frame>							frames;

	for (auto & frame : list)
		frames.push_back(Frame(sf::seconds(frame.first), { frame.second, sf::FloatRect(), sf::Vector2f() }));
	animation.setFrames(frames);
	animation.setLoop(loopMode);
}

void ANpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idleState;
	StatePtr					walkLeftState;
	StatePtr					walkRightState;

	idleState = std::make_shared<State>("0", m_idleAnimation, m_sprite);
	walkLeftState = std::make_shared<State>("1", m_walkAnimation, m_sprite);
	walkRightState = std::make_shared<State>("2", m_walkAnimation, m_sprite);

	machine.setStart(idleState);
	machine.addTransition(Idle, idleState, idleState);
	machine.addTransition(Idle, walkLeftState, idleState);
	machine.addTransition(Idle, walkRightState, idleState);

	machine.addTransition(Left, idleState, walkLeftState);

	machine.addTransition(Right, idleState, walkRightState);

	m_sprite.setMachine(machine);
	m_sprite.restart();
	m_sprite.setNextEvent(Idle);
}

bool ANpc::canWalk(void) const
{
	return true;
}

bool ANpc::canJump(void) const
{
	return false;
}

bool ANpc::canDoubleJump(void) const
{
	return false;
}

bool ANpc::canDance(void) const
{
	return false;
}

bool ANpc::canSpecial(void) const
{
	return false;
}

void ANpc::setArea(sf::FloatRect const & area)
{
	m_area = area;
}

void ANpc::setOrigin(sf::Vector2f const & origin)
{
	m_origin = origin;
	m_sprite.setOrigin(origin);
}

void ANpc::setPosition(sf::Vector2f const & position)
{
	m_box->setPosition(position.x, position.y - m_box->getSize().y);
}

void ANpc::setVelocity(float velocity)
{
	m_velocity = velocity;
}

void ANpc::setBoxCollision(std::size_t type, std::size_t mask)
{
	m_box->setCollisionType(type);
	m_box->setCollisionMask(mask);
}

void ANpc::setSize(sf::Vector2f const & size)
{
	m_box->setSize(size);
}

void ANpc::addMapOffset(float x, float y)
{
	m_box->setPosition(m_box->getPosition().x + x, m_box->getPosition().y + y);
	m_box->update(); // We must update ourselves because the box is out of the screen, and the engine didn't update shape out of the screen
	m_area.left += x;
	m_area.top += y;
}

sf::Vector2f const & ANpc::getPosition(void) const
{
	return m_box->getPosition();
}

void ANpc::doSpecialAction(void)
{
}

void ANpc::update(sf::Time frametime)
{
	updateState();
	updatePhysics();

	m_sprite.update(frametime);
	sf::FloatRect const & bounds = m_box->getGlobalBounds();
	m_sprite.setPosition(bounds.left, bounds.top);
}

void ANpc::updateState(void)
{
	sf::FloatRect const & bounds = m_box->getGlobalBounds();
	if ((bounds.left + bounds.width) <= (m_area.left + m_area.width) && m_sprite.getCurrentEvent() == Idle && canWalk())
	{
		m_sprite.setNextEvent(Right);
		m_sprite.setOrigin(m_origin.x, 0.f);
		m_sprite.setScale(1, 1);
	}
	else if (bounds.left >= m_area.left && m_sprite.getCurrentEvent() == Idle && canWalk())
	{
		m_sprite.setNextEvent(Left);
		sf::Vector2f const & size = m_sprite.getLocalSize();
		m_sprite.setOrigin(size.x - m_origin.x, 0.f);
		m_sprite.setScale(-1, 1);
	}
	else if ((bounds.left <= m_area.left || (bounds.left + bounds.width) >= (m_area.left + m_area.width))
			&& m_sprite.getCurrentEvent() != Idle)
	{
		m_sprite.setNextEvent(Idle);
	}
}

void ANpc::updatePhysics(void)
{
	sf::Vector2f velocity;
	if (m_sprite.getCurrentEvent() == Left)
	{
		velocity.x = (-1.f * m_velocity);
	}
	else if (m_sprite.getCurrentEvent() == Right)
	{
		velocity.x = m_velocity;
	}
	m_box->setVelocity(velocity);
}

void ANpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	m_sprite.draw(render, states);
}
