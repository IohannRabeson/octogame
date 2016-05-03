#include "Firecamp.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

Firecamp::Firecamp(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(FIRECAMP_OSS, scale, position, 3u)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_spriteFire.setSpriteSheet(resources.getSpriteSheet(FIREBALL_OSS));
	m_spriteFire.setScale(scale);
	m_spriteFire.setPosition(position.x + 50.f, position.y + 25.f);

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.1f), 0u);
	frames.emplace_back(sf::seconds(0.1f), 1u);
	frames.emplace_back(sf::seconds(0.1f), 2u);
	frames.emplace_back(sf::seconds(0.1f), 3u);
	frames.emplace_back(sf::seconds(0.1f), 4u);
	m_animationFire.setFrames(frames);
	m_animationFire.setLoop(octo::LoopMode::Loop);
	m_spriteFire.setAnimation(m_animationFire);
	m_spriteFire.play();
	setupSmoke(position);
}

void Firecamp::setupSmoke(sf::Vector2f const & position)
{
	m_smoke.setup(sf::Vector2f(5.f, 5.f));
	m_smoke.setVelocity(sf::Vector2f(0.f, -100.f));
	m_smoke.setEmitTimeRange(0.2f, 0.3f);
	m_smoke.setGrowTimeRange(0.4f, 0.6f);
	m_smoke.setLifeTimeRange(0.6f, 0.8f);
	m_smoke.setScaleFactor(10.f);
	m_smoke.setDispersion(80.f);
	m_smoke.setColor(sf::Color(55, 55, 55, 150));
	m_smoke.setPosition(position + sf::Vector2f(70.f, 0.f));
}

void Firecamp::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_spriteFire.setPosition(m_spriteFire.getPosition().x + x, m_spriteFire.getPosition().y + y);
	m_smoke.setPosition(m_smoke.getPositionEmitter() + sf::Vector2f(x, y));
}

void Firecamp::setPosition(sf::Vector2f const & position)
{
	Firecamp::setPosition(position);
	m_spriteFire.setPosition(position);
	m_smoke.setPosition(position + sf::Vector2f(70.f, 0.f));
}

void Firecamp::update(sf::Time frameTime)
{
	InstanceDecor::update(frameTime);
	m_spriteFire.update(frameTime);
	m_smoke.update(frameTime);
}

void Firecamp::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	m_spriteFire.draw(render, states);
	InstanceDecor::draw(render, states);
	m_smoke.draw(render);
}
