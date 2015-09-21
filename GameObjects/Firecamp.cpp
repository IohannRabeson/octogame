#include "Firecamp.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

Firecamp::Firecamp(sf::Vector2f const & scale, sf::Vector2f const & position) :
	InstanceDecor(FIRECAMP_OSS, scale, position, 3u)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_spriteFire.setSpriteSheet(resources.getSpriteSheet(FIREBALL_OSS));
	m_spriteFire.setScale(scale);
	m_spriteFire.setPosition(position.x + 93.f, position.y);
	m_spriteFire.rotate(90);

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.2f), 0u);
	frames.emplace_back(sf::seconds(0.2f), 1u);
	frames.emplace_back(sf::seconds(0.2f), 2u);
	frames.emplace_back(sf::seconds(0.2f), 3u);
	frames.emplace_back(sf::seconds(0.2f), 4u);
	frames.emplace_back(sf::seconds(0.2f), 5u);
	frames.emplace_back(sf::seconds(0.2f), 6u);
	m_animationFire.setFrames(frames);
	m_animationFire.setLoop(octo::LoopMode::Loop);
	m_spriteFire.setAnimation(m_animationFire);
	m_spriteFire.play();
}

void Firecamp::addMapOffset(float x, float y)
{
	InstanceDecor::addMapOffset(x, y);
	m_spriteFire.setPosition(m_spriteFire.getPosition().x + x, m_spriteFire.getPosition().y + y);
}

void Firecamp::setPosition(sf::Vector2f const & position)
{
	Firecamp::setPosition(position);
	m_spriteFire.setPosition(position);
}

void Firecamp::update(sf::Time frameTime)
{
	InstanceDecor::update(frameTime);
	m_spriteFire.update(frameTime);
}

void Firecamp::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	m_spriteFire.draw(render, states);
	InstanceDecor::draw(render, states);
}
