#include "MonolithStep.hpp"
#include <Application.hpp>
#include <ResourceManager.hpp>

MonolithStep::MonolithStep(ResourceKey const & key, sf::Vector2f const & position, sf::Vector2f const & scale)
{
	octo::ResourceManager & resources = octo::Application::getResourceManager();

	octo::SpriteAnimation::FrameList	frames;
	frames.emplace_back(sf::seconds(0.2f), 0u);
	frames.emplace_back(sf::seconds(0.2f), 1u);
	frames.emplace_back(sf::seconds(0.2f), 2u);
	frames.emplace_back(sf::seconds(0.2f), 3u);
	m_animation.setFrames(frames);
	m_animation.setLoop(octo::LoopMode::NoLoop);

	m_sprite.setSpriteSheet(resources.getSpriteSheet(key));
	m_sprite.setScale(scale);
	m_sprite.setPosition(position.x, position.y);
	m_sprite.setAnimation(m_animation);
	m_sprite.stop();
}

void MonolithStep::addMapOffset(float x, float y)
{
	m_sprite.setPosition(m_sprite.getPosition().x + x, m_sprite.getPosition().y + y);
}

void MonolithStep::setPosition(sf::Vector2f const & position)
{
	m_sprite.setPosition(position);
}

void MonolithStep::update(sf::Time frametime)
{
	m_sprite.update(frametime);
}

void MonolithStep::draw(sf::RenderTarget& render, sf::RenderStates states) const
{
	m_sprite.draw(render, states);
}
