#include "Door.hpp"
#include "CharacterOcto.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"
#include "SkyCycle.hpp"

Door::Door(SkyCycle & skyCycle, sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(DOOR_OSS, scale, position, 1, 1.f),
	m_lock(false),
	m_box(PhysicsEngine::getShapeBuilder().createRectangle()),
	m_skyCycle(skyCycle),
	m_timer(sf::Time::Zero),
	m_timerMax(sf::seconds(1.f)),
	m_actionEnable(false)
{
	m_box->setSize(sf::Vector2f(75.f, 180.f));
	m_box->setPosition(position);
	m_box->update();

	m_box->setGameObject(this);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Door));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));
}

void Door::update(sf::Time frametime)
{
	InstanceDecor::update(frametime);
	if (m_actionEnable)
	{
		m_timer += frametime;
		m_timer = std::min(m_timer, m_timerMax);
		m_lock = true;
	}
	else
	{
		m_timer -= frametime;
		m_timer = std::max(m_timer, sf::Time::Zero);
		if (m_timer == sf::Time::Zero)
			m_lock = false;
	}
	if (m_lock)
		m_skyCycle.setSpeedCoeff(1.f + m_timer / m_timerMax * 20.f);
	m_actionEnable = false;
}

void Door::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_box->setPosition(m_box->getPosition() + sf::Vector2f(x, y));
	m_box->update();
}

void Door::setPosition(sf::Vector2f const & position)
{
	InstanceDecor::setPosition(position);
	m_box->setPosition(position);
	m_box->update();
}

void Door::collideOcto(CharacterOcto * octo)
{
	m_actionEnable = octo->isActionEnable();
}

void Door::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	InstanceDecor::draw(render, states);
}
