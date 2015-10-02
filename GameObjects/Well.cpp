#include "Well.hpp"
#include "ResourceDefinitions.hpp"
#include "PhysicsEngine.hpp"
#include "CircleShape.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

Well::Well(void)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(WELL_OSS));
	m_sprite.setScale(0.8f, 0.8f);

	typedef octo::SpriteAnimation::Frame	Frame;
	m_animation.setFrames({	Frame(sf::seconds(0.2f), 0u)});
	m_animation.setLoop(octo::LoopMode::Loop);

	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

void Well::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position.x, position.y - 160.f);
}

sf::Vector2f const & Well::getPosition(void) const
{
	return m_sprite.getPosition();
}

void Well::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
}

void Well::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_sprite);
}
