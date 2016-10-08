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
	m_box(PhysicsEngine::getShapeBuilder().createRectangle()),
	m_velocity(0, -150.f),
	m_scale(scale.x)
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
	m_smoke.setVelocity(m_velocity * m_scale);
	m_smoke.setEmitTimeRange(0.15f * m_scale, 0.25f * m_scale);
	m_smoke.setGrowTimeRange(0.4f * m_scale, 0.7f * m_scale);
	m_smoke.setLifeTimeRange(0.6f * m_scale, 0.8f * m_scale);
	m_smoke.setScaleFactor(10.f * m_scale);
	m_smoke.setDispersion(80.f * m_scale);
	m_smoke.setColor(sf::Color(240, 240, 240, 150));
	m_smoke.setPosition(position);
}

void SmokeInstance::update(sf::Time frametime)
{
	InstanceDecor::update(frametime);

	sf::Vector2f const & positionSmoke = m_smoke.getPositionEmitter();

	if (m_collideEvent && m_positionOcto.y < positionSmoke.y && positionSmoke.y - m_positionOcto.x < 200.f)
	{
		if (m_positionOcto.x < positionSmoke.x && positionSmoke.x - m_positionOcto.x < 100.f)
		{
			m_smoke.setVelocity(sf::Vector2f(-(positionSmoke.x - m_positionOcto.x), m_velocity.y));

			m_smoke.setEmitTimeRange(0.3f * m_scale, 0.4f * m_scale);
			m_smoke.setScaleFactor(15.f * m_scale);
			m_smoke.setDispersion(200.f * m_scale);
		}
		else if (positionSmoke.x < m_positionOcto.x && m_positionOcto.x - positionSmoke.x < 100.f)
		{
			m_smoke.setVelocity(sf::Vector2f((m_positionOcto.x - positionSmoke.x), m_velocity.y));

			m_smoke.setEmitTimeRange(0.3f * m_scale, 0.4f * m_scale);
			m_smoke.setScaleFactor(15.f * m_scale);
			m_smoke.setDispersion(200.f * m_scale);
		}
	}
	else
	{
		m_smoke.setVelocity(m_velocity);
		m_smoke.setEmitTimeRange(0.15f * m_scale, 0.25f * m_scale);
		m_smoke.setScaleFactor(10.f * m_scale);
		m_smoke.setDispersion(80.f * m_scale);
	}

	m_collideEvent = false;
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
	m_collideEvent = true;
	m_positionOcto = octo->getPosition();
}

void SmokeInstance::draw(sf::RenderTarget & render, sf::RenderStates) const
{
	m_smoke.draw(render);
}
