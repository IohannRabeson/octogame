#include "SimpleObject.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

SimpleObject::SimpleObject(ResourceKey key)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	m_sprite.setSpriteSheet(resources.getSpriteSheet(key));
	m_sprite.setScale(1.f, 1.f);
}

void SimpleObject::setupAnimation(std::initializer_list<octo::SpriteAnimation::Frame> list, octo::LoopMode loop)
{
	m_animation.setFrames(list);
	m_animation.setLoop(loop);

	m_sprite.setAnimation(m_animation);
	m_sprite.play();
}

void SimpleObject::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position.x, position.y);
}

void SimpleObject::update(sf::Time frameTime)
{
	m_sprite.update(frameTime);
}

void SimpleObject::draw(sf::RenderTarget& render, sf::RenderStates) const
{
	render.draw(m_sprite);
}
