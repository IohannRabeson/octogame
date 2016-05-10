#include "JellyfishNpc.hpp"
#include "RectangleShape.hpp"
#include "SkyCycle.hpp"
#include "CircleShape.hpp"
#include "CharacterOcto.hpp"
#include <Interpolations.hpp>

JellyfishNpc::JellyfishNpc(void) :
	ANpc(JELLYFISH_OSS, false),
	m_waterLevel(0.f),
	m_isMet(false)
{
	setSize(sf::Vector2f(10.f, 300.f));
	setOrigin(sf::Vector2f(60.f, 75.f));
	setScale(0.8f);
	setVelocity(50.f);
	setTextOffset(sf::Vector2f(100.f, -80.f));
	setup();

	setupBox(this, static_cast<std::size_t>(GameObjectType::LucienNpc), static_cast<std::size_t>(GameObjectType::PlayerEvent));
}

void JellyfishNpc::setup(void)
{
	typedef octo::CharacterAnimation::Frame			Frame;

	getIdleAnimation().setFrames({
			Frame(sf::seconds(1.f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			});
	getIdleAnimation().setLoop(octo::LoopMode::Loop);

	getSpecial1Animation().setFrames({
			Frame(sf::seconds(0.4f), {0u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {1u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {2u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {3u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {4u, sf::FloatRect(), sf::Vector2f()}),
			Frame(sf::seconds(0.4f), {5u, sf::FloatRect(), sf::Vector2f()}),
			});
	getSpecial1Animation().setLoop(octo::LoopMode::Loop);

	setupMachine();
}

void JellyfishNpc::setupMachine(void)
{
	typedef octo::CharacterSprite::ACharacterState	State;
	typedef octo::FiniteStateMachine::StatePtr		StatePtr;

	octo::FiniteStateMachine	machine;
	StatePtr					idle;
	StatePtr					special1;

	idle = std::make_shared<State>("0", getIdleAnimation(), getSprite());
	special1 = std::make_shared<State>("3", getSpecial1Animation(), getSprite());

	machine.setStart(idle);
	machine.addTransition(Idle, idle, idle);
	machine.addTransition(Idle, special1, idle);

	machine.addTransition(Special1, special1, special1);
	machine.addTransition(Special1, idle, special1);

	setMachine(machine);
	setNextEvent(Idle);
}

void JellyfishNpc::setPosition(sf::Vector2f const & position)
{
	if (!m_isMet)
		ANpc::setPosition(position);
}

void JellyfishNpc::update(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();

	computeBehavior(frametime);
	updateState();
	updatePhysics();

	sprite.update(frametime);
	sprite.setPosition(getBox()->getRenderPosition());

	updateText(frametime);
	resetVariables();
}

void JellyfishNpc::computeBehavior(sf::Time frametime)
{
	octo::CharacterSprite & sprite = getSprite();
	RectangleShape * box = getBox();
	if (sprite.getCurrentEvent() == Special1)
	{
		sf::Vector2f position = octo::linearInterpolation(m_octoPosition, box->getPosition(), 1.f - frametime.asSeconds());
		if (position.y > m_waterLevel + 200.f)
			box->setPosition(position);
		float angle = octo::rad2Deg(std::atan2(box->getPosition().y - m_octoPosition.y, box->getPosition().x - m_octoPosition.x)) - 90.f;
		if (angle < 0.f)
			angle += 360.f;
		//float diff = angle - sprite.getRotation();
		//sprite.rotate(diff * frametime.asSeconds() * 2.f);
		sprite.setRotation(angle);
	}
	else if (m_isMet)
	{
		if (sprite.getRotation() > 180.f)
			sprite.setRotation(octo::linearInterpolation(360.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
		else
			sprite.setRotation(octo::linearInterpolation(0.f, sprite.getRotation(), 1.f - frametime.asSeconds()));
		if (box->getPosition().y > m_waterLevel + 200.f)
			box->setPosition(sf::Vector2f(box->getPosition().x, box->getPosition().y - frametime.asSeconds() * 150.f));
	}
}

void JellyfishNpc::updateState(void)
{
	octo::CharacterSprite & sprite = getSprite();

	if (sprite.getCurrentEvent() == Idle && getCollideEventOcto() && m_octoPosition.y > m_waterLevel)
		sprite.setNextEvent(Special1);
	else if (sprite.getCurrentEvent() == Special1 && !getCollideEventOcto())
		sprite.setNextEvent(Idle);
}

void JellyfishNpc::updatePhysics(void)
{
	getBox()->update();
}

void JellyfishNpc::collideOctoEvent(CharacterOcto * octo)
{
	ANpc::collideOctoEvent(octo);
	m_octoPosition = octo->getPosition();
	if (!m_isMet)
		m_isMet = true;
	if (m_waterLevel == 0.f)
		m_waterLevel = octo->getWaterLevel();
}

void JellyfishNpc::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	ANpc::draw(render, states);
}
