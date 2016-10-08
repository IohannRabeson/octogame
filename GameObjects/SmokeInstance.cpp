#include "SmokeInstance.hpp"
#include "CharacterOcto.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>

SmokeInstance::SmokeInstance(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(SMOKE_INSTANCE_OSS),
	m_box(PhysicsEngine::getShapeBuilder().createRectangle())
{
	m_box->setSize(16.f, 16.f);
	m_box->setPosition(position);
	m_box->update();

	m_box->setGameObject(this);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::CheckPoint));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::PlayerEvent));

	m_smoke.setup(sf::Vector2f(5.f, 5.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -100.f) * scale.x);
	m_smoke.setEmitTimeRange(0.2f * scale.x, 0.3f * scale.y);
	m_smoke.setGrowTimeRange(0.4f * scale.x, 0.6f * scale.y);
	m_smoke.setLifeTimeRange(0.6f * scale.x, 0.8f * scale.y);
	m_smoke.setScaleFactor(10.f * scale.x);
	m_smoke.setDispersion(80.f * scale.x);
	m_smoke.setColor(sf::Color(240, 240, 240, 150));
	m_smoke.setPosition(position);
}

void SmokeInstance::update(sf::Time frametime)
{
	InstanceDecor::update(frametime);
	m_smoke.update(frametime);
}

void SmokeInstance::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_box->setPosition(m_box->getPosition() + sf::Vector2f(x, y));
	m_box->update();
	m_smoke.setPosition(m_box->getPosition() + sf::Vector2f(8.f, 0.f));
}

void SmokeInstance::setPosition(sf::Vector2f const & position)
{
	InstanceDecor::setPosition(position);
	m_box->setPosition(position);
	m_box->update();
	m_smoke.setPosition(position + sf::Vector2f(8.f, 0.f));
}

void SmokeInstance::collideOctoEvent(CharacterOcto * octo)
{
	(void)octo;
	m_smoke.setVelocity(sf::Vector2f(0.f, -200.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
}

void SmokeInstance::draw(sf::RenderTarget & render, sf::RenderStates) const
{
	m_smoke.draw(render);
}
