#include "SpaceShip.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include "Progress.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

SpaceShip::SpaceShip(SpaceShipEvents event) :
	m_generator("random"),
	m_musicCount(20u),
	m_music(new MusicSystem[m_musicCount]),
	m_event(event),
	m_box(nullptr),
	m_isFinal(false)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_isFinal = Progress::getInstance().canRepairShip();

	if (event == Broken)
	{
		m_sprite.setSpriteSheet(resources.getSpriteSheet(BROKEN_SHIP_OSS));
		m_sprite.setScale(1.f, 1.f);

		typedef octo::SpriteAnimation::Frame	Frame;
		m_animation.setFrames({	Frame(sf::seconds(0.2f), 0u),
								Frame(sf::seconds(0.2f), 1u),
								Frame(sf::seconds(0.2f), 2u),
								Frame(sf::seconds(0.2f), 3u)});
		m_animation.setLoop(octo::LoopMode::Loop);
		m_smoke.setup(sf::Vector2f(10.f, 10.f));
		m_smoke.setVelocity(sf::Vector2f(0.f, -140.f));
		m_box = PhysicsEngine::getShapeBuilder().createCircle(false);
		m_box->setGameObject(this);
		m_box->setCollisionType(static_cast<std::size_t>(GameObjectType::SpaceShip));
		m_box->setCollisionMask(static_cast<std::size_t>(GameObjectType::PlayerEvent));
		m_box->setRadius(300.f);
		m_box->setApplyGravity(false);
		m_box->setType(AShape::Type::e_trigger);
	}
	else if (event == Flying)
	{
		m_sprite.setSpriteSheet(resources.getSpriteSheet(SHIP_OSS));
		m_sprite.setScale(1.f, 1.f);

		typedef octo::SpriteAnimation::Frame	Frame;
		m_animation.setFrames({	Frame(sf::seconds(0.05f), 0u),
								Frame(sf::seconds(0.05f), 1u),
								Frame(sf::seconds(0.05f), 2u),
								Frame(sf::seconds(0.05f), 3u),
								Frame(sf::seconds(0.05f), 4u),
								Frame(sf::seconds(0.05f), 5u),
								Frame(sf::seconds(0.05f), 6u),
								Frame(sf::seconds(0.05f), 7u)});
		m_animation.setLoop(octo::LoopMode::Loop);

		m_smoke.setup(sf::Vector2f(10.f, 10.f));
		m_smoke.setVelocity(sf::Vector2f(0.f, -140.f));

		for (std::size_t i = 0u; i < m_musicCount; i++)
			m_music[i].canEmit(true);
	}
	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

void SpaceShip::setNextEvent(SpaceShipEvents event)
{
	m_event = event;
}

void SpaceShip::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position.x, position.y - m_sprite.getLocalSize().y * 0.775f);
	if (m_event == Broken)
	{
		m_smoke.setPosition(position + sf::Vector2f(220.f, -200.f));
		if (m_box)
		{
			m_box->setPosition(m_sprite.getPosition() + m_sprite.getGlobalSize() / 2.f - sf::Vector2f(m_box->getRadius(), m_box->getRadius()));
			m_box->update();
		}
	}
}

sf::Vector2f const & SpaceShip::getPosition(void) const
{
	return m_sprite.getPosition();
}

sf::Vector2f SpaceShip::getSize(void) const
{
	return m_sprite.getGlobalSize();
}

void SpaceShip::setSmokeVelocity(sf::Vector2f const & velocity)
{
	m_smoke.setVelocity(velocity);
}

void SpaceShip::setRepairProgression(float progression)
{
	m_smoke.setVelocity(sf::Vector2f(0.f, -200.f));
	if ((progression > 0.5f && progression < 0.55f) ||
		(progression > 0.7f && progression < 0.75f) ||
		(progression > 0.9f && progression < 0.95f))
	{
		m_smoke.setEmitTimeRange(0.004f, 0.04f);
		m_smoke.setGrowTimeRange(0.8f, 1.9f);
		m_smoke.setLifeTimeRange(3.f, 5.f);
		m_smoke.setScaleFactor(2.f);
	}
	else if (progression < 1.f)
	{
		float x = 0.f;
		if (progression < 0.5f)
			x = 1.f - std::min(progression * 1.5f, 1.f);
		else
			x = 1.f - std::min(0.5f * 1.5f + (progression - 0.5f), 1.f);

		m_smoke.setEmitTimeRange(0.01f * (1.f + progression), 0.2f * (1.f + progression));
		m_smoke.setGrowTimeRange(0.5f * x, 1.5f * x);
		m_smoke.setLifeTimeRange(2.5f * x, 4.f * x);
		m_smoke.setScaleFactor(1.f * x);
	}
	else
	{
		m_smoke.setEmitTimeRange(0.01f, 0.2f);
		m_smoke.setGrowTimeRange(0.5f, 1.5f);
		m_smoke.setLifeTimeRange(2.5f, 4.f);
		m_smoke.setScaleFactor(1.f);
	}
}

void SpaceShip::move(sf::Vector2f const & translation)
{
	m_sprite.setPosition(m_sprite.getPosition() + translation);
	m_smoke.setPosition(m_sprite.getPosition() + sf::Vector2f(1000.f, 280.f));

	for (std::size_t i = 0u; i < m_musicCount; i++)
		m_music[i].setEmitter(m_sprite.getPosition() + sf::Vector2f(1000.f + m_generator.randomFloat(-50.f, 50.f), 170.f + m_generator.randomFloat(-60.f, 60.f)));
}

void SpaceShip::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
	m_smoke.update(frameTime);
	for (std::size_t i = 0u; i < m_musicCount; i++)
		m_music[i].update(frameTime);
}

bool SpaceShip::isInScreen(void)
{
	octo::Camera const & camera = octo::Application::getCamera();
	sf::FloatRect rect = m_sprite.getGlobalBounds();
	rect.left -= 500.f;
	rect.width += 1000.f;

	if (camera.getRectangle().intersects(rect))
		return true;
	return false;
}

void SpaceShip::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	if (m_isFinal)
	{
		for (std::size_t i = 0u; i < m_musicCount; i++)
			m_music[i].draw(render);
		render.draw(m_sprite);
		m_smoke.draw(render);
	}
}

void SpaceShip::drawFront(sf::RenderTarget& render, sf::RenderStates) const
{
	for (std::size_t i = 0u; i < m_musicCount; i++)
		m_music[i].draw(render);
	render.draw(m_sprite);
	m_smoke.draw(render);
}
