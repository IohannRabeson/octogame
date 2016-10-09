#include "SmokeInstance.hpp"
#include "CharacterOcto.hpp"
#include "Progress.hpp"
#include "RectangleShape.hpp"
#include "PhysicsEngine.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>
#include <AudioManager.hpp>

SmokeInstance::SmokeInstance(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(SMOKE_INSTANCE_OSS, scale, position, 1, 1.f),
	m_box(PhysicsEngine::getShapeBuilder().createRectangle()),
	m_velocity(0, -100.f * scale.x),
	m_scale(scale.x),
	m_collideEvent(false)
{
	m_box->setSize(16.f * m_scale, 16.f * m_scale);
	m_box->setPosition(position);
	m_box->update();

	m_box->setGameObject(this);
	m_box->setApplyGravity(false);
	m_box->setType(AShape::Type::e_trigger);
	m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::CheckPoint));
	m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::PlayerEvent));

	m_smoke.setup(sf::Vector2f(3.f * m_scale, 3.f * m_scale));
	m_smoke.setVelocity(m_velocity);
	m_smoke.setEmitTimeRange(0.15f, 0.25f);
	m_smoke.setGrowTimeRange(0.4f, 0.7f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(15.f);
	m_smoke.setDispersion(130.f);
	m_smoke.setColor(sf::Color(240, 240, 240, 150));
	m_smoke.setPosition(position + sf::Vector2f(m_box->getSize().x / 2.f, 0.f));
}

void SmokeInstance::update(sf::Time frametime)
{
	InstanceDecor::update(frametime);

	sf::Vector2f positionSmoke = m_smoke.getPositionEmitter();
	positionSmoke.y += 100.f;
	//Avoid negative values
	positionSmoke.x += 10000000.f * 16.f;
	m_positionOcto.x += 10000000.f * 16.f;

	if (m_collideEvent && m_positionOcto.y < positionSmoke.y && positionSmoke.y - m_positionOcto.y < 200.f)
	{
		if (m_positionOcto.x < positionSmoke.x && m_lastPositionOcto.x > m_positionOcto.x && positionSmoke.x - m_positionOcto.x < 200.f)
		{
			m_velocity.x = -1.f * (positionSmoke.x - m_positionOcto.x);
			m_smoke.setScaleFactor(20.f);
		}
		else if (positionSmoke.x < m_positionOcto.x && m_lastPositionOcto.x < m_positionOcto.x && m_positionOcto.x - positionSmoke.x < 200.f)
		{
			m_velocity.x = m_positionOcto.x - positionSmoke.x;
			m_smoke.setScaleFactor(20.f);
		}
	}
	else
	{
		if (m_velocity.x > 0.f)
			m_velocity.x = std::min(m_velocity.x - frametime.asSeconds() * 10.f, 0.f);
		if (m_velocity.x < 0.f)
			m_velocity.x = std::max(m_velocity.x + frametime.asSeconds() * 10.f, 0.f);
		m_smoke.setScaleFactor(15.f);
	}
	m_smoke.setVelocity(m_velocity);
	m_smoke.update(frametime);

	m_lastPositionOcto = m_positionOcto;
	m_collideEvent = false;
}

void SmokeInstance::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_box->setPosition(m_box->getPosition() + sf::Vector2f(x, y));
	m_box->update();
	m_smoke.setPosition(m_box->getPosition() + sf::Vector2f(m_box->getSize().x / 2.f, 0.f));
}

void SmokeInstance::setPosition(sf::Vector2f const & position)
{
	InstanceDecor::setPosition(position);
	m_box->setPosition(position);
	m_box->update();
	m_smoke.setPosition(position + sf::Vector2f(m_box->getSize().x / 2.f, 0.f));
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
