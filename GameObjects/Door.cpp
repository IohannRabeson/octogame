#include "Door.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"

Door::Door(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(DOOR_OSS, scale, position, 1, 1.f),
	m_box(PhysicsEngine::getShapeBuilder().createRectangle())
{
	m_box->setSize(sf::Vector2f(75.f, 180.f));
	m_box->setPosition(position);
	m_box->update();

	m_box->setGameObject(this);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::Door));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::Player));

	getSprite().setColor(sf::Color::Transparent);
}

void Door::update(sf::Time frametime)
{
	InstanceDecor::update(frametime);
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

void Door::draw(sf::RenderTarget & render, sf::RenderStates states) const
{
	InstanceDecor::draw(render, states);
}
